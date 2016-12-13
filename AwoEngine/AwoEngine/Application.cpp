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
	// device init
	m_pDeviceManager = new DeviceManager;
	m_pDeviceManager->InitD3D(hWnd, windowSize);

	TwInit(TW_DIRECT3D11, m_pDeviceManager->m_pDevice);
#if false
	TwWindowSize(windowSize.cy, windowSize.cx);
	m_pBar = TwNewBar("ModelDetail");
	TwAddVarRW(m_pBar, "Position", TW_TYPE_DIR3F, &m_position, "");
	TwAddVarRW(m_pBar, "Angle", TW_TYPE_QUAT4F, &m_angle, "");
	TwAddVarRW(m_pBar, "AutoRotation", TW_TYPE_BOOLCPP, &m_isAutoRotation, "");
	TwAddVarRW(m_pBar, "AutoRotationSpeed", TW_TYPE_FLOAT, &m_autoRotationSpeed, "");
	TwAddVarRW(m_pBar, "Scale", TW_TYPE_DIR3F, &m_scale, "");
#endif

	// canera init
	m_pCamera = new Camera();
	//m_pCamera->ShowCameraEditWindow(windowSize);

	// mesh importer init
	m_pMeshManager = new MeshImporter();
	m_pMeshManager->m_pDevice = m_pDeviceManager->m_pDevice;
	m_pMeshManager->m_pDeviceContext = m_pDeviceManager->m_pDeviceContext;
	m_pMeshManager->m_pSampleLinear = m_pDeviceManager->m_pSampleLinear;
	m_pMeshManager->m_pConstantBuffer = m_pDeviceManager->m_pConstantBuffer1;

	//MeshData* pMeshData	= m_pMeshManager->CreateMeshData("ball_low.fbx");
	//MeshData* pMeshData = m_pMeshManager->CreateMeshData("table.fbx");


	/*
	//  ball 1
	Transform* pTransform = new Transform();
	pTransform->m_position = D3DXVECTOR3(0, 5, 0);
	pTransform->m_angle = D3DXQUATERNION(0,0,0,1);
	pTransform->m_scale = D3DXVECTOR3(1, 1, 1);

	GameObject* pBall = new GameObject(pTransform, pMeshData, m_pPhysics);
	m_pGameObjList.push_back(pBall);
	
	// ball 2
	Transform* pTransform2 = new Transform();
	pTransform2->m_position = D3DXVECTOR3(1000, 5, 0);
	pTransform2->m_angle = D3DXQUATERNION(0, 0, 0, 1);
	pTransform2->m_scale = D3DXVECTOR3(1, 1, 1);
	
	GameObject* pBall2 = new GameObject(pTransform2, pMeshData, m_pPhysics);
	m_pGameObjList.push_back(pBall2);
	*/
