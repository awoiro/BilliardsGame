#include <stdlib.h>
#include <vector>
#include "MeshData.h"

#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"wininet.lib")


HRESULT MeshImporter::InitFBXImporter()
{
	m_pFBXManager = FbxManager::Create();
	return S_OK;
}

void MeshImporter::DestroyFBXImporter()
{
	if (m_pFBXManager) m_pFBXManager->Destroy();
}

FbxScene * MeshImporter::GetFBXData(CHAR * pFileName)
{
	if (m_pFBXManager == nullptr) { IS_CHECK_ERROR(false, "FBXImporterが正しく作成されていません"); return nullptr; }

	FbxImporter* pImporter = FbxImporter::Create(m_pFBXManager, "");

	int iFormat = -1;
	bool isOK = pImporter->Initialize((const char*)pFileName, iFormat);
	if (!isOK)
	{
		CHECK_ERROR(E_FAIL, "FBX読み込み失敗");
	}

	FbxScene* pFBXScene = FbxScene::Create(m_pFBXManager, "");

	pImporter->Import(pFBXScene);

	// ポリゴンを三角化
	FbxGeometryConverter geometryConverter(m_pFBXManager);
	geometryConverter.Triangulate(pFBXScene, true);

	return pFBXScene;
}

MeshData * MeshImporter::CreateMeshData(CHAR* pFileName)
{
	FbxScene* pScene = GetFBXData(pFileName);
	if (!pScene) { IS_CHECK_ERROR(false, "FBXデータの取得に失敗"); return nullptr; }

	FbxNode* pRootNode = pScene->GetRootNode();
	if (!pRootNode) { IS_CHECK_ERROR(false, "FBXデータの取得に失敗"); return nullptr; }

	MeshData* pRootMeshObject = GetMeshData(pRootNode);

	return pRootMeshObject;
}

void MeshImporter::RenderMesh(D3DXMATRIX * pWorld, MeshData * pMeshData)
{
	pMeshData->m_finalWorld = *pWorld;

	FbxMesh* pMesh = pMeshData->pFBXMesh;//GetFBXMesh();
	if (pMesh)
	{
		FbxMatrix currentLocalTransform = pMesh->GetNode()->EvaluateLocalTransform(0);
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				pMeshData->m_myOrientation(x, y) = currentLocalTransform.Get(x, y);
			}
		}
		pMeshData->m_parentOrientation = *pWorld;
		pMeshData->m_finalWorld = pMeshData->m_myOrientation * pMeshData->m_parentOrientation;
	}

	ID3D11Buffer* pVertexBuffer = pMeshData->pVertexBuffer;//GetVertexBuffer();
	MaterialData* pMaterialData = pMeshData->pMaterials;//GetMaterials();

	if (pVertexBuffer && pMaterialData)
	{
		UINT stride = sizeof(VERTEX_DATA);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		const int MATERIAL_COUNT = pMeshData->m_materialCount;//GetMaterialCount();
		for (int i = 0; i < MATERIAL_COUNT; i++)
		{
			if (pMaterialData[i].TriangleCount == 0)
			{
				continue;
			}
			// インデックスバッファーをセット
			UINT stride = sizeof(int);
			UINT offset = 0;
			ID3D11Buffer** ppIndexBuffer = pMeshData->ppIndexBuffer;//GetIndexBuffer();
			m_pDeviceContext->IASetIndexBuffer(ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

			m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3D11_MAPPED_SUBRESOURCE pData;
			if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
			{
				ShaderGlobal01 sg;
				sg.world = pMeshData->m_finalWorld;
				D3DXMatrixTranspose(&sg.world, &sg.world);
				sg.WVP = pMeshData->m_finalWorld*m_view*m_proj;
				D3DXMatrixTranspose(&sg.WVP, &sg.WVP);
				sg.ambient = pMaterialData[i].ambient;
				sg.diffuse = pMaterialData[i].diffuse;
				sg.specular = pMaterialData[i].specular;
				memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ShaderGlobal01));
				m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
			}
			m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
			m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer);
			// テクスチャーをシェーダーに渡す
			if (pMaterialData[i].diffuseTextureName[0] != NULL)
			{
				m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
				m_pDeviceContext->PSSetShaderResources(0, 1, &pMaterialData[i].pDiffuseTexture);
			}

			m_pDeviceContext->DrawIndexed(pMaterialData[i].TriangleCount * 3, 0, 0);
		}
	}

	int childCount = pMeshData->m_childCount;//GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		RenderMesh(&pMeshData->m_finalWorld, pMeshData->ppChildMeshData[i]);
	}
}

