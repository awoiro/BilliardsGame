#pragma once

#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "MeshData.h"
#include "ErrorManager.h"

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#define SAMPLE_COUNT 8
#define SAMPLE_QUALITY 0.9

static D3DXVECTOR3 g_lightDir(-1, 1, -1);

struct SHADER_GLOBAL0
{
	D3DXVECTOR4 lightDir;
	D3DXVECTOR4 eye;
};

class DeviceManager
{
public:
	DeviceManager()
		: m_pDevice(nullptr)
		, m_pSwapChain(nullptr)
		, m_pRTV(nullptr)
		, m_pDSV(nullptr)
		, m_pVertexLayout(nullptr)
		, m_pDepthStencil(nullptr)
		, m_pDeviceContext(nullptr)
		, m_pVertexShader(nullptr)
		, m_pPixelShader(nullptr)
		, m_pConstantBuffer0(nullptr)
		, m_pConstantBuffer1(nullptr)
		, m_pSampleLinear(nullptr)
	{};
	~DeviceManager()
	{
		//delete g_pMesh;
		SAFE_RELEASE(m_pSampleLinear);
		SAFE_RELEASE(m_pDSV);
		SAFE_RELEASE(m_pDepthStencil);
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pConstantBuffer0);
		SAFE_RELEASE(m_pConstantBuffer1);
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pRTV);
		SAFE_RELEASE(m_pVertexLayout);
		SAFE_RELEASE(m_pDeviceContext);
		SAFE_RELEASE(m_pDevice);
	}

	HRESULT InitD3D(HWND hWnd, SIZE windowSize);
	HRESULT CreateDeviceAndSwapChian(const HWND hWnd, SIZE windowSize);
	HRESULT InitBackBuffer(SIZE windowSize);
	HRESULT CompileAndCreateShader(LPCSTR fileName, LPCSTR funcName, LPCSTR profile, void **ppShader);
	HRESULT CreateInputLayoutObject(ID3DBlob *pBlobVS);
	void InitShader();
	void ClearRender();

	void RenderSetUp(SIZE windowSize);
	void UpdateScreen();
	void ResizeRenderWindow(LPARAM lParam);
	void ResizeRenderWindow(SIZE windowSize);

	ID3D11Device* m_pDevice;
	IDXGISwapChain*			m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;
	D3D11_TEXTURE2D_DESC	m_depthStencilDesc;
	ID3D11InputLayout*      m_pVertexLayout;
	ID3D11Texture2D*		m_pDepthStencil;

	ID3D11DeviceContext	 *m_pDeviceContext = nullptr;
	ID3D11VertexShader *m_pVertexShader = nullptr;
	ID3D11PixelShader *m_pPixelShader = nullptr;

	ID3D11Buffer* m_pConstantBuffer0 = nullptr;
	ID3D11Buffer* m_pConstantBuffer1 = nullptr;

	ID3D11SamplerState* m_pSampleLinear = nullptr;
};
