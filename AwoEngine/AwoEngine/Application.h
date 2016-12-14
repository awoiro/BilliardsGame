#pragma once
#include <Windows.h>
#include <vector>
#include <AntTweakBar.h>
#include "DeviceManager.h"
#include "MeshData.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameManager.h"

class Application
{
public:
	Application()
		: m_pDeviceManager(nullptr)
		, m_pMeshManager(nullptr)
		, m_pInput(nullptr)
		, m_pPhysics(nullptr)
		, m_pCamera(nullptr)
		, m_pAudio(nullptr)
		, m_pGameManager(nullptr)
	{};
	~Application()
	{
		TwTerminate();
		delete m_pGameManager;
		delete m_pAudio;
		delete m_pCamera;
		delete m_pPhysics;
		delete m_pInput;
		delete m_pMeshManager;
		delete m_pDeviceManager;
	};

	void Init(HWND hWnd, SIZE windowSize);
	void Update(HWND hwnd, SIZE windowSize);

	void RenderSetUp(HWND hwnd, SIZE windowSize);
	void InputKeyUpdate();

	void ResizeWindow(LPARAM lParam);
	void ResizeWindow(SIZE windowSize);

private:
	// device
	DeviceManager* m_pDeviceManager;
	
	// mesh manager
	MeshImporter* m_pMeshManager;

	// input
	Input* m_pInput;

	// PhysX
	Physics* m_pPhysics;

	// Camera
	Camera* m_pCamera;

	// Audio
	Audio* m_pAudio;

	// GameManager
	GameManager* m_pGameManager;
};
