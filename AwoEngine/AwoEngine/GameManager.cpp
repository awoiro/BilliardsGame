#include "GameManager.h"

void GameManager::Reset()
{}

void GameManager::Update(DeviceManager* pDeviceManager, MeshImporter* pMeshManager, Physics* pPhysics, Audio* pAudio)
{
	if (m_isPlayGame)
	{
		// shooter move
		m_pShooter->MoveShootingPosition();
		m_pShooter->UpdateArrowLocation();

		// collider
		Collider(pAudio,pPhysics);

		// foul check and player change
		if (CheckBallStop() && CheckIsShootAiming() == false)
		{
			CheckFoul() ? PenaltyPlayerChange() : PlayerChange();

			// no 0 ball event
			if (m_ppBalls[0]->isRender == false)
			{
				m_ppBalls[0]->m_pTransform->m_position = D3DXVECTOR3(0, 100, -100);
				m_ppBalls[0]->isRender = true;
				m_ppBalls[0]->EnableRigid(pPhysics);
				m_pShooter->SetIsFreeMove(false);
				m_pShooter->SetIsKitchenMove(true);
			}
		}
	}
}

bool GameManager::CheckBallStop() const
{
	if (m_ppBalls[0]->isRender == false) { return true; }
	return m_ppBalls[0]->m_pRigidBody->isSleeping();
}

bool GameManager::CheckIsShootAiming() const
{
	return m_pShooter->GetIsShootAiming();
}

bool GameManager::CheckFoul() const
{
	if (m_turn == LOW_TURN)
	{
		if (m_firstTouchBallNum > 8) { MessageBoxA(nullptr, "ファール : ノーヒット", "ファール", MB_OK); return true; }
		if (m_touchCushion == false) { MessageBoxA(nullptr, "ファール : ノークッション", "ファール", MB_OK); return true; }
	}
	else
	{
		if (m_firstTouchBallNum < 8) { MessageBoxA(nullptr, "ファール : ノーヒット", "ファール", MB_OK); return true; }
		if (m_touchCushion == false) { MessageBoxA(nullptr, "ファール : ノークッション", "ファール", MB_OK); return true; }
	}
	return false;
}

bool GameManager::CheckWin() const
{
	if (m_turn == LOW_TURN)
	{
		for (int i = 1; i < 8; i++)
		{
			if (m_ppBalls[i]->isRender)
			{
				return false;
			}
		}
	}
	else
	{
		for (int i = 9; i < 16; i++)
		{
			if (m_ppBalls[i]->isRender)
			{
				return false;
			}
		}
	}
	return true;
}

bool GameManager::CheckIsCreateModel() const
{
	return m_isCreatedModel;
}

void GameManager::PlayerChange()
{
	if (m_turn == LOW_TURN)
	{
		MessageBoxA(nullptr, "プレイヤーチェンジ : HIGH GROUP", "チェンジ", MB_OK);
		m_turn = HIGH_TURN;
	}
	else
	{
		MessageBoxA(nullptr, "プレイヤーチェンジ : LOW GROUP", "チェンジ", MB_OK);
		m_turn = LOW_TURN;
	}

	m_pShooter->SetIsShoot(true);
	m_pShooter->SetIsShootAiming(true);
}

void GameManager::PenaltyPlayerChange()
{
	m_pShooter->SetIsFreeMove(true);

	PlayerChange();
}


#pragma region show text
void GameManager::TwRenderText(const int x, const int y, const char * msg)
{
	TwBeginText(x, y, 0, 0);

	const unsigned int color = 0xffffffff;//0xffffc0ff;
	TwAddTextLine(msg, color, 0);

	TwEndText();
}

void GameManager::ShowLoadText(const int x, const int y)
{
	TwRenderText(x, y, "Now Loading...");
}

void GameManager::ShowCommandText(const int x, const int y)
{
	TwRenderText(x, y, "COMMAND LIST");
	TwRenderText(x+10, y+20, "|- Reset command : R");
	TwRenderText(x+10, y+40, "|- Shoot command : S");
	TwRenderText(x+10, y+60, "|- FullScreen toggle : Alt + Enter");
}