MeshData * MeshImporter::GetMeshData(FbxNode * pNode)
{
	if (pNode == nullptr) { return nullptr; }

	MeshData* pOutMeshData = new MeshData;

	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();
	FbxNodeAttribute::EType type;
	if (pAttr)
	{
		type = pAttr->GetAttributeType();

		if (type == FbxNodeAttribute::eMesh)
		{
			// このノードのデータを取得
			FbxMesh* pFbxMesh = pNode->GetMesh();

			pOutMeshData->pVertexBuffer = GetVertexBuffer(pFbxMesh);
			pOutMeshData->pMaterials = GetMaterials(pNode);
			pOutMeshData->ppIndexBuffer = GetIndexBuffer(pNode);
			pOutMeshData->pFBXMesh = pFbxMesh;
		}
	}

	// 子供のメッシュ数を取得
	int childMeshDataCount = 0;
	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		FbxNode* pChildNode = pNode->GetChild(i);
		pAttr = pChildNode->GetNodeAttribute();
		type = pAttr->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			childMeshDataCount++;
		}
	}

	// 各種データを設定
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh == nullptr)
	{
		pOutMeshData->m_childCount = childMeshDataCount;
		pOutMeshData->m_materialCount = 0;
		pOutMeshData->m_vertexCount = 0;
		pOutMeshData->m_uvCount = 0;
		pOutMeshData->m_polygonCount = 0;
	}
	else
	{
		pOutMeshData->m_childCount = childMeshDataCount;
		pOutMeshData->m_materialCount = pNode->GetMaterialCount();
		pOutMeshData->m_vertexCount = pMesh->GetControlPointsCount();
		pOutMeshData->m_uvCount = pMesh->GetTextureUVCount();
		pOutMeshData->m_polygonCount = pMesh->GetPolygonCount();
	}

	// 子供のメッシュデータを取得
	if (childMeshDataCount > 0)
	{
		pOutMeshData->ppChildMeshData = new MeshData*[childMeshDataCount];

		int currentChildCount = 0;
		for (int i = 0; i < childMeshDataCount; i++)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			pAttr = pChildNode->GetNodeAttribute();
			type = pAttr->GetAttributeType();
			if (type == FbxNodeAttribute::eMesh)
			{
				pOutMeshData->ppChildMeshData[currentChildCount] = GetMeshData(pChildNode);
				currentChildCount++;
			}
		}
	}
	else
	{
		pOutMeshData->ppChildMeshData = nullptr;
	}

	return pOutMeshData;
}

