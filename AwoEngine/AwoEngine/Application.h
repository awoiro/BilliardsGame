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

class Application
{
public:
	Application()
		: m_pDeviceManager(nullptr)
		, m_pMeshManager(nullptr)
		, m_position(0, 0, 0)
		, m_angle(0, 0, 0, 1)
		, m_isAutoRotation(true)
		, m_autoRotationSpeed(3.0f)
		, m_scale(1000, 1000, 1000)
	{
		pFBXFileName = strdup("ball_s.fbx");

		m_pAudio = new Audio();
		m_pAudio->Init();
		m_pAudio->PlayCue(BGM1);
		
		m_pInput = new Input();
		m_pPhysics = new Physics();
	};
	~Application()
	{
		TwTerminate();
		
		delete m_pTable;
		delete[] m_ppBalls;
//		m_pGameObjList.clear();

		delete m_pCamera;

		delete m_pPhysics;

		delete m_pInput;
		
		m_pAudio->StopCue(BGM1);
		delete m_pAudio;

		delete pFBXFileName;
		delete m_pDeviceManager;
	};

	void Init(HWND hWnd, SIZE windowSize);
	void Update(HWND hwnd, SIZE windowSize);
	void RenderSetUp(HWND hwnd, SIZE windowSize);
	void InputKeyUpdate();
	void ResizeWindow(LPARAM lParam);

	void MeshInitEvent();
	void CreateModel();

	// device
	DeviceManager* m_pDeviceManager;
	
	// mesh data
	MeshImporter* m_pMeshManager;
	char* pFBXFileName;
	//std::vector<MeshData*> meshDataList = std::vector<MeshData*>();
	
	// antWeenBar
	//TwBar* m_pBar;

	// meshOrientation
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_angle;
	D3DXVECTOR3 m_scale;
	bool m_isAutoRotation;
	float m_autoRotationSpeed;

	// sound
	Audio* m_pAudio = nullptr;

	// input
	Input* m_pInput = nullptr;

	// PhysX
	Physics* m_pPhysics = nullptr;

	// Camera
	Camera* m_pCamera = nullptr;

	// GameObjects
	//std::vector<GameObject*> m_pGameObjList = std::vector<GameObject*>();

	// Balls
	Ball** m_ppBalls = nullptr;
	int m_ballCount = -1;

	// Table
	Table* m_pTable = nullptr;
};