void GameManager::ShowTurnText(const int x, const int y)
{
	TwRenderText(x, y, "CURRENT TURN");
	if (m_turn == LOW_TURN)
	{
		TwRenderText(x + 10, y + 20, "|- => LOW TURN <=");
		TwRenderText(x + 10, y + 40, "|-    HIGH TURN");
	}
	else
	{
		TwRenderText(x + 10, y + 20, "|-    LOW TURN");
		TwRenderText(x + 10, y + 40, "|- => HIGH TURN <=");
	}
}

void GameManager::ShowCurrentBallStateText(const int x, const int y)
{
	TwRenderText(x, y, "BALL STATE");

	// Low group text
	TwRenderText(x+10, y+20, "|- LOW GROUP");
	for (int i = 1; i < 8; i++)
	{
		if (m_ppBalls[i]->isRender)
		{
			char msg[40];
			wsprintfA(msg, "|- %d BALL : ON THE TABLE", i);
			TwRenderText(x+20, y+20 + i * 20, msg);
		}
		else
		{
			char msg[40];
			wsprintfA(msg, "|- %d BALL : FELL", i);
			TwRenderText(x+20, y+20 + i * 20, msg);
		}
	}

	// hight group text
	TwRenderText(x+10, y+180, "|- HIGH GROUP");
	for (int i = 9; i < 16; i++)
	{
		if (m_ppBalls[i]->isRender)
		{
			char msg[40];
			wsprintfA(msg, "|- %d BALL : ON THE TABLE", i);
			TwRenderText(x+20, y+20 + i * 20, msg);
		}
		else
		{
			char msg[40];
			wsprintfA(msg, "|- %d BALL : FELL", i);
			TwRenderText(x+20, y+20 + i * 20, msg);
		}
	}

	// 8 ball text
	{
		if (m_ppBalls[8]->isRender)
		{
			TwRenderText(x+10, y+340, "|- 8 BALL : ON THE TABLE");
		}
		else
		{
			TwRenderText(x+10, y+340, "|- 8 BALL : FELL");
		}
	}
}
#pragma endregion