ID3D11Buffer* MeshImporter::GetVertexBuffer(FbxMesh* pMesh)
{
	if (pMesh == nullptr) { return nullptr; }

	const int VERTEX_COUNT = pMesh->GetControlPointsCount();
	const int POLYGON_COUNT = pMesh->GetPolygonCount();
	const int UV_COUNT = pMesh->GetTextureUVCount();

	// 頂点バッファーのサイズ指定
	VERTEX_DATA* pVertexData = 0;
	if (UV_COUNT > VERTEX_COUNT)
	{
		// UVベース
		pVertexData = new VERTEX_DATA[UV_COUNT];
	}
	else
	{
		// 座標ベース
		pVertexData = new VERTEX_DATA[VERTEX_COUNT];
	}

	// ポリゴンごとにデータ取得
	for (int i = 0; i < POLYGON_COUNT; i++)
	{
		int index[3] = { 0,0,0 };

		// インデックス
		if (UV_COUNT > VERTEX_COUNT)
		{
			// UVベース
			index[0] = pMesh->GetTextureUVIndex(i, 0);
			index[1] = pMesh->GetTextureUVIndex(i, 1);
			index[2] = pMesh->GetTextureUVIndex(i, 2);
		}
		else
		{
			// 座標ベース
			int startIndex = pMesh->GetPolygonVertexIndex(i);
			int* pIndex = pMesh->GetPolygonVertices();

			index[0] = pIndex[startIndex];
			index[1] = pIndex[startIndex + 1];
			index[2] = pIndex[startIndex + 2];
		}

		// 頂点
		FbxVector4* vertex = 0;
		vertex = pMesh->GetControlPoints();
		int vIndex = pMesh->GetPolygonVertex(i, 0);
		pVertexData[index[0]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[0]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[0]].pos.z = vertex[vIndex].mData[2];

		vIndex = pMesh->GetPolygonVertex(i, 1);
		pVertexData[index[1]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[1]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[1]].pos.z = vertex[vIndex].mData[2];

		vIndex = pMesh->GetPolygonVertex(i, 2);
		pVertexData[index[2]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[2]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[2]].pos.z = vertex[vIndex].mData[2];

		// 法線
		FbxVector4 normal;
		pMesh->GetPolygonVertexNormal(i, 0, normal);
		pVertexData[index[0]].normal.x = normal[0];
		pVertexData[index[0]].normal.y = normal[1];
		pVertexData[index[0]].normal.z = normal[2];

		pMesh->GetPolygonVertexNormal(i, 1, normal);
		pVertexData[index[1]].normal.x = normal[0];
		pVertexData[index[1]].normal.y = normal[1];
		pVertexData[index[1]].normal.z = normal[2];

		pMesh->GetPolygonVertexNormal(i, 2, normal);
		pVertexData[index[2]].normal.x = normal[0];
		pVertexData[index[2]].normal.y = normal[1];
		pVertexData[index[2]].normal.z = normal[2];

		// UVモードのチェック
		auto element = pMesh->GetElementUV(0);
		auto mappingMode = element->GetMappingMode();
		auto referenceMode = element->GetReferenceMode();
		if (mappingMode != FbxLayerElement::EMappingMode::eByPolygonVertex) { IS_CHECK_ERROR(false, "このFBXは対応していないマッピングモードで作成されています"); return nullptr; }
		if (referenceMode != FbxLayerElement::EReferenceMode::eIndexToDirect) { IS_CHECK_ERROR(false, "このFBXは対応していないレファレンスモードで作成されています"); return nullptr; }

		// UV
		FbxLayerElementUV* pUV = pMesh->GetLayer(0)->GetUVs();

		int UVIndex = pMesh->GetTextureUVIndex(i, 0);
		FbxVector2 uvPos = pUV->GetDirectArray().GetAt(UVIndex);
		pVertexData[index[0]].texture.x = uvPos.mData[0];
		pVertexData[index[0]].texture.y = 1.0f - uvPos.mData[1];

		UVIndex = pMesh->GetTextureUVIndex(i, 1);
		uvPos = pUV->GetDirectArray().GetAt(UVIndex);
		pVertexData[index[1]].texture.x = uvPos.mData[0];
		pVertexData[index[1]].texture.y = 1.0f - uvPos.mData[1];

		UVIndex = pMesh->GetTextureUVIndex(i, 2);
		uvPos = pUV->GetDirectArray().GetAt(UVIndex);
		pVertexData[index[2]].texture.x = uvPos.mData[0];
		pVertexData[index[2]].texture.y = 1.0f - uvPos.mData[1];
	}

#if false
	// MappingMode が eBy_CONTROL_POINTの場合
	FbxLayerElementUV* pUV = pMesh->GetLayer(0)->GetUVs();
	for (int k = 0; k < UV_COUNT; k++)
	{
		FbxVector2 uvPos = pUV->GetDirectArray().GetAt(k);
		pVertexData[k].texture.x = uvPos.mData[0];
		pVertexData[k].texture.y = uvPos.mData[1];
	}
#endif


	// 頂点バッファーを作成
	ID3D11Buffer* pVertexBuffer = nullptr;

	const int COUNT = UV_COUNT > VERTEX_COUNT ? UV_COUNT : VERTEX_COUNT;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_DATA) * COUNT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertexData;
	HRESULT hr = m_pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	CHECK_ERROR(hr, "VertexBufferCreateError");
	
	delete[] pVertexData;
	return pVertexBuffer;
}

