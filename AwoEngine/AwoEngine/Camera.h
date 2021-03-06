#pragma once
#include "DeviceManager.h"
#include "Input.h"
#include <AntTweakBar.h>

struct ShaderGlobal_Camera
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
	// lookVector
	// sideVector
	// upVector
	// nearPlane
	// farPlane
};

class Camera
{
public:
	Camera()
		: m_eye(0.0f, 300.0f, -250.0f)
		, m_lookAt(0.0f, 0.0f, 0.0f)
		, m_up(0.0f, 1.0f, 0.0f)
	{};
	~Camera() {};

#if false
	void CameraMove(Input* pInput)
	{
		if (pInput->GetKeyDown(DIK_W)) {/**/ };
		if (pInput->GetKeyDown(DIK_S)) {/**/ };
		if (pInput->GetKeyDown(DIK_A)) {/**/ };
		if (pInput->GetKeyDown(DIK_D)) {/**/ };
	}
#endif	

	void ShowCameraEditWindow()
	{
		m_pBar = TwNewBar("CameraSetting");
		TwDefine("CameraSetting position='200 40' size='200 400'");
		TwAddVarRW(m_pBar, "Eye", TW_TYPE_DIR3F, &m_eye, "");
		TwDefine(" CameraSetting/Eye opened=true ");
		TwAddVarRW(m_pBar, "LookAt", TW_TYPE_DIR3F, &m_lookAt, "");
		TwDefine(" CameraSetting/LookAt opened=true ");
		TwAddVarRW(m_pBar, "UP", TW_TYPE_DIR3F, &m_up, "");
		TwDefine(" CameraSetting/UP opened=true ");
	}
	void CameraSetting(const DeviceManager* pDeviceManager)
	{
		// shaderにデータを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(pDeviceManager->m_pDeviceContext->Map(pDeviceManager->m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_GLOBAL0 sg;
			sg.lightDir = D3DXVECTOR4(g_lightDir.x, g_lightDir.y, g_lightDir.z, 0.0f);
			sg.eye = D3DXVECTOR4(m_eye.x, m_eye.y, m_eye.z, 0);
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL0));
			pDeviceManager->m_pDeviceContext->Unmap(pDeviceManager->m_pConstantBuffer0, 0);
		}

		pDeviceManager->m_pDeviceContext->VSSetConstantBuffers(0, 1, &pDeviceManager->m_pConstantBuffer0);
		pDeviceManager->m_pDeviceContext->PSSetConstantBuffers(0, 1, &pDeviceManager->m_pConstantBuffer0);
	}

	D3DXMATRIX GetView() const 
	{ 
		D3DXMATRIX view; 
		D3DXMatrixLookAtLH(&view, &m_eye, &m_lookAt, &m_up);
		return view; 
	}
	D3DXMATRIX GetProjection(const D3D11_TEXTURE2D_DESC depthStencilDesc) const 
	{
		D3DXMATRIX projection;
#if true
		float aspectRatio = (float)depthStencilDesc.Width / depthStencilDesc.Height;
		D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 4, aspectRatio, 1.0f, 100000.0f);
#else
		D3DXMatrixPerspectiveFovLH(&projection, (float)D3DX_PI / 4, (float)width / (float)height, 0.1f, 1100.0f);
#endif
		return projection;
	}

private:
	D3DXVECTOR3 m_eye;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_up;

	TwBar* m_pBar;
};