void GameManager::CreateModels(DeviceManager* pDeviceManager, MeshImporter* pMeshManager, Physics* pPhysics)
{
	// create balls
	{
		// mesh datas
		MeshData* pBallMesh0 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_0_d.bmp", pBallMesh0);
		MeshData* pBallMesh1 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_1_d.bmp", pBallMesh1);
		MeshData* pBallMesh2 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_2_d.bmp", pBallMesh2);
		MeshData* pBallMesh3 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_3_d.bmp", pBallMesh3);
		MeshData* pBallMesh4 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_4_d.bmp", pBallMesh4);
		MeshData* pBallMesh5 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_5_d.bmp", pBallMesh5);
		MeshData* pBallMesh6 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_6_d.bmp", pBallMesh6);
		MeshData* pBallMesh7 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_7_d.bmp", pBallMesh7);
		MeshData* pBallMesh8 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_8_d.bmp", pBallMesh8);
		MeshData* pBallMesh9 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_9_d.bmp", pBallMesh9);
		MeshData* pBallMesh10 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_10_d.bmp", pBallMesh10);
		MeshData* pBallMesh11 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_11_d.bmp", pBallMesh11);
		MeshData* pBallMesh12 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_12_d.bmp", pBallMesh12);
		MeshData* pBallMesh13 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_13_d.bmp", pBallMesh13);
		MeshData* pBallMesh14 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_14_d.bmp", pBallMesh14);
		MeshData* pBallMesh15 = pMeshManager->CreateMeshData("ball_low.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "ball_15_d.bmp", pBallMesh15);

		const int r = BALL_RADIUS * 2;
		m_ballCount = 16;
		D3DXVECTOR3 cPos = D3DXVECTOR3(0, 100, 70); // centerPos

		m_ppBalls = new Ball*[m_ballCount];
		m_ppBalls[0] = new Ball(pBallMesh0, D3DXVECTOR3(0, cPos.y, -100), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 0);// white ball
		m_ppBalls[1] = new Ball(pBallMesh1, D3DXVECTOR3(cPos.x, cPos.y, cPos.z - 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[2] = new Ball(pBallMesh2, D3DXVECTOR3(cPos.x + r, cPos.y, cPos.z - 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[3] = new Ball(pBallMesh3, D3DXVECTOR3(cPos.x - r, cPos.y, cPos.z - 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[4] = new Ball(pBallMesh4, D3DXVECTOR3(cPos.x + 2 * r, cPos.y, cPos.z), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[5] = new Ball(pBallMesh5, D3DXVECTOR3(cPos.x - 2 * r, cPos.y, cPos.z), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[6] = new Ball(pBallMesh6, D3DXVECTOR3(cPos.x + 3 * r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[7] = new Ball(pBallMesh7, D3DXVECTOR3(cPos.x + 4 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);// L ball
		m_ppBalls[8] = new Ball(pBallMesh8, D3DXVECTOR3(cPos.x, cPos.y, cPos.z), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);// 8 ball
		m_ppBalls[9] = new Ball(pBallMesh9, D3DXVECTOR3(cPos.x - 4 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);// H ball
		m_ppBalls[10] = new Ball(pBallMesh10, D3DXVECTOR3(cPos.x + r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[11] = new Ball(pBallMesh11, D3DXVECTOR3(cPos.x - r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[12] = new Ball(pBallMesh12, D3DXVECTOR3(cPos.x - 3 * r, cPos.y, cPos.z + 2 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[13] = new Ball(pBallMesh13, D3DXVECTOR3(cPos.x + 2 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[14] = new Ball(pBallMesh14, D3DXVECTOR3(cPos.x, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
		m_ppBalls[15] = new Ball(pBallMesh15, D3DXVECTOR3(cPos.x - 2 * r, cPos.y, cPos.z + 4 * r), D3DXQUATERNION(0, 1, 0, 0), pPhysics, r, 1);
	

		m_pShooter->SetParentBall(m_ppBalls[0]);
	}

	// create table
	{
		const int w = TABLE_WIDTH;
		const int h = TABLE_HEIGHT;
		const int c = COLLIDER_SIZE;
		const int r = BALL_RADIUS;

		const int offs = 9;//offset

		MeshData* pTableMesh = pMeshManager->CreateMeshData("table.fbx");
		pMeshManager->SetMaterialTexture(pDeviceManager->m_pDevice, "table_d.bmp", pTableMesh);

		m_pTable = new Table(pTableMesh, D3DXVECTOR3(0, 0, 0), D3DXQUATERNION(0, 0, 0, 1));
		// add collider
		{
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(150, 50, 300));	// 底面
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 130, 0), D3DXVECTOR3(150, 50, 300));	// 上面

			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2), 80, (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r) - offs));	// 左側面 奥
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2), 80, -1 * (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r) - offs)); // 左側面 前
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(-1 * (w / 2 + c / 2) - 1 * r * 2, 80, 0), D3DXVECTOR3(c, 100, h + (2 * r) + 10));	// 左側面　左でっぱり

			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3((w / 2 + c / 2), 80, (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r) - offs)); // 右側面 奥
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3((w / 2 + c / 2), 80, -1 * (h / 4)), D3DXVECTOR3(c, 100, (h / 2) - (2 * r) - offs)); // 右側面 前
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3((w / 2 + c / 2) + r * 2, 80, 0), D3DXVECTOR3(c, 100, h + (2 * r) + 10));	// 右側面 右でっぱり

			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 80, (h / 2) + (c / 2)), D3DXVECTOR3(w - 2 * r - offs, 100, c)); // 奥 手前
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 80, (h / 2) + (c / 2) + r * 2), D3DXVECTOR3(w + (2 * r) + 10, 100, c)); // 奥 奥

			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 80, -1 * (h / 2 + c / 2)), D3DXVECTOR3(w - 2 * r - offs, 100, c));  // 手前 奥
			m_pTable->AddRigidStatic(pPhysics, D3DXVECTOR3(0, 80, -1 * (h / 2 + c / 2) + (r*-2)), D3DXVECTOR3(w + (2 * r) + 10, 100, c));  // 手前 手前
		}
	}

	// create holes
	{
		const int r = BALL_RADIUS;
		const int w = TABLE_WIDTH;
		const int h = TABLE_HEIGHT;
		m_holeCount = 6;
		m_ppHoles = new Hole*[m_holeCount];

		m_ppHoles[0] = new Hole(D3DXVECTOR3(-w / 2, 50 + r, h / 2), r);// 左上
		m_ppHoles[1] = new Hole(D3DXVECTOR3(-w / 2 - 4, 50 + r, 0), r);// 左中
		m_ppHoles[2] = new Hole(D3DXVECTOR3(-w / 2, 50 + r, -h / 2), r);// 左下
		m_ppHoles[3] = new Hole(D3DXVECTOR3(w / 2, 50 + r, h / 2), r);// 右上
		m_ppHoles[4] = new Hole(D3DXVECTOR3(w / 2 + 4, 50 + r, 0), r);// 右中
		m_ppHoles[5] = new Hole(D3DXVECTOR3(w / 2, 50 + r, -h / 2), r);// 右下
	}
	
	// create arrow
	{
		m_pShooter->CreateArrowModel(pMeshManager);
		m_pShooter->CreateShooterWindow();
	}

	// create ball num
	{
		m_ppBallNums = new GameObject*[m_ballCount];
		for (int i = 0; i < m_ballCount; i++)
		{
			m_ppBallNums[i] = new GameObject();

			Transform* pTransform = new Transform();
			pTransform->m_position = m_ppBalls[i]->m_pTransform->m_position + D3DXVECTOR3(0,10,0);
			pTransform->m_angle = D3DXQUATERNION(0,1,0,0);
			pTransform->m_scale = D3DXVECTOR3(10, 10, 10);

			char dataName[20];
			wsprintf(dataName, "text_%d.fbx", i);
			MeshData* pMesh = pMeshManager->CreateMeshData(dataName);

			m_ppBallNums[i]->SetTransform(pTransform);
			m_ppBallNums[i]->SetMesh(pMesh);
		}
	}

	m_isCreatedModel = true;
	m_isPlayGame = true;
	m_pShooter->SetIsShoot(true);
	m_pShooter->SetIsShootAiming(true);
	m_pShooter->SetIsFreeMove(true);
}


void GameManager::PhysicsMoveToBall()
{
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }
		m_ppBalls[i]->UpdatePos();

		m_ppBallNums[i]->m_pTransform->m_position = m_ppBalls[i]->m_pTransform->m_position + +D3DXVECTOR3(0, 10, 0);
	}
}

void GameManager::Shoot(Audio* pAudio)
{
	m_pShooter->Shoot(pAudio);
}

void GameManager::BallPocket(int ballID, int pocketID, Audio* pAudio, Physics* pPhysics)
{
	pAudio->PlayCue(SE_BALL_FALL);

	char msg[30];
	wsprintfA(msg, "%d BALL POCKET TO %d HALL", ballID, pocketID);
	MessageBoxA(nullptr, msg, "BALL POCKET", MB_OK);
	m_ppBalls[ballID]->isRender = false;
	m_ppBalls[ballID]->DisableRigid();
	m_ppBallNums[ballID]->isRender = false;

	if (ballID == 8)
	{
		if (CheckWin())
		{
			if (m_turn == LOW_TURN)
			{
				MessageBoxA(nullptr, "LOW GROUP PLAYER WIN!!! ", "GAME OVER", MB_OK);
			}
			else
			{
				MessageBoxA(nullptr, "HIGH GROUP PLAYER WIN!!! ", "GAME OVER", MB_OK);
			}
		}
		else
		{
			m_ppBalls[8]->m_pTransform->m_position = D3DXVECTOR3(0, 100, 70);
			m_ppBalls[8]->isRender = true;
			m_ppBalls[8]->EnableRigid(pPhysics);
			m_ppBallNums[ballID]->isRender = true;
		}
		Reset();
	}
}


#pragma region collider
void GameManager::Collider(Audio* pAudio,Physics* pPhysics)
{
	BallToBallCollision(pAudio);
	BallToHaleCollision(pAudio, pPhysics);
	SetFirstTouchBall();
	SetTouchCushion();
}

bool GameManager::CollisionToGameObject(const GameObject * pObj1, const GameObject * pObj2)
{
	// 2つのオブジェクトの距離(2乗)
	D3DXVECTOR3 distSquared = pObj1->m_pTransform->m_position - pObj2->m_pTransform->m_position;
	distSquared.x = distSquared.x * distSquared.x;
	//distSquared.y = distSquared.y * distSquared.y;
	distSquared.z = distSquared.z * distSquared.z;

	// 2つのオブジェクトの半径の合計(2乗)
	float sumRadiusSquared = pObj1->m_radius + pObj2->m_radius;
	sumRadiusSquared = sumRadiusSquared * sumRadiusSquared;

	// 衝突しているか
	if (distSquared.x + distSquared.z <= sumRadiusSquared - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameManager::BallToBallCollision(Audio* pAudio)
{
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }
		for (int k = i + 1; k < m_ballCount; k++)
		{
			if (m_ppBalls[k]->isRender == false) { continue; }
			if (i == k) { continue; }
			if (CollisionToGameObject(m_ppBalls[i], m_ppBalls[k]))
			{
				pAudio->PlayCue(SE_BALL_HIT);
			}
		}
	}
}

void GameManager::BallToHaleCollision(Audio* pAudio, Physics* pPhysics)
{
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->isRender == false) { continue; }

		for (int k = 0; k < m_holeCount; k++)
		{
			if (CollisionToGameObject(m_ppBalls[i], m_ppHoles[k]))
			{
				BallPocket(i,k,pAudio,pPhysics);
			}
		}
	}
}