MaterialData* MeshImporter::GetMaterials(FbxNode* pNode)
{
	int MATERIAL_COUNT = pNode->GetMaterialCount();

	MaterialData* pMaterialData = new MaterialData[MATERIAL_COUNT];
	ID3D11Buffer** ppIndexBuffer = new ID3D11Buffer*[MATERIAL_COUNT];

	for (int i = 0; i < MATERIAL_COUNT; i++)
	{
#if true
		// Phongモデル
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		// ambient
		FbxDouble3 ambient = pPhong->Ambient;
		pMaterialData[i].ambient.x = (float)ambient.mData[0];
		pMaterialData[i].ambient.y = (float)ambient.mData[1];
		pMaterialData[i].ambient.z = (float)ambient.mData[2];

		// diffuse
		FbxDouble3 diffuse = pPhong->Diffuse;
		pMaterialData[i].diffuse.x = (float)diffuse.mData[0];
		pMaterialData[i].diffuse.y = (float)diffuse.mData[1];
		pMaterialData[i].diffuse.z = (float)diffuse.mData[2];

		// emissive

		// bump

		// transparency

		// specular
		FbxDouble3 specular = pPhong->Specular;
		pMaterialData[i].specular.x = (float)specular.mData[0];
		pMaterialData[i].specular.y = (float)specular.mData[1];
		pMaterialData[i].specular.z = (float)specular.mData[2];

#endif
		/*
		・KFbxSurfaceMaterial::ShadingModel
		・KFbxSurfaceMaterial::MultiLayer
		・KFbxSurfaceMaterial::Emissive
		・KFbxSurfaceMaterial::EmissiveFactor
		・KFbxSurfaceMaterial::Ambient
		・KFbxSurfaceMaterial::AmbientFactor
		・KFbxSurfaceMaterial::Diffuse
		・KFbxSurfaceMaterial::DiffuseFactor
		・KFbxSurfaceMaterial::Specular
		・KFbxSurfaceMaterial::SpecularFactor
		・KFbxSurfaceMaterial::Shininess
		・KFbxSurfaceMaterial::Bump
		・KFbxSurfaceMaterial::NormalMap
		・KFbxSurfaceMaterial::TransparentColor
		・KFbxSurfaceMaterial::TransparencyFactor
		・KFbxSurfaceMaterial::Reflection
		・KFbxSurfaceMaterial::ReflectionFactor
		*/

		HRESULT hr;

		//テクスチャー（ディフューズテクスチャーのみ）
#if false
		FbxProperty lProperty;
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		FbxTexture* texture = FbxCast<FbxTexture>(lProperty.GetSrcObject(FbxTexture::ClassId, 0));

		if (texture)
		{
			strcpy_s(pMaterialData[i].diffuseTextureName, texture->GetName());
			//HRESULT hr;
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, pMaterialData[i].diffuseTextureName, nullptr, nullptr, &pMaterialData[i].pDiffuseTexture, nullptr)))
			{
				char ext[11];
				_splitpath(pMaterialData[i].diffuseTextureName, 0, 0, pMaterialData[i].diffuseTextureName, ext);
				strcat_s(pMaterialData[i].diffuseTextureName, ext);
				HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(m_pDevice, pMaterialData[i].diffuseTextureName, nullptr, nullptr, &pMaterialData[i].pDiffuseTexture, nullptr);

				if (FAILED(hr))
				{
					CHAR* pTextureName = "Texture2.bmp";
					strcpy_s(pMaterialData[i].diffuseTextureName, pTextureName);
					hr = D3DX11CreateShaderResourceViewFromFileA(m_pDevice, pMaterialData[i].diffuseTextureName, nullptr, nullptr, &pMaterialData[i].pDiffuseTexture, nullptr);

					CHECK_ERROR(hr, "テクスチャー読み込み失敗");
				}
			}
		}
