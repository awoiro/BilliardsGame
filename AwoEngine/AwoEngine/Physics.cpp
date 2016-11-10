#include "Physics.h"

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