#include "DeviceManager.h"

HRESULT DeviceManager::CreateDeviceAndSwapChian(const HWND hWnd, SIZE windowSize)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = windowSize.cy;
	sd.BufferDesc.Height = windowSize.cx;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	m_SwapChainDesc = sd;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&pFeatureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd, &m_pSwapChain,
		&m_pDevice,
		pFeatureLevel,
		&m_pDeviceContext);
	CHECK_ERROR(hr, "CreateDeviceError");

	return E_NOTIMPL;
}

HRESULT DeviceManager::InitBackBuffer(SIZE windowSize)
{
	// RTVの作成
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRTV);
	pBackBuffer->Release();

	// DSVの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = windowSize.cy;
	descDepth.Height = windowSize.cx;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_depthStencilDesc = descDepth;
	m_pDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);

	m_pDevice->CreateDepthStencilView(m_pDepthStencil, nullptr, &m_pDSV);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = windowSize.cy;
	vp.Height = windowSize.cy;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	return S_OK;
}

HRESULT DeviceManager::CompileAndCreateShader(LPCSTR fileName, LPCSTR funcName, LPCSTR profile, void **ppShader)
{
	HRESULT hr = E_FAIL;

	ID3DBlob* pBlob = nullptr;
	ID3DBlob* pErrors = nullptr;

	// コンパイル
	D3DX11CompileFromFileA(
		fileName,
		nullptr,
		nullptr,
		funcName,
		profile,
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_DEBUG,
		0,
		nullptr,
		&pBlob,
		&pErrors,
		&hr
	);
	//if (FAILED(hr)) { CHECK_ERROR(hr, (char*)pErrors->GetBufferPointer()); }
	CHECK_ERROR(hr, "シェーダーのコンパイルエラー");
	SAFE_RELEASE(pErrors);

	// profileから先頭２文字取得
	char shaderTarget[3] = { 0 };
	strncpy_s(shaderTarget, 3, profile, 2);

	// シェーダーオブジェクトの作成
	if (!strcmp(shaderTarget, "vs"))//Vertex Shader
	{
		hr = m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11VertexShader**)ppShader);
		CHECK_ERROR(hr, "");
		// 頂点レイアウト定設定
		hr = CreateInputLayoutObject(pBlob);
		CHECK_ERROR(hr, "");
	}
	if (!strcmp(shaderTarget, "ps"))//Pixel Shader
	{
		hr = m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11PixelShader**)ppShader);
		CHECK_ERROR(hr, "");
	}
	if (!strcmp(shaderTarget, "gs"))//Geometry Shader
	{
		hr = m_pDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11GeometryShader**)ppShader);
		CHECK_ERROR(hr, "");
	}
	if (!strcmp(shaderTarget, "hs"))//Hull Shader
	{
		hr = m_pDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11HullShader**)ppShader);
		CHECK_ERROR(hr, "");
	}
	if (!strcmp(shaderTarget, "ds"))//Domain Shader
	{
		hr = m_pDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11DomainShader**)ppShader);
		CHECK_ERROR(hr, "");
	}
	if (!strcmp(shaderTarget, "cs"))//Compute Shader
	{
		hr = m_pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, (ID3D11ComputeShader**)ppShader);
		CHECK_ERROR(hr, "");
	}
	SAFE_RELEASE(pBlob);

	return S_OK;
}

HRESULT DeviceManager::CreateInputLayoutObject(ID3DBlob *pBlobVS)
{
	HRESULT hr = E_FAIL;

	// 作成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int layoutLength = sizeof(layout) / sizeof(layout[0]);
	hr = m_pDevice->CreateInputLayout(layout, layoutLength, pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &m_pVertexLayout);
	CHECK_ERROR(hr, "");

	// セット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	return S_OK;
}