#else
		CHAR* pTextureName = "table_base.bmp";
		strcpy_s(pMaterialData[i].diffuseTextureName, pTextureName);
		hr = D3DX11CreateShaderResourceViewFromFileA(m_pDevice, pMaterialData[i].diffuseTextureName, nullptr, nullptr, &pMaterialData[i].pDiffuseTexture, nullptr);

		CHECK_ERROR(hr, "テクスチャー読み込み失敗");
#endif

		FbxMesh* pMesh = pNode->GetMesh();
		const int POLYGON_COUNT = pMesh->GetPolygonCount();

		int count = 0;
		for (int k = 0; k < POLYGON_COUNT; k++)
		{
			FbxLayerElementMaterial* mat = pMesh->GetLayer(0)->GetMaterials();
			int matId = mat->GetIndexArray().GetAt(k);
			if (matId == i)
			{
				count += 3;
			}
		}

		pMaterialData[i].TriangleCount = count / 3;
	}
	return pMaterialData;
}

ID3D11Buffer ** MeshImporter::GetIndexBuffer(FbxNode * pNode)
{
	DWORD MATERIAL_COUNT = pNode->GetMaterialCount();
	ID3D11Buffer** ppIndexBuffer = new ID3D11Buffer*[MATERIAL_COUNT];

	for (int i = 0; i < MATERIAL_COUNT; i++)
	{
		// インデックスバッファーの作成
		FbxMesh* pMesh = pNode->GetMesh();
		const int POLYGON_COUNT = pMesh->GetPolygonCount();
		int* pIndex = new int[POLYGON_COUNT * 3];

		const int VERTEX_COUNT = pMesh->GetControlPointsCount();
		const int UV_COUNT = pMesh->GetTextureUVCount();

		int count = 0;
		for (int k = 0; k < POLYGON_COUNT; k++)
		{
			FbxLayerElementMaterial* mat = pMesh->GetLayer(0)->GetMaterials();
			int matId = mat->GetIndexArray().GetAt(k);
			if (matId == i)
			{
				if (UV_COUNT > VERTEX_COUNT)
				{
					// UVベースのインデックス
					pIndex[count] = pMesh->GetTextureUVIndex(k, 0);
					pIndex[count + 1] = pMesh->GetTextureUVIndex(k, 1);
					pIndex[count + 2] = pMesh->GetTextureUVIndex(k, 2);
				}
				else
				{
					// 座標頂点ベースのインデックス
					pIndex[count] = pMesh->GetPolygonVertex(k, 0);
					pIndex[count + 1] = pMesh->GetPolygonVertex(k, 1);
					pIndex[count + 2] = pMesh->GetPolygonVertex(k, 2);
				}

				count += 3;
			}
		}

		//

		// 
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pIndex;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr = m_pDevice->CreateBuffer(&bd, &InitData, &ppIndexBuffer[i]);
		CHECK_ERROR(hr, "");

		delete pIndex;
	}

	return ppIndexBuffer;
}


HRESULT MeshImporter::SetMaterialTexture(ID3D11Device * pDevice, CHAR * pDiffuseTextureName, MaterialData * pTargetMaterial)
{
	if (pTargetMaterial == nullptr) { IS_CHECK_ERROR(false, "テクスチャーセット先のマテリアルが取得できていません"); return E_FAIL; }

	HRESULT hr = E_FAIL;
	if (pDiffuseTextureName[0] != NULL)
	{
		strcpy_s(pTargetMaterial->diffuseTextureName, pDiffuseTextureName);
		hr = D3DX11CreateShaderResourceViewFromFileA(pDevice, pTargetMaterial->diffuseTextureName, nullptr, nullptr, &pTargetMaterial->pDiffuseTexture, nullptr);
		CHECK_ERROR(hr, "Diffuseテクスチャーインポート失敗");
	}
	/*if(pNormalTextureName[0] != NULL){}*/
	return hr;
}

