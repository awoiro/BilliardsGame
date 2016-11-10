#pragma once
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <fbxsdk.h>
#include <string.h>
#include "ErrorManager.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

struct ShaderGlobal01
{
	D3DXMATRIX world;
	D3DXMATRIX WVP;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
};


//頂点構造体
struct VERTEX_DATA
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
};

// マテリアル構造体
struct MaterialData
{
	D3DXVECTOR4 ambient = D3DXVECTOR4(0,0,0,0);
	D3DXVECTOR4 diffuse = D3DXVECTOR4(1,0,0,0);
	D3DXVECTOR4 specular = D3DXVECTOR4(1,1,1,1);

	CHAR diffuseTextureName[100];
	ID3D11ShaderResourceView* pDiffuseTexture;

	DWORD TriangleCount;

	MaterialData()
		: pDiffuseTexture(nullptr)
	{
		ZeroMemory(this, sizeof(MaterialData));
	}
	~MaterialData()
	{
		SAFE_RELEASE(pDiffuseTexture);
	}
};

// FBXのMeshデータ構造体
struct MeshData
{

	MeshData()
		: pVertexBuffer(nullptr)
		, pMaterials(nullptr)
		, ppChildMeshData(nullptr)
		, pFBXMesh(nullptr)
	{
		ZeroMemory(this, sizeof(MeshData));
		D3DXMatrixIdentity(&m_myOrientation);
	}

	~MeshData()
	{
		SAFE_RELEASE(pVertexBuffer);
		delete[] ppChildMeshData;
		delete pMaterials;
		delete pVertexBuffer;

		for (int i = 0; i < m_materialCount; i++)
		{
			SAFE_RELEASE(ppIndexBuffer[i]);
		}
		delete[] ppIndexBuffer;
	}

	MeshData** ppChildMeshData;

	D3DXMATRIX m_parentOrientation;
	D3DXMATRIX m_myOrientation;
	D3DXMATRIX m_finalWorld;

	ID3D11Buffer* pVertexBuffer;
	MaterialData* pMaterials;
	ID3D11Buffer** ppIndexBuffer;

	FbxMesh* pFBXMesh;

	int m_childCount;
	int m_materialCount;
	int m_vertexCount;
	int m_uvCount;
	int m_polygonCount;

};

class MeshImporter
{
public:
	MeshImporter()
	{
		ZeroMemory(this, sizeof(MeshImporter));
		InitFBXImporter();
	};
	~MeshImporter()
	{
		DestroyFBXImporter();
	};

	FbxManager *m_pFBXManager;
	FbxImporter* m_pImporter;
	FbxScene* m_pFBXScene;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11Buffer* m_pConstantBuffer;

	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;

	MeshData* CreateMeshData(CHAR* pFileName);
	void RenderMesh(D3DXMATRIX* pWorld, MeshData* pMeshData);
	HRESULT SetMaterialTexture(ID3D11Device* pDevice, CHAR* pDiffuseTextureName,/*CHAR* pNormalTextureName,*/ MaterialData* pTargetMaterial);

protected:
	HRESULT InitFBXImporter();
	void DestroyFBXImporter();
	FbxScene* GetFBXData(CHAR* pFileName);
	MeshData* GetMeshData(FbxNode* pNode);
	ID3D11Buffer* GetVertexBuffer(FbxMesh* pMesh);
	MaterialData* GetMaterials(FbxNode* pNode);
	ID3D11Buffer** GetIndexBuffer(FbxNode* pNode);
};
