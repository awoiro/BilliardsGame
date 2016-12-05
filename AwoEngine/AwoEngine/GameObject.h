#pragma once
#include "Transform.h"
#include "Physics.h"
#include "MeshData.h"

class GameObject
{
public:
	GameObject() {};
	/*
	GameObject(Transform* pTransform, MeshData* pRootMehs, Physics* pPhysics)
	{
		m_pTransform = pTransform;
		m_pRootMesh = pRootMehs;
		//m_pRigidBody = pPhysics->CreateDynamic(PxTransform(PxVec3(10, 10, -100)), PxSphereGeometry(10), PxVec3(0, 0, 0));
		//m_pRigidBody = pPhysics->CreateDynamic(PxTransform(PxVec3(pTransform->m_position.x, pTransform->m_position.y, pTransform->m_position.z)), PxSphereGeometry(57.1), PxVec3(0, 0, 0));

	}
	*/
	~GameObject()
	{
		delete m_pTransform;
	}

	virtual void SetTransform(Transform* pTransform) { m_pTransform = pTransform; }
	virtual void SetMesh(MeshData* pRootMeshData) { m_pRootMesh = pRootMeshData; }

	virtual void Update() {}

 	virtual void Render(MeshImporter* pMeshManager);

	Transform* m_pTransform = nullptr;
	MeshData* m_pRootMesh = nullptr;
protected:

};

class DynamicGameObject : public GameObject
{
public:

	DynamicGameObject() {};
	~DynamicGameObject() {};

	virtual void SetRigidDynamicSphere(Physics* pPhysics, int radius)
	{
		m_pRigidBody = pPhysics->CreateDynamic(PxTransform
			( PxVec3(m_pTransform->m_position.x, m_pTransform->m_position.y, m_pTransform->m_position.z))
			, PxSphereGeometry(radius)
			, PxVec3(0, 0, 0));
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
		m_ballNumber = ballNum;
	}
	~Ball() {};

	int m_ballNumber = -1;

private:

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
