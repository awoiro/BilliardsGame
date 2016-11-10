#include "Text.h"

Text::Text()
{}

Text::~Text()
{}

HRESULT Text::Init(DeviceManager * pDeviceManger, float width, float height, float size, D3DXCOLOR color)
{
	HRESULT hr = E_FAIL;

	m_alpha = color.a;
	m_color = color;
	m_scale = size / 25;

	for (int i = 0; i < 100; i++)
	{
		m_kerning[i] = 10;
	}

	// フォントごとのクアッド作成

	float left = 0, top = 0, right = 0, bottom = 0;
	int count = 0;
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			left = k * WDIMENTION;
			top = i * DIMENSION;
			right = left + m_kerning[count];
			bottom = top + DIMENSION;
			left /= TEX_DIMENTION;
			top /= TEX_DIMENTION;
			right /= TEX_DIMENTION;
			bottom /= TEX_DIMENTION;

			TextVertex vertices[] =
			{
				D3DXVECTOR3(0,DIMENSION,0)			, D3DXVECTOR2(left,bottom)	,
				D3DXVECTOR3(0,0,0)					, D3DXVECTOR2(left,top)		,
				D3DXVECTOR3(WDIMENTION,DIMENSION,0)	, D3DXVECTOR2(right,bottom) ,
				D3DXVECTOR3(WDIMENTION,0,0)			, D3DXVECTOR2(right,top)
			};

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(TextVertex) * 4;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = vertices;
			hr = pDeviceManger->m_pDevice->CreateBuffer(&bufferDesc, &initData, &m_pVertexBuffer[100]);
			if (FAILED(hr))
			{
				return E_FAIL;
			}

			count++;
		}
	}

	// サンプラー作成
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDeviceManger->m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);

	// フォントテクチャー作成
	hr = D3DX11CreateShaderResourceViewFromFile(pDeviceManger->m_pDevice, "ascii.png", nullptr, nullptr, &m_pFontTexture, nullptr);
	CHECK_ERROR(hr, "フォントテクスチャー作成失敗");
	if (FAILED(hr)) { return E_FAIL; }

	//

	//

	return S_OK;
}