int* GetIndexData(MeshData* pMeshData)
{
	// インデックスバッファーの作成
	const int POLYGON_COUNT = pMeshData->m_polygonCount;
	const int UV_COUNT = pMeshData->m_uvCount;
	const int VERTEX_COUNT = pMeshData->m_vertexCount;
	int vCount = UV_COUNT > VERTEX_COUNT ? UV_COUNT : VERTEX_COUNT;
	pMeshData->vertexCountToCollider = vCount;

	int* pIndex = new int[POLYGON_COUNT * 3];

	int count = 0;
	for (int k = 0; k < POLYGON_COUNT; k++)
	{
		FbxLayerElementMaterial* mat = pMeshData->pFBXMesh->GetLayer(0)->GetMaterials();
		int matId = mat->GetIndexArray().GetAt(k);
		if (matId == 0)// Zero Only
		{
			if (UV_COUNT > VERTEX_COUNT)
			{
				// UVベースのインデックス
				pIndex[count] = pMeshData->pFBXMesh->GetTextureUVIndex(k, 0);
				pIndex[count + 1] = pMeshData->pFBXMesh->GetTextureUVIndex(k, 1);
				pIndex[count + 2] = pMeshData->pFBXMesh->GetTextureUVIndex(k, 2);
			}
			else
			{
				// 座標頂点ベースのインデックス
				pIndex[count] = pMeshData->pFBXMesh->GetPolygonVertex(k, 0);
				pIndex[count + 1] = pMeshData->pFBXMesh->GetPolygonVertex(k, 1);
				pIndex[count + 2] = pMeshData->pFBXMesh->GetPolygonVertex(k, 2);
			}

			count += 3;
		}
	}
	return pIndex;
}

/*
D3DXVECTOR3* GetVertexDataToCollider(MeshData* pMeshData)
{
	const int POLYGON_COUNT = pMeshData->m_polygonCount;
	const int UV_COUNT = pMeshData->m_uvCount;
	const int VERTEX_COUNT = pMeshData->m_vertexCount;
	int vCount = UV_COUNT > VERTEX_COUNT ? UV_COUNT : VERTEX_COUNT;
	pMeshData->vertexCountToCollider = vCount;

	D3DXVECTOR3* pVertex = new D3DXVECTOR3[vCount];

	// ポリゴンごとにデータ取得
	for (int i = 0; i < POLYGON_COUNT; i++)
	{
		int index[3] = { 0,0,0 };

		// インデックス
		if (UV_COUNT > VERTEX_COUNT)
		{
			// UVベース
			index[0] = pMesh->GetTextureUVIndex(i, 0);
			index[1] = pMesh->GetTextureUVIndex(i, 1);
			index[2] = pMesh->GetTextureUVIndex(i, 2);
		}
		else
		{
			// 座標ベース
			int startIndex = pMesh->GetPolygonVertexIndex(i);
			int* pIndex = pMesh->GetPolygonVertices();

			index[0] = pIndex[startIndex];
			index[1] = pIndex[startIndex + 1];
			index[2] = pIndex[startIndex + 2];
		}

		// 頂点
		FbxVector4* vertex = 0;
		vertex = pMesh->GetControlPoints();
		int vIndex = pMesh->GetPolygonVertex(i, 0);
		pVertexData[index[0]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[0]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[0]].pos.z = vertex[vIndex].mData[2];

		vIndex = pMesh->GetPolygonVertex(i, 1);
		pVertexData[index[1]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[1]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[1]].pos.z = vertex[vIndex].mData[2];

		vIndex = pMesh->GetPolygonVertex(i, 2);
		pVertexData[index[2]].pos.x = vertex[vIndex].mData[0] * -1;
		pVertexData[index[2]].pos.y = vertex[vIndex].mData[1];
		pVertexData[index[2]].pos.z = vertex[vIndex].mData[2];
}
*/