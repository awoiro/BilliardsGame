#include "Application.h"

void Application::Init(HWND hWnd, SIZE windowSize)
{
	// device init
	m_pDeviceManager = new DeviceManager;
	m_pDeviceManager->InitD3D(hWnd, windowSize);

	// antWeekBar init
	TwInit(TW_DIRECT3D11, m_pDeviceManager->m_pDevice);

	// canera init
	m_pCamera = new Camera();

	// input init 
	m_pInput = new Input();

	// physics init
	m_pPhysics = new Physics();

	// mesh importer init
	m_pMeshManager = new MeshImporter();
	m_pMeshManager->m_pDevice = m_pDeviceManager->m_pDevice;
	m_pMeshManager->m_pDeviceContext = m_pDeviceManager->m_pDeviceContext;
	m_pMeshManager->m_pSampleLinear = m_pDeviceManager->m_pSampleLinear;
	m_pMeshManager->m_pConstantBuffer = m_pDeviceManager->m_pConstantBuffer1;

	// audio init
	m_pAudio = new Audio();
	m_pAudio->Init();
	m_pAudio->PlayCue(BGM3);

	// game manager init
	m_pGameManager = new GameManager();
}

void Application::Update(HWND hwnd, SIZE windowSize)
{
	// input event
#if false
	if (m_pInput->GetKeyDown(DIK_R)) // reset
	{
		m_pGameManager->Reset();
	}
#endif

	if (m_pInput->GetKeyDown(DIK_S)) // shoot
	{
		m_pGameManager->Shoot(m_pAudio);
	}

	// game manager
	m_pGameManager->Update(m_pDeviceManager,m_pMeshManager,m_pPhysics, m_pAudio);
}

void Application::RenderSetUp(HWND hwnd, SIZE windowSize)
{
	m_pDeviceManager->RenderSetUp(windowSize);

	if (m_pMeshManager)
	{
		m_pCamera->CameraSetting(m_pDeviceManager);

		m_pMeshManager->m_view = m_pCamera->GetView();
		m_pMeshManager->m_proj = m_pCamera->GetProjection(m_pDeviceManager->m_depthStencilDesc);
	
		// obj render
		m_pGameManager->Render(m_pMeshManager);
	}

	// tw window and text render
	TwDraw();

	// PhysX
	m_pPhysics->StepPhysics();

	// PhysX move
	m_pGameManager->PhysicsMoveToBall();

	// screen update
	m_pDeviceManager->UpdateScreen();

	// create models
	if (m_pGameManager->CheckIsCreateModel() == false)
	{
		m_pGameManager->CreateModels(m_pDeviceManager, m_pMeshManager, m_pPhysics);
		m_pCamera->ShowCameraEditWindow();
	}
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

void Application::ResizeWindow(SIZE windowSize)
{
	m_pDeviceManager->ResizeRenderWindow(windowSize);
}



#if false // インプットテスト
void InputTest()
{

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

	D3DXVECTOR2 mousePos = m_pInput->GetMousePos(windowSize.cx, windowSize.cy, hWnd);
	char message[100];
	wsprintfA(message, "Mouse Pos ( X:%d , Y:%d )", (int)mousePos.x, (int)mousePos.y);
	RenderTwText(10, 100, message);
}
#endif