void DeviceManager::InitShader()
{
	HRESULT hr = E_FAIL;

	// 頂点シェーダーを作成
	CompileAndCreateShader("Shader.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader);

	// ピクセルシェーダーを作成
	CompileAndCreateShader("Shader.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* pIr = nullptr;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
	// コンスタントバッファー0作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	hr = m_pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer0);
	CHECK_ERROR(hr, "バッファ作成エラー");

	// コンスタントバッファー1作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ShaderGlobal01);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = m_pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer1);
	CHECK_ERROR(hr, "バッファー作成エラー");

	// サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return;
}

HRESULT DeviceManager::InitD3D(HWND hWnd, SIZE windowSize)
{
	CreateDeviceAndSwapChian(hWnd, windowSize);
	InitBackBuffer(windowSize);
	InitShader();
	return S_OK;
}
void DeviceManager::ClearRender()
{
	float ClearColor[4] = { 0,0,0.4,1 };
	m_pDeviceContext->ClearRenderTargetView(m_pRTV, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeviceManager::RenderSetUp(SIZE windowSize)
{
	ClearRender();

	//ビュー行列
	//D3DXVECTOR3 Eye(0.0f, 0.0f, -3500.5f);
	D3DXVECTOR3 Eye(0.0f, 0.0f, -3000.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_view, &Eye, &At, &Up);

	//プロジェクション行列
#if true
	float aspectRatio = (float)m_depthStencilDesc.Width / m_depthStencilDesc.Height;
	D3DXMatrixPerspectiveFovLH(&m_projection, D3DX_PI / 4, aspectRatio, 1.0f, 100000.0f);
#else
	D3DXMatrixPerspectiveFovLH(&m_projection, (float)D3DX_PI / 4, (float)width / (float)height, 0.1f, 1100.0f);
#endif

	// shaderにデータを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_GLOBAL0 sg;
		sg.lightDir = D3DXVECTOR4(g_lightDir.x, g_lightDir.y, g_lightDir.z, 0.0f);
		sg.eye = D3DXVECTOR4(Eye.x, Eye.y, Eye.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL0));
		m_pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
}

void DeviceManager::UpdateScreen()
{
	m_pSwapChain->Present(0, 0);
}

void DeviceManager::ResizeRenderWindow(LPARAM lParam)
{
	if (m_pDevice)
	{
		// RTVとDSVを開放
		ID3D11RenderTargetView *nullptrRTV = nullptr;
		m_pDeviceContext->OMSetRenderTargets(1, &nullptrRTV, nullptr);
		if (m_pRTV)
		{
			m_pRTV->Release();
			m_pRTV = nullptr;
		}
		if (m_pDSV)
		{
			m_pDSV->Release();
			m_pDSV = nullptr;
		}

		if (m_pSwapChain)
		{
			// スワップチェーン再設定
			m_SwapChainDesc.BufferDesc.Width = LOWORD(lParam);
			m_SwapChainDesc.BufferDesc.Height = HIWORD(lParam);
			m_pSwapChain->ResizeBuffers(
				m_SwapChainDesc.BufferCount,
				m_SwapChainDesc.BufferDesc.Width,
				m_SwapChainDesc.BufferDesc.Height,
				m_SwapChainDesc.BufferDesc.Format,
				m_SwapChainDesc.Flags
			);

			// RTVとDSVを再設定
			ID3D11Texture2D *backBuffer = nullptr, *dsBuffer = nullptr;
			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
			m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRTV);
			backBuffer->Release();
			m_depthStencilDesc.Width = m_SwapChainDesc.BufferDesc.Width;
			m_depthStencilDesc.Height = m_SwapChainDesc.BufferDesc.Height;
			m_pDevice->CreateTexture2D(&m_depthStencilDesc, nullptr, &dsBuffer);
			m_pDevice->CreateDepthStencilView(dsBuffer, nullptr, &m_pDSV);
			dsBuffer->Release();
			m_pDeviceContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);

			// ビューポート設定
			D3D11_VIEWPORT vp;
			vp.Width = (float)m_SwapChainDesc.BufferDesc.Width;
			vp.Height = (float)m_SwapChainDesc.BufferDesc.Height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			m_pDeviceContext->RSSetViewports(1, &vp);
		}
	}
}