#pragma once

#include "DeviceManager.h"

#define DIMENSION 12.0f
#define WDIMENTION 10.0f
#define TEX_DIMENTION 128.0f

struct TextVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texPos;
};

struct TextConstantBuffer
{
	D3DXMATRIX WVP;
	D3DXVECTOR4 color;
	D3DXVECTOR4 alpha;
};

class Text
{
public:
	Text();
	~Text();

	HRESULT Init(DeviceManager* pDeviceManger, float width, float height, float size, D3DXCOLOR color);
	void Render(char* text, float x, float y);
	void RenderFont(int fontIndex, float x, float y);

private:
	float m_kerning[100];
	float m_scale;//25pixelを基準 25pixel=1.0f
	float m_alpha;
	D3DXCOLOR m_color;

	// シェーダー関係
	ID3D11Buffer* m_pVertexBuffer[100];
	ID3D11SamplerState* m_pSamplerState;
	ID3D11ShaderResourceView* m_pFontTexture;

	ID3D11SamplerState* m_pSampleLinear;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11BlendState* m_pBlendState;
};