#if false
	// create balls
	{
		// mesh datas
		MeshData* pBallMesh0 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_0_d.bmp", pBallMesh0);
		MeshData* pBallMesh1 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_1_d.bmp", pBallMesh1);
		MeshData* pBallMesh2 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_2_d.bmp", pBallMesh2);
		MeshData* pBallMesh3 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_3_d.bmp", pBallMesh3);
		MeshData* pBallMesh4 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_4_d.bmp", pBallMesh4);
		MeshData* pBallMesh5 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_5_d.bmp", pBallMesh5);
		MeshData* pBallMesh6 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_6_d.bmp", pBallMesh6);
		MeshData* pBallMesh7 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_7_d.bmp", pBallMesh7);
		MeshData* pBallMesh8 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_8_d.bmp", pBallMesh8);
		MeshData* pBallMesh9 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_9_d.bmp", pBallMesh9);
		MeshData* pBallMesh10 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_10_d.bmp", pBallMesh10);
		MeshData* pBallMesh11 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_11_d.bmp", pBallMesh11);
		MeshData* pBallMesh12 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_12_d.bmp", pBallMesh12);
		MeshData* pBallMesh13 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_13_d.bmp", pBallMesh13);
		MeshData* pBallMesh14 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_14_d.bmp", pBallMesh14);
		MeshData* pBallMesh15 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_15_d.bmp", pBallMesh15);

		const int r = 5.71;// radius
		m_ballCount = 16;
		D3DXVECTOR3 cPos = D3DXVECTOR3(0, 100, 70); // centerPos

		m_ppBalls = new Ball*[m_ballCount];
		m_ppBalls[0]  = new Ball(pBallMesh0,  D3DXVECTOR3(0,				cPos.y, -100		), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 0);// white ball
		m_ppBalls[1]  = new Ball(pBallMesh1,  D3DXVECTOR3(cPos.x,			cPos.y, cPos.z - 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[2]  = new Ball(pBallMesh2,  D3DXVECTOR3(cPos.x + r,		cPos.y, cPos.z - 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[3]  = new Ball(pBallMesh3,  D3DXVECTOR3(cPos.x - r,		cPos.y, cPos.z - 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[4]  = new Ball(pBallMesh4,  D3DXVECTOR3(cPos.x + 2*r,		cPos.y, cPos.z		), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[5]  = new Ball(pBallMesh5,  D3DXVECTOR3(cPos.x - 2*r,		cPos.y, cPos.z		), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[6]  = new Ball(pBallMesh6,  D3DXVECTOR3(cPos.x + 3*r,		cPos.y, cPos.z + 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[7]  = new Ball(pBallMesh7,  D3DXVECTOR3(cPos.x + 4*r,		cPos.y, cPos.z + 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// L ball
		m_ppBalls[8]  = new Ball(pBallMesh8,  D3DXVECTOR3(cPos.x,			cPos.y, cPos.z		), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// 8 ball
		m_ppBalls[9]  = new Ball(pBallMesh9,  D3DXVECTOR3(cPos.x - 4*r,		cPos.y, cPos.z + 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// H ball
		m_ppBalls[10] = new Ball(pBallMesh10, D3DXVECTOR3(cPos.x + r,		cPos.y, cPos.z + 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[11] = new Ball(pBallMesh11, D3DXVECTOR3(cPos.x - r,		cPos.y, cPos.z + 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[12] = new Ball(pBallMesh12, D3DXVECTOR3(cPos.x - 3*r,		cPos.y, cPos.z + 2*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[13] = new Ball(pBallMesh13, D3DXVECTOR3(cPos.x + 2*r,		cPos.y, cPos.z + 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[14] = new Ball(pBallMesh14, D3DXVECTOR3(cPos.x,			cPos.y, cPos.z + 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[15] = new Ball(pBallMesh15, D3DXVECTOR3(cPos.x - 2*r,		cPos.y, cPos.z + 4*r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);

		/*
		for (int i = 0; i < m_ballCount; i++)
		{
			m_ppBalls[i] = new Ball();

			Transform* pTransform = new Transform();
			pTransform->m_position = D3DXVECTOR3(0, 0, 0);
			pTransform->m_angle = D3DXQUATERNION(0, 0, 0, 1);
			pTransform->m_scale = D3DXVECTOR3(1, 1, 1);

			m_ppBalls[i]->SetTransform(pTransform);
			m_ppBalls[i]->SetMesh(pBallMesh);
		}
		*/
	}

	// create table
	{
		MeshData* pTableMesh = m_pMeshManager->CreateMeshData("table.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "table_d.bmp", pTableMesh);

		m_pTable = new Table(pTableMesh, D3DXVECTOR3(0, 0, 0), D3DXQUATERNION(0, 0, 0, 1));

		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(150, 50, 300));	// 底面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(-88.5, 80, 0), D3DXVECTOR3(50, 100, 300));	// 左側面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(88.5, 80, 0), D3DXVECTOR3(50, 100, 300));	// 右側面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, 152), D3DXVECTOR3(150, 100, 50));   // 奥
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, -152), D3DXVECTOR3(150, 100, 50));  // 手前
	}
#endif
}

void Application::Update(HWND hwnd, SIZE windowSize)
{
	/*
	for (int i = 0; i < m_pGameObjList.size(); i++)
	{
		m_pGameObjList[i]->Update();
	}

	if (m_pInput->GetKeyDown(DIK_R))
	{
		//m_pGameObjList[0]->m_pRigidBody->addForce(PxVec3(10000, 10000, 0));
		m_pGameObjList[0]->m_pRigidBody->setLinearVelocity(PxVec3(200, 0, 0));
	}
	*/

	if (m_pInput->GetKeyDown(DIK_S) && m_ballCount > 0)
	{
		//m_ppBalls[0]->m_pRigidBody->addForce(PxVec3(100, 0, 0));
		/*
		D3DXVECTOR3* pShootVec = new D3DXVECTOR3;
		D3DXVec3Normalize(pShootVec,&D3DXVECTOR3(m_shootAngle));
		*pShootVec = *pShootVec * m_shootPower;
		m_ppBalls[0]->m_pRigidBody->setLinearVelocity(PxVec3(pShootVec->x,pShootVec->y,pShootVec->z));
		delete pShootVec;
		//D3DXVec3
		*/
		/*
		D3DXVECTOR3 shootVer = D3DXVECTOR3();

		const float c_X = m_shootAngle.x;
		const float c_Y = m_shootAngle.y;
		const float c_Z = m_shootAngle.z;
		const float c_W = m_shootAngle.w;
		const float c_singularityTest = c_Z * c_X - c_W * c_Y;
		const float c_yawY = 2.0f * (c_W * c_Z + c_X * c_Y);
		const float c_yawX = (1.0f - 2.0f * ((c_Y * c_Y) + (c_Z * c_Z)));

		const float c_singularityThreshold = 0.499f;
		const float c_radToDeg = (180.f) / D3DX_PI;

		if (c_singularityTest < -c_singularityThreshold)
		{
			shootVer.x = -90.0f;
			shootVer.y = atan2(c_yawY, c_yawX) * c_radToDeg;
			shootVer.z = 
		}
		else if (c_singularityTest > c_singularityThreshold)
		{

		}
		else
		{
		
		}
		*/

		m_pAudio->PlayCue(SE_BALL_HIT);

		D3DXVECTOR3 vec = D3DXVECTOR3(0,0,0);
		vec.x = sinf(m_shootAngle * DIG_TO_RAD);
		vec.z = cosf(m_shootAngle * DIG_TO_RAD);
		D3DXVECTOR3 shootVec = D3DXVECTOR3();
		D3DXVec3Normalize(&shootVec, &vec);
		shootVec *= m_shootPower;
		m_ppBalls[0]->m_pRigidBody->setLinearVelocity(PxVec3(shootVec.x, shootVec.y, shootVec.z));//setLinearVelocity(PxVec3(shootVec.x, shootVec.y, shootVec.z));

		//setLinearVelocity();
		//m_ppBalls[0]->m_pRigidBody->setLinearVelocity(PxVec3(0, 0, 300));
	}

	CollisionCheckForHolls();
	CollisionCheckForBalls();
}

void Application::CollisionCheckForHolls()
{
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }

		for (int k = 0; k < m_holeCount; k++)
		{
			if (IsBallHitToHole(m_ppBalls[i],m_ppHoles[k]))
			{
				m_pAudio->PlayCue(SE_BALL_FALL);
				char msg[30];
				wsprintfA(msg, "%d BALL POCKET TO %d HALL", i, k);
				MessageBoxA(nullptr, msg, "BALL POCKET", MB_OK);
				m_ppBalls[i]->isRender = false;
			}
		}
	}
}

void Application::CollisionCheckForBalls()
{
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }
		for (int k = i+1; k < m_ballCount; k++)
		{
			if (m_ppBalls[k]->isRender == false) { continue; }
			if (i == k) { continue; }
			if (IsBallHitToBall(m_ppBalls[i], m_ppBalls[k]))
			{
				m_pAudio->PlayCue(SE_BALL_HIT);
			}
		}
	}
}

bool Application::IsBallHitToHole(const Ball* pBall, const Hole* pHole)
{
	// 2つのオブジェクトの距離(2乗)
	D3DXVECTOR3 distSquared = pBall->m_pTransform->m_position - pHole->m_pTransform->m_position;
	distSquared.x = distSquared.x * distSquared.x;
	//distSquared.y = distSquared.y * distSquared.y;
	distSquared.z = distSquared.z * distSquared.z;
	
	// 2つのオブジェクトの半径の合計(2乗)
	float sumRadiusSquared = pBall->m_radius + pHole->m_radius;
	sumRadiusSquared = sumRadiusSquared * sumRadiusSquared;

	// 衝突しているか
	if (distSquared.x + distSquared.z <= sumRadiusSquared)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Application::IsBallHitToBall(const Ball* pBall, const Ball* pBall2)
{
	// 2つのオブジェクトの距離(2乗)
	D3DXVECTOR3 distSquared = pBall->m_pTransform->m_position - pBall2->m_pTransform->m_position;
	distSquared.x = distSquared.x * distSquared.x;
	//distSquared.y = distSquared.y * distSquared.y;
	distSquared.z = distSquared.z * distSquared.z;

	// 2つのオブジェクトの半径の合計(2乗)
	float sumRadiusSquared = pBall->m_radius + pBall2->m_radius;
	sumRadiusSquared = sumRadiusSquared * sumRadiusSquared;

	// 衝突しているか
	if (distSquared.x + distSquared.z <= sumRadiusSquared-1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Application::RenderSetUp(HWND hwnd, SIZE windowSize)
{
	m_pDeviceManager->RenderSetUp(windowSize);

	if (m_pMeshManager)
	{
#if false
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
#else
		m_pCamera->CameraSetting(m_pDeviceManager);

		m_pMeshManager->m_view = m_pCamera->GetView();//m_pDeviceManager->m_view;
		m_pMeshManager->m_proj = m_pCamera->GetProjection(m_pDeviceManager->m_depthStencilDesc);//m_pDeviceManager->m_projection;
#pragma region Gameobjects render

		// ball render
		for (int i = 0; i < m_ballCount; i++)
		{
			/*
			D3DXMATRIX world = m_ppBalls[i]->m_pTransform->GetWorld();
			MeshData* pMeshData = m_ppBalls[i]->m_pRootMesh;
			m_pMeshManager->RenderMesh(&world, pMeshData);
			*/
			RenderGameobject(m_ppBalls[i]);
		}
		// table render
		if(m_pTable)
		{
			/*
			D3DXMATRIX world = m_pTable->m_pTransform->GetWorld();
			MeshData* pMeshData = m_pTable->m_pRootMesh;
			m_pMeshManager->RenderMesh(&world, pMeshData);
			*/
			RenderGameobject(m_pTable);
		}
		// arrow render
		if (m_pArrow)
		{
			/*
			D3DXMATRIX world = m_pArrow->m_pTransform->GetWorld();
			MeshData* pMeshData = m_pArrow->m_pRootMesh;
			m_pMeshManager->RenderMesh(&world, pMeshData);
			*/
			RenderGameobject(m_pArrow);
		}

		/*
		for (int i = 0; i < m_pGameObjList.size(); i++)
		{
			D3DXMATRIX world = m_pGameObjList[i]->m_pTransform->GetWorld();
			MeshData* pMeshData = m_pGameObjList[i]->m_pRootMesh;
			m_pMeshManager->RenderMesh(&world, pMeshData);
		}
		*/
#pragma endregion

#endif
	}

	//RenderTwText(50, 50, "messageA");
	//RenderTwText(50, 62, "messageB");
	if (m_ballCount <= 0)
	{
		RenderTwText(50, 50, "Now Loading ...");
	}
	else
	{
		//RenderTwText(50, 50, "Reset command : R");
		RenderTwText(50, 70, "Shoot command : S");
		RenderTwText(50, 90, "FullScreen toggle : Alt + Enter");
	}

#if false // インプットテスト

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
#endif

	TwDraw();

	// PhysX
	m_pPhysics->StepPhysics();

	// PhysX 移動
	/*
	for (int i = 0; i < m_pGameObjList.size(); i++)
	{
		m_pGameObjList[i]->UpdatePos();
	}
	*/
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }
		m_ppBalls[i]->UpdatePos();
	}
	if (m_pArrow)
	{
		m_pArrow->m_pTransform->m_position = m_ppBalls[0]->m_pTransform->m_position;
		
		D3DXMATRIX matRotation;
		D3DXMatrixRotationY(&matRotation, m_shootAngle * DIG_TO_RAD);
		D3DXQUATERNION autoRotationValue;
		D3DXQuaternionRotationMatrix(&autoRotationValue, &matRotation);
		m_pArrow->m_pTransform->m_angle = autoRotationValue;
		//m_pArrow->m_pTransform->m_angle = m_shootAngle;
	}
	// 画面更新
	m_pDeviceManager->UpdateScreen();

	if (m_ballCount <= 0)
	{
		CreateModel();
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

	//_ meshDataList.push_back(m_pMeshManager->CreateMeshData(pFBXFileName));
}


void Application::CreateModel()
{
	//OutputDebugString("Start create model\n");

	// create balls
	const int c_ballRadius = 5.71 / 2;
	{
		// mesh datas
		MeshData* pBallMesh0 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_0_d.bmp", pBallMesh0);
		MeshData* pBallMesh1 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_1_d.bmp", pBallMesh1);
		MeshData* pBallMesh2 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_2_d.bmp", pBallMesh2);
		MeshData* pBallMesh3 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_3_d.bmp", pBallMesh3);
		MeshData* pBallMesh4 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_4_d.bmp", pBallMesh4);
		MeshData* pBallMesh5 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_5_d.bmp", pBallMesh5);
		MeshData* pBallMesh6 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_6_d.bmp", pBallMesh6);
		MeshData* pBallMesh7 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_7_d.bmp", pBallMesh7);
		MeshData* pBallMesh8 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_8_d.bmp", pBallMesh8);
		MeshData* pBallMesh9 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_9_d.bmp", pBallMesh9);
		MeshData* pBallMesh10 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_10_d.bmp", pBallMesh10);
		MeshData* pBallMesh11 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_11_d.bmp", pBallMesh11);
		MeshData* pBallMesh12 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_12_d.bmp", pBallMesh12);
		MeshData* pBallMesh13 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_13_d.bmp", pBallMesh13);
		MeshData* pBallMesh14 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_14_d.bmp", pBallMesh14);
		MeshData* pBallMesh15 = m_pMeshManager->CreateMeshData("ball_low.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "ball_15_d.bmp", pBallMesh15);

		const int r = c_ballRadius * 2 + 1.8;
		m_ballCount = 16;
		D3DXVECTOR3 cPos = D3DXVECTOR3(0, 100, 70); // centerPos

		m_ppBalls = new Ball*[m_ballCount];
		m_ppBalls[0] = new Ball(pBallMesh0, D3DXVECTOR3(0, cPos.y, -100), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 0);// white ball
		m_ppBalls[1] = new Ball(pBallMesh1, D3DXVECTOR3(cPos.x, cPos.y, cPos.z - 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[2] = new Ball(pBallMesh2, D3DXVECTOR3(cPos.x + r, cPos.y, cPos.z - 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[3] = new Ball(pBallMesh3, D3DXVECTOR3(cPos.x - r, cPos.y, cPos.z - 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[4] = new Ball(pBallMesh4, D3DXVECTOR3(cPos.x + 2 * r, cPos.y, cPos.z), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[5] = new Ball(pBallMesh5, D3DXVECTOR3(cPos.x - 2 * r, cPos.y, cPos.z), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[6] = new Ball(pBallMesh6, D3DXVECTOR3(cPos.x + 3 * r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[7] = new Ball(pBallMesh7, D3DXVECTOR3(cPos.x + 4 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// L ball
		m_ppBalls[8] = new Ball(pBallMesh8, D3DXVECTOR3(cPos.x, cPos.y, cPos.z), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// 8 ball
		m_ppBalls[9] = new Ball(pBallMesh9, D3DXVECTOR3(cPos.x - 4 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);// H ball
		m_ppBalls[10] = new Ball(pBallMesh10, D3DXVECTOR3(cPos.x + r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[11] = new Ball(pBallMesh11, D3DXVECTOR3(cPos.x - r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[12] = new Ball(pBallMesh12, D3DXVECTOR3(cPos.x - 3 * r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[13] = new Ball(pBallMesh13, D3DXVECTOR3(cPos.x + 2 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[14] = new Ball(pBallMesh14, D3DXVECTOR3(cPos.x, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);
		m_ppBalls[15] = new Ball(pBallMesh15, D3DXVECTOR3(cPos.x - 2 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 0, 0, 1), m_pPhysics, r, 1);

		/*
		for (int i = 0; i < m_ballCount; i++)
		{
		m_ppBalls[i] = new Ball();

		Transform* pTransform = new Transform();
		pTransform->m_position = D3DXVECTOR3(0, 0, 0);
		pTransform->m_angle = D3DXQUATERNION(0, 0, 0, 1);
		pTransform->m_scale = D3DXVECTOR3(1, 1, 1);

		m_ppBalls[i]->SetTransform(pTransform);
		m_ppBalls[i]->SetMesh(pBallMesh);
		}
		*/
	}

	const int c_tableWidth = 127;//127
	const int c_tableHeight = 254;
	const int c_colliderSize = 50;

	// create table
	{
		const int w = c_tableWidth;
		const int h = c_tableHeight;
		const int c = c_colliderSize;
		const int r = c_ballRadius;

		const int offs = 9;//offset

		MeshData* pTableMesh = m_pMeshManager->CreateMeshData("table.fbx");
		m_pMeshManager->SetMaterialTexture(m_pDeviceManager->m_pDevice, "table_d.bmp", pTableMesh);

		m_pTable = new Table(pTableMesh, D3DXVECTOR3(0, 0, 0), D3DXQUATERNION(0, 0, 0, 1));
#if false
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(150, 50, 300));	// 底面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(-88.5, 80, 0), D3DXVECTOR3(50, 100, 300));	// 左側面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(88.5, 80, 0), D3DXVECTOR3(50, 100, 300));	// 右側面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, 152), D3DXVECTOR3(150, 100, 50));   // 奥
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, -152), D3DXVECTOR3(150, 100, 50));  // 手前
#else
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(150, 50, 300));		// 底面
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 130, 0), D3DXVECTOR3(150, 50, 300));	// 上面

		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2), 80, (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r)- offs));	// 左側面 奥
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2), 80, -1 * (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r)- offs)); // 左側面 前
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2) - 1 * r*2, 80, 0), D3DXVECTOR3(c, 100, h+(2*r)+10));	// 左側面　左でっぱり

		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3((w / 2 + c / 2), 80, (h/4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r)- offs)); // 右側面 奥
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3((w / 2 + c / 2), 80, -1 * (h/4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r)- offs)); // 右側面 前
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3((w / 2 + c / 2) + r*2, 80, 0), D3DXVECTOR3(c, 100, h+(2*r)+10));	// 右側面 右でっぱり

		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, (h / 2) + (c / 2)), D3DXVECTOR3(w-2*r- offs, 100, c)); // 奥 手前
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, (h / 2) + (c / 2) + r*2), D3DXVECTOR3(w+(2*r)+10, 100, c)); // 奥 奥

		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, -1*(h/2+c/2)), D3DXVECTOR3(w - 2 * r- offs, 100, c));  // 手前 奥
		m_pTable->AddRigidStatic(m_pPhysics, D3DXVECTOR3(0, 80, -1 * (h / 2 + c / 2) + (r*-2)), D3DXVECTOR3(w + (2 * r) + 10, 100, c));  // 手前 手前
#endif
	}

	// create holes
	{
		//const int c_radius = 57.1 / 2.0;
		const int r = c_ballRadius;
		const int w = c_tableWidth;
		const int h = c_tableHeight;
		m_holeCount = 6;
		m_ppHoles = new Hole*[m_holeCount];

		m_ppHoles[0] = new Hole(D3DXVECTOR3(-w/2,50+r,h/2), r);// 左上
		m_ppHoles[1] = new Hole(D3DXVECTOR3(-w/2,50+r,0), r);// 左中
		m_ppHoles[2] = new Hole(D3DXVECTOR3(-w/2,50+r,-h/2), r);// 左下
		m_ppHoles[3] = new Hole(D3DXVECTOR3(w/2,50+r,h/2), r);// 右上
		m_ppHoles[4] = new Hole(D3DXVECTOR3(w/2,50+r,0), r);// 右中
		m_ppHoles[5] = new Hole(D3DXVECTOR3(w/2,50+r,-h/2), r);// 右下
	}

	// create arrow
	{
		//m_shootAngle = D3DXQUATERNION(0, 0, 0, 1);
		m_shootAngle = 0;
		m_shootPower = 50;

		m_pArrow = new Arrow();

		Transform* pTransform = new Transform();
		pTransform->m_position = m_ppBalls[0]->m_pTransform->m_position;
		
		D3DXMATRIX matRotation = D3DXMATRIX();
		D3DXMatrixRotationY(&matRotation, m_shootAngle * DIG_TO_RAD);
		D3DXQUATERNION autoRotationValue = D3DXQUATERNION();
		D3DXQuaternionRotationMatrix(&autoRotationValue, &matRotation);
		pTransform->m_angle = autoRotationValue;
		//D3DXMatrixRotationQuaternion(&matRotation, &m_angle);
		//D3DXQuaternionRotationMatrix(&pTransform->m_angle, &matRotation);
		//pTransform->m_angle = m_shootAngle;

		pTransform->m_scale = D3DXVECTOR3(7, 7, 7);
		m_pArrow->SetTransform(pTransform);
		
		m_pArrow->SetMesh(m_pMeshManager->CreateMeshData("arrow.fbx"));

		m_pShooterBar = TwNewBar("ShooterSetting");
		TwAddVarRO(m_pShooterBar, "arrowQuat", TW_TYPE_QUAT4F, &m_pArrow->m_pTransform->m_angle, "");
		TwAddVarRW(m_pShooterBar, "ShootingAngle", TW_TYPE_FLOAT, &m_shootAngle, "");
		TwAddVarRW(m_pShooterBar, "ShootingPower", TW_TYPE_FLOAT, &m_shootPower, "");
	}

	//OutputDebugString("complete create model\n");
}