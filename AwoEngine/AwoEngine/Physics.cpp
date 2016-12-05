#include "Physics.h"

void Physics::StepPhysics()
{
	m_pScene->simulate(1.0f / 60.0f);
	m_pScene->fetchResults(true);
}

HRESULT Physics::Init()
{
	HRESULT hr = E_FAIL;

	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallBack);

	PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_pFoundation);
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, profileZoneManager);

	if (m_pPhysics->getPvdConnectionManager())
	{
		m_pPhysics->getVisualDebugger()->setVisualizeConstraints(true);
		m_pPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
		m_pPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		//m_pPhysics->getVisualDebugger()->updateCamera("camera", PxVec3(0, 0, 0), PxVec3(0, 0, 0), PxVec3(0, 1, 0));
		m_pConnection = PxVisualDebuggerExt::createConnection(m_pPhysics->getPvdConnectionManager(), "127.0.0.1", 5425, 10);
	}
	
	// ƒV[ƒ“‚Ìì¬
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_pScene = m_pPhysics->createScene(sceneDesc);

	// ƒ}ƒeƒŠƒAƒ‹‚Ìì¬
	//								[Ã‰^“®–€ŽC]  [“®‰^“®–€ŽC]  [”½”­ŒW”]
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// ƒOƒ‰ƒ“ƒh‚Ìì¬
	PxRigidStatic* pGroundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
	m_pScene->addActor(*pGroundPlane);

	// ƒ{[ƒ‹‚Ìì¬
	//PxRigidDynamic* pRigid = CreateDynamic(PxTransform(PxVec3(10, 10, -100)), PxSphereGeometry(10), PxVec3(0, 0, 0));
	//CreateDynamic(PxTransform(PxVec3(30, 30, -100)), PxSphereGeometry(10), PxVec3(0, 0, 0));
	//CreateDynamic(PxTransform(PxVec3(50, 50, -100)), PxSphereGeometry(10), PxVec3(0, 0, 0));

	// •s“®ŽlŠpŒ`‚Ìì¬
	//CreateStatic(PxTransform(PxVec3(0, 0, 0)), PxBoxGeometry(5, 4, 3), PxVec3(0, 0, 0));

	return S_OK;
}

void Physics::Shutdown()
{
	//PX_UNUSED(bool)
	m_pScene->release();
	m_pDispatcher->release();
	PxProfileZoneManager* profileZoneManager = m_pPhysics->getProfileZoneManager();
	if (m_pConnection != nullptr) { m_pConnection->release(); }
	m_pPhysics->release();
	profileZoneManager->release();
	m_pFoundation->release();
}

PxRigidDynamic * Physics::CreateDynamic(const PxTransform & t, const PxGeometry & geometry, const PxVec3 & velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_pPhysics, t, geometry, *m_pMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_pScene->addActor(*dynamic);
	return dynamic;
}

/*
PxTriangleMesh Physics::createTriangleMesh()
{
	return nullptr;
}
*/

PxRigidStatic * Physics::CreateStatic(const PxTransform & t, const PxGeometry & geometry, const PxVec3 & velocity)
{
	//PxRigidStatic* pStatic = PxCreateStatic(*m_pPhysics, t, geometry, *m_pMaterial, 10.0f);
	PxRigidStatic* pStatic = m_pPhysics->createRigidStatic(t);
	PxShape* pShape = pStatic->createShape(geometry, *m_pMaterial);
	PX_UNUSED(pShape);
	m_pScene->addActor(*pStatic);
	return pStatic;
}

void Physics::UpdatePVDCamera()
{
#if false
	if (m_pPhysics->getPvdConnectionManager())
	{
		// name orizin up target
		m_pPhysics->getVisualDebugger()->updateCamera("camera", PxVec3(0,0,-3500), PxVec3(0,0,0), PxVec3(0,1,0));
	}
#endif
}