#include "Application.h"

/*
void RenderTwText()
{
	TwBeginText(2, 0, 0, 0);
	const unsigned int color = 0xffffc0ff;
	char msg[256];

	//double averageTime = 60.0f;//g_DeviceManager->GetAverageFrameTime();
	//double fps = (averageTime > 0) ? 1.0 / averageTime : 0.0;

	//sprintf_s(msg, "%.1f FPS", fps);
	sprintf_s(msg, "ssss");
	TwAddTextLine(msg, color, 0);

	TwEndText();
}
*/


void RenderTwText(int x, int y, char* msg)
{
	TwBeginText(x, y, 0, 0);

	const unsigned int color = 0xffffc0ff;
	TwAddTextLine(msg, color, 0);

	TwEndText();
}

void Application::Init(HWND hWnd, SIZE windowSize)
{
	m_pDeviceManager = new DeviceManager;
	m_pDeviceManager->InitD3D(hWnd, windowSize);

	TwInit(TW_DIRECT3D11, m_pDeviceManager->m_pDevice);
	TwWindowSize(windowSize.cy, windowSize.cx);
	m_pBar = TwNewBar("ModelDetail");
	TwAddVarRW(m_pBar, "Position", TW_TYPE_DIR3F, &m_position, "");
	TwAddVarRW(m_pBar, "Angle", TW_TYPE_QUAT4F, &m_angle, "");
	TwAddVarRW(m_pBar, "AutoRotation", TW_TYPE_BOOLCPP, &m_isAutoRotation, "");
	TwAddVarRW(m_pBar, "AutoRotationSpeed", TW_TYPE_FLOAT, &m_autoRotationSpeed, "");
	TwAddVarRW(m_pBar, "Scale", TW_TYPE_DIR3F, &m_scale, "");

	m_pMeshManager = new MeshImporter();
	m_pMeshManager->m_pDevice = m_pDeviceManager->m_pDevice;
	m_pMeshManager->m_pDeviceContext = m_pDeviceManager->m_pDeviceContext;
	m_pMeshManager->m_pSampleLinear = m_pDeviceManager->m_pSampleLinear;
	m_pMeshManager->m_pConstantBuffer = m_pDeviceManager->m_pConstantBuffer1;
}

void Application::Update(HWND hwnd, SIZE windowSize)
{

}

void Application::RenderSetUp(HWND hwnd, SIZE windowSize)
{
	m_pDeviceManager->RenderSetUp(windowSize);

	if (m_pMeshManager)
	{
		m_pMeshManager->m_view = m_pDeviceManager->m_view;
		m_pMeshManager->m_proj = m_pDeviceManager->m_projection;

		//レンダー
		D3DXMATRIX matRotation;
		if (m_isAutoRotation)
		{
			D3DXMatrixRotationY(&matRotation, m_autoRotationSpeed * 0.0001f);
			D3DXQUATERNION autoRotationValue;
			D3DXQuaternionRotationMatrix(&autoRotationValue, &matRotation);
			m_angle *= autoRotationValue;
		}
		D3DXMatrixRotationQuaternion(&matRotation, &m_angle);

		D3DXMATRIX matScaling;
		D3DXMatrixScaling(&matScaling, m_scale.x, m_scale.y, m_scale.z);

		D3DXMATRIX matTranslate;
		D3DXMatrixTranslation(&matTranslate, m_position.x, m_position.y, m_position.z);

		D3DXMATRIX mWorld = matScaling * matRotation * matTranslate;
		for (int i = 0; i < meshDataList.size(); i++)
		{
			m_pMeshManager->RenderMesh(&mWorld, meshDataList[i]);
		}
	}

	RenderTwText(50, 50, "messageA");
	RenderTwText(50, 62, "messageB");

#if true // インプットテスト

	// キーボードのインプット
	if (m_pInput->GetKeyDown(DIK_T)) { RenderTwText(10, 10, "KeyDownT"); };
	if (m_pInput->GetKeyUp(DIK_R)) { RenderTwText(10, 20, "KeyDownT"); };
	if (m_pInput->GetKeyUp(DIK_A)) { RenderTwText(10, 30, "KeyDownT"); };
	if (m_pInput->GetKeyDown(DIK_R)) { RenderTwText(10, 40, "KeyDownT"); };
	if (m_pInput->GetKeyDown(DIK_A)) { RenderTwText(10, 50, "KeyDownT"); };
	if (m_pInput->GetKeyDown(DIK_R)) { RenderTwText(10, 60, "KeyDownT"); }
	if (m_pInput->GetKey(DIK_E)) { RenderTwText(10, 65, "GetKey"); }

	// マウスインプット
	if (m_pInput->GetMouseButtonUp(Input::MOUSE_BUTTON_RIGHT)) { RenderTwText(10, 70, "KeyDownT"); }
	if (m_pInput->GetMouseButtonDown(Input::MOUSE_BUTTON_RIGHT)) { RenderTwText(10, 80, "KeyDownT"); }
	if (m_pInput->GetMouseButton(Input::MOUSE_BUTTON_LEFT)) { RenderTwText(10, 90, "KeyDownT"); }

	D3DXVECTOR2 mousePos = m_pInput->GetMousePos(windowSize.cx, windowSize.cy, hwnd);
	char message[100];
	wsprintfA(message, "Mouse Pos ( X:%d , Y:%d )", (int)mousePos.x, (int)mousePos.y);
	RenderTwText(10, 100, message);
#endif

	TwDraw();

	m_pDeviceManager->UpdateScreen();
}

void Application::InputKeyUpdate()
{
	m_pInput->UpdateKeyCode();
	m_pInput->UpdateMouseCode();
}

void Application::ResizeWindow(LPARAM lParam)
{
	m_pDeviceManager->ResizeRenderWindow(lParam);
}

void Application::MeshInitEvent()
{
	/*
	if (m_pMeshManager)
	{
		delete m_pMeshManager;
	}

	*/
	//m_pMeshManager = new MeshImporter;
	//m_pMeshManager->m_pDevice = m_pDeviceManager->m_pDevice;
	//m_pMeshManager->m_pDeviceContext = m_pDeviceManager->m_pDeviceContext;
	//m_pMeshManager->m_pSampleLinear = m_pDeviceManager->m_pSampleLinear;
	//m_pMeshManager->m_pConstantBuffer = m_pDeviceManager->m_pConstantBuffer1;
	//m_pMeshManager->CreateFromFBX(pFBXFileName);

	meshDataList.push_back(m_pMeshManager->CreateMeshData(pFBXFileName));
}