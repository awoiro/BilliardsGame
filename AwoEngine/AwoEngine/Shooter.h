#pragma once

#include <AntTweakBar.h>
#include "GameObject.h"
#include "MeshData.h"
#include "Audio.h"

class Shooter
{
public:

	Shooter()
		: m_isFreeMove(true)
		, m_isKitchenMove(false)
		, m_isShoot(false)
		, m_isShootAiming(false)
		, m_shootAngle(0)
		, m_shootPower(50)
		, m_pParentBall(nullptr)
		, m_pArrow(nullptr)
		, m_pBar(nullptr)
	{}

	~Shooter()
	{
		//delete m_pBar;
		delete m_pArrow;
		delete m_pParentBall;
	}


	void CreateArrowModel(MeshImporter* pMeshManager)
	{
		m_pArrow = new GameObject();

		Transform* pTransform = new Transform();
		pTransform->m_position = m_pParentBall->m_pTransform->m_position;

		D3DXMATRIX matRotation = D3DXMATRIX();
		D3DXMatrixRotationY(&matRotation, m_shootAngle * DIG_TO_RAD);
		D3DXQUATERNION autoRotationValue = D3DXQUATERNION();
		D3DXQuaternionRotationMatrix(&autoRotationValue, &matRotation);
		pTransform->m_angle = autoRotationValue;
		pTransform->m_scale = D3DXVECTOR3(7, 7, 7);
		m_pArrow->SetTransform(pTransform);

		m_pArrow->SetMesh(pMeshManager->CreateMeshData("arrow.fbx"));
	}

	void CreateShooterWindow()
	{
		m_pBar = TwNewBar("ShooterSetting");
		TwDefine("ShooterSetting position='410 40' size='200 320'");
		TwAddVarRO(m_pBar, "CanShoot", TW_TYPE_BOOL8, &m_isShoot, "");
		TwAddVarRO(m_pBar, "CanFreeMove", TW_TYPE_BOOL8, &m_isFreeMove, "");
		TwAddVarRO(m_pBar, "CanKitchenMove", TW_TYPE_BOOL8, &m_isKitchenMove, "");
		TwAddVarRO(m_pBar, "ArrowQuat", TW_TYPE_QUAT4F, &m_pArrow->m_pTransform->m_angle, "");
		TwDefine(" ShooterSetting/ArrowQuat opened=true ");
		TwAddVarRW(m_pBar, "ShootingAngle", TW_TYPE_FLOAT, &m_shootAngle, "");
		TwAddVarRW(m_pBar, "ShootingPower", TW_TYPE_FLOAT, &m_shootPower, "");
		TwAddVarRW(m_pBar, "ArrowSize", TW_TYPE_DIR3F, &m_pArrow->m_pTransform->m_scale, "");
		TwDefine(" ShooterSetting/ArrowSize opened=true ");
	}

	void MoveShootingPosition()
	{
		if (m_isShoot == false) { return; }

		if (m_isFreeMove)
		{
			// todo : 自由移動処理
		}
		else if(m_isKitchenMove)
		{
			// todo : キッチン内移動処理
		}
	}

	void UpdateArrowLocation()
	{
		m_pArrow->m_pTransform->m_position = m_pParentBall->m_pTransform->m_position;

		D3DXMATRIX matRotation;
		D3DXMatrixRotationY(&matRotation, m_shootAngle * DIG_TO_RAD);
		D3DXQUATERNION autoRotationValue;
		D3DXQuaternionRotationMatrix(&autoRotationValue, &matRotation);
		m_pArrow->m_pTransform->m_angle = autoRotationValue;
	}

	void Render(MeshImporter* pMeshManager)
	{
		if (m_pParentBall->isRender == false) { return; }

		D3DXMATRIX world = m_pArrow->m_pTransform->GetWorld();
		MeshData* pMeshData = m_pArrow->m_pRootMesh;
		pMeshManager->RenderMesh(&world, pMeshData);
	}

	void Shoot(Audio* pAudio)
	{
		if (m_isShoot == false) { return; }
		if (m_isShootAiming == false) { return; }

		// play SE
		pAudio->PlayCue(SE_BALL_HIT);

		// add velocity
		D3DXVECTOR3 vec = D3DXVECTOR3(0, 0, 0);
		vec.x = sinf(m_shootAngle * DIG_TO_RAD);
		vec.z = cosf(m_shootAngle * DIG_TO_RAD);
		D3DXVECTOR3 shootVec = D3DXVECTOR3();
		D3DXVec3Normalize(&shootVec, &vec);
		shootVec *= m_shootPower;
		m_pParentBall->m_pRigidBody->setLinearVelocity(PxVec3(shootVec.x, shootVec.y, shootVec.z));
	
		// checcker reset
		m_isShoot = false;
		m_isFreeMove = false;
		m_isKitchenMove = false;
		m_isShootAiming = false;
	}

	inline void SetParentBall(Ball* pBall) { m_pParentBall = pBall; }
	inline void SetIsFreeMove(const bool isFreeMove) { m_isFreeMove = isFreeMove; }
	inline void SetIsKitchenMove(const bool isKitchenMove) { m_isKitchenMove = isKitchenMove; }
	inline void SetIsShoot(const bool isShoot) { m_isShoot = isShoot; }
	inline void SetIsShootAiming(const bool isBallStop) { m_isShootAiming = isBallStop; }
	inline bool GetIsShootAiming() const { return m_isShootAiming; }

private:
	// checkker
	bool m_isFreeMove;
	bool m_isKitchenMove;
	bool m_isShoot;
	bool m_isShootAiming;

	// status
	float m_shootAngle;
	float m_shootPower;

	// model
	Ball* m_pParentBall;
	GameObject* m_pArrow;
	TwBar* m_pBar;
};

