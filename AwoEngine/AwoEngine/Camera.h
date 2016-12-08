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
		TwAddVarRW(m_pBar, "Eye", TW_TYPE_DIR3F, &m_eye, "");
		TwAddVarRW(m_pBar, "LookAt", TW_TYPE_DIR3F, &m_lookAt, "");
		TwAddVarRW(m_pBar, "UP", TW_TYPE_DIR3F, &m_up, "");
		
	}

	void CameraSetting(DeviceManager* pDeviceManager)
	{
		//m_view = GetView();
		//m_Projection = GetProjection(pDeviceManager->m_depthStencilDesc);

		// shader‚Éƒf[ƒ^‚ð“n‚·
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

	D3DXVECTOR3 m_eye;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_up;

	TwBar* m_pBar;
	
	//D3DXMATRIX m_view;
	//D3DXMATRIX m_Projection;


	D3DXMATRIX GetView()
	{ 
		D3DXMATRIX view; 
		D3DXMatrixLookAtLH(&view, &m_eye, &m_lookAt, &m_up);
		return view; 
	}
	D3DXMATRIX GetProjection(D3D11_TEXTURE2D_DESC depthStencilDesc)
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
};

