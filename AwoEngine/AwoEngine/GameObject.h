#pragma once
#include "Transform.h"
#include "Physics.h"
#include "MeshData.h"

class GameObject
{
public:
	GameObject() {};

	~GameObject()
	{
		delete m_pRootMesh;
		delete m_pTransform;
	}

	virtual void SetTransform(Transform* pTransform) { m_pTransform = pTransform; }
	virtual void SetMesh(MeshData* pRootMeshData) { m_pRootMesh = pRootMeshData; }

	virtual void Update() {}

	virtual void Render(MeshImporter* pMeshManager)
	{
		D3DXMATRIX world = m_pTransform->GetWorld();
		pMeshManager->RenderMesh(&world, m_pRootMesh);
	}

	Transform* m_pTransform = nullptr;
	MeshData* m_pRootMesh = nullptr;

	bool isRender = true;
	int m_radius = 0;
protected:

};

class DynamicGameObject : public GameObject
{
public:

	DynamicGameObject() {};
	~DynamicGameObject() 
	{
		//SAFE_RELEASE(m_pRigidBody);
	};

	virtual void SetRigidDynamicSphere(Physics* pPhysics, int radius)
	{
		PxVec3 pos = PxVec3(m_pTransform->m_position.x, m_pTransform->m_position.y, m_pTransform->m_position.z);
		PxQuat rot = PxQuat(m_pTransform->m_angle.x, m_pTransform->m_angle.y, m_pTransform->m_angle.z, m_pTransform->m_angle.w);
		m_pRigidBody = pPhysics->CreateDynamic(PxTransform
			( pos,rot )
			, PxSphereGeometry(radius)
			, PxVec3(0, 0, 0));

		m_radius = radius;
	}
	//virtual void SetRigidDynamicBox() {};

	virtual void UpdatePos()
	{
		PxTransform targetTransform = m_pRigidBody->getGlobalPose();
		m_pTransform->m_position.x = targetTransform.p.x;
		m_pTransform->m_position.y = targetTransform.p.y;
		m_pTransform->m_position.z = targetTransform.p.z;
		m_pTransform->m_angle.x = targetTransform.q.x;
		m_pTransform->m_angle.y = targetTransform.q.y;
		m_pTransform->m_angle.z = targetTransform.q.z;
		m_pTransform->m_angle.w = targetTransform.q.w;
	}

	virtual void DisableRigid()
	{
		m_pRigidBody->release();
	}

	virtual void EnableRigid(Physics* pPhysics)
	{
		m_pRigidBody = pPhysics->CreateDynamic(PxTransform
		(PxVec3(m_pTransform->m_position.x, m_pTransform->m_position.y, m_pTransform->m_position.z))
			, PxSphereGeometry(m_radius)
			, PxVec3(0, 0, 0));
	}

	PxRigidDynamic* m_pRigidBody = nullptr;
protected:
};

class StaticGameObject : public GameObject
{
public:
	StaticGameObject() {};
	~StaticGameObject() {};

	//virtual void SetRigidStaticSphere() {};
	virtual void SetRigidStaticBox(Physics* pPhysics, D3DXVECTOR3 extents) 
	{
		pPhysics->CreateStatic(PxTransform
			( PxVec3(m_pTransform->m_position.x, m_pTransform->m_position.y, m_pTransform->m_position.z))
			, PxBoxGeometry(extents.x / 2, extents.y / 2, extents.z / 2)
			, PxVec3(0, 0, 0));
	}

protected:
	//PxRigidStatic* m_pRigidStatic = nullptr;
};

class Ball : public DynamicGameObject
{
public:
	Ball() {};
	Ball(MeshData* pMeshData, D3DXVECTOR3 pos, D3DXQUATERNION rot,Physics* pPhysics, int radius, int ballNum)
	{
		Transform* pTransform = new Transform();
		pTransform->m_position = pos;
		pTransform->m_angle = rot;
		pTransform->m_scale = D3DXVECTOR3(1, 1, 1);

		SetTransform(pTransform);
		SetMesh(pMeshData);
		SetRigidDynamicSphere(pPhysics, radius);
		m_radius = radius;
		m_ballNumber = ballNum;
	}
	~Ball() {};

	//int m_radius;
	int m_ballNumber = -1;

private:

};

class Hole : public GameObject
{
public:
	Hole() {};
	Hole(D3DXVECTOR3 pos, int radius)
	{
		Transform* pTransform = new Transform();
		pTransform->m_position = pos;
		pTransform->m_angle = D3DXQUATERNION(0, 0, 0, 1);
		pTransform->m_scale = D3DXVECTOR3(1, 1, 1);
		SetTransform(pTransform);
		m_radius = radius;
	}
	~Hole() {};
	//int m_radius;
};

class Table : public StaticGameObject
{
public:
	Table() {};
	Table(MeshData* pMeshData, D3DXVECTOR3 pos, D3DXQUATERNION rot)
	{
		Transform* pTransform = new Transform();
		pTransform->m_position = pos;
		pTransform->m_angle = rot;
		pTransform->m_scale = D3DXVECTOR3(1, 1, 1);
		
		SetTransform(pTransform);
		SetMesh(pMeshData);
	}
	~Table() {};

	void AddRigidStatic(Physics *pPhysics, D3DXVECTOR3 offset, D3DXVECTOR3 extents)
	{
		pPhysics->CreateStatic(PxTransform
		(PxVec3(m_pTransform->m_position.x + offset.x, m_pTransform->m_position.y + offset.y, m_pTransform->m_position.z + offset.z))
			, PxBoxGeometry(extents.x / 2, extents.y / 2, extents.z / 2)
			, PxVec3(0, 0, 0));
	}

private:

};