void GameManager::SetFirstTouchBall()
{
	if (m_firstTouchBallNum != 0) { return; }

	for (int i = 1; i < m_ballCount; i++)
	{
		if (CollisionToGameObject(m_ppBalls[0], m_ppBalls[i]))
		{
			m_firstTouchBallNum = i;
		}
	}
}

void GameManager::SetTouchCushion()
{
	if (m_touchCushion == true) { return; }
	
	for (int i = 0; i < m_ballCount; i++)
	{
		if (m_ppBalls[i]->m_pRigidBody->isSleeping()) { continue; }

		const float c_offset = 10;
		D3DXVECTOR3 pos = m_ppBalls[i]->m_pTransform->m_position;
		if (pos.x > TABLE_WIDTH / 2 - c_offset ||
			pos.x < TABLE_WIDTH / 2 * -1 + c_offset ||
			pos.z > TABLE_HEIGHT / 2 - c_offset ||
			pos.z < TABLE_HEIGHT / 2 * -1 + c_offset)
		{
			m_touchCushion = true;
		}
	}
}
#pragma endregion


void GameManager::Render(MeshImporter* pMeshManager)
{
	if (m_isPlayGame)
	{
		if (m_isCreatedModel)
		{
			// ball render
			for (int i = 0; i < m_ballCount; i++)
			{
				RenderGameObject(m_ppBalls[i], pMeshManager);
			}

			// table render
			RenderGameObject(m_pTable, pMeshManager);

			// shooter render
			m_pShooter->Render(pMeshManager);

			// ball num render
			for (int i = 0; i < m_ballCount; i++)
			{
				RenderGameObject(m_ppBallNums[i],pMeshManager);
			}
		}

		// text render
		ShowCommandText(10,20);
		ShowTurnText(10,120);
		ShowCurrentBallStateText(10,200);
	}
	else
	{
		// load text render
		ShowLoadText(10,20);
	}
}

void GameManager::RenderGameObject(const GameObject* pObj, MeshImporter* pMeshManager)
{
	if (!pObj) { return; }
	if (pObj->isRender == false) { return; }

	D3DXMATRIX world = pObj->m_pTransform->GetWorld();
	MeshData* pMeshData = pObj->m_pRootMesh;
	pMeshManager->RenderMesh(&world, pMeshData);
}
