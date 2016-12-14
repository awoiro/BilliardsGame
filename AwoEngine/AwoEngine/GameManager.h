#pragma once

#include <AntTweakBar.h>
#include "Shooter.h"

#define TABLE_WIDTH 127
#define TABLE_HEIGHT 254
#define BALL_RADIUS 5.71/2
#define COLLIDER_SIZE 50

class GameManager
{
public:
	GameManager()
		: m_isPlayGame(false)
		, m_turn(LOW_TURN)
		, m_firstTouchBallNum(0)
		, m_touchCushion(false)
		, m_isCreatedModel(false)
		, m_pShooter(nullptr)
		, m_ppBalls(nullptr)
		, m_ballCount(0)
		, m_pTable(nullptr)
		, m_ppHoles(nullptr)
		, m_holeCount(0)
	{
		m_pShooter = new Shooter();
	};
	~GameManager()
	{
		delete[] m_ppHoles;
		delete m_pTable;
		delete[] m_ppBalls;
		delete m_pShooter;
	};

	// reset
	void Reset();

	// updater
	void Update(DeviceManager* pDeviceManager, MeshImporter* pMeshManager, Physics* pPhysics, Audio* pAudio);

	// checcker
	bool CheckBallStop() const;
	bool CheckIsShootAiming() const;
	bool CheckFoul() const;
	bool CheckWin() const;
	bool CheckIsCreateModel() const;

	// shooer change
	void PlayerChange();
	void PenaltyPlayerChange();

	// create model
	void CreateModels(DeviceManager* pDeviceManager, MeshImporter* pMeshManager, Physics* pPhysics);

	// move
	void PhysicsMoveToBall();

	// shoot
	void Shoot(Audio* pAudio);

	// pocket
	void BallPocket(int ballID, int pocketID, Audio* pAudio);

	// collisions
	void Collider(Audio* pAudio);

	// render
	void Render(MeshImporter* pMeshManager);

private:
	// text message
	void TwRenderText(const int x, const int y, const char* msg);
	void ShowLoadText(const int x, const int y);// +20
	void ShowCommandText(const int x, const int y);// +80
	void ShowTurnText(const int x, const int y);// +40
	void ShowCurrentBallStateText(const int x, const int y);// +360

	// collisions
	bool CollisionToGameObject(const GameObject* pObj1, const GameObject* pObj2);
	void BallToBallCollision(Audio* pAudio);
	void BallToHaleCollision(Audio* pAudio);
	void SetFirstTouchBall();
	void SetTouchCushion();

	// render
	void RenderGameObject(const GameObject* pObj, MeshImporter* pMeshManager);
	
	// checkker
	bool m_isPlayGame;
	bool m_isCreatedModel;


	// turn
	enum TURN
	{
		LOW_TURN = 0,
		HIGH_TURN
	};
	TURN m_turn;

	// Foul checkker
	int m_firstTouchBallNum;
	bool m_touchCushion;
	
	// shooter
	Shooter* m_pShooter;

	// Balls
	Ball** m_ppBalls;
	int m_ballCount;

	// Table
	Table* m_pTable;

	// Table holes
	Hole** m_ppHoles;
	int m_holeCount;
};

