#pragma once
#include "Transform.h"
#include "Physics.h"
#include "MeshData.h"

class GameObject
{
public:
	GameObject(Transform* pTransform, MeshData* pRootMehs, Physics* pPhysics)
	{
		m_pTransform = pTransform;
		m_pRootMesh = pRootMehs;
		//m_pRigidBody = pPhysics->createDynamic(PxTransform(PxVec3(10, 10, -100)), PxSphereGeometry(10), PxVec3(0, 0, 0));
		m_pRigidBody = pPhysics->createDynamic(PxTransform(PxVec3(pTransform->m_position.x, pTransform->m_position.y, pTransform->m_position.z)), PxSphereGeometry(57.1), PxVec3(0, 0, 0));

	}
	~GameObject()
	{
		delete m_pTransform;
	}
	void Update()
	{
		
	}
	void Render(MeshImporter* pMeshManager);
	void UpdatePos()
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
	Transform* m_pTransform = nullptr;
	MeshData* m_pRootMesh = nullptr;
	PxRigidDynamic* m_pRigidBody = nullptr;

protected:

};

