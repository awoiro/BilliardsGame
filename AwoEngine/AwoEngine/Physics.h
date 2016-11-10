#pragma once

#include <PxPhysicsAPI.h>
#include "ErrorManager.h"

#pragma comment (lib,"PhysX3CommonDEBUG_x86.lib" )
#pragma comment (lib,"PhysX3DEBUG_x86.lib" )
#pragma comment (lib,"PhysX3CookingDEBUG_x86.lib" )
#pragma comment (lib,"PhysX3CharacterKinematicDEBUG_x86.lib" )
#pragma comment (lib,"PhysX3ExtensionsDEBUG.lib" )
#pragma comment (lib,"PhysX3VehicleDEBUG.lib" )
#pragma comment (lib,"PhysXProfileSDKDEBUG.lib" )
#pragma comment (lib,"PhysXVisualDebuggerSDKDEBUG.lib" )
#pragma comment (lib,"PxTaskDEBUG.lib" )

using namespace physx;

class Physics
{
public:
	Physics()
		: m_pFoundation(nullptr)
		, m_pPhysics(nullptr)
	{
		Init();
	};
	~Physics()
	{
		Shutdown();
	};

private:

	HRESULT Init();
	void Shutdown();

	PxDefaultAllocator		m_allocator;
	PxDefaultErrorCallback	m_errorCallBack;

	PxFoundation*			m_pFoundation = nullptr;
	PxPhysics*				m_pPhysics = nullptr;

	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	PxScene*				m_pScene = nullptr;

	PxMaterial*				m_pMaterial = nullptr;

	PxVisualDebuggerConnection* m_pConnection = nullptr;

	PxReal stackZ = 10.0f;
};

#pragma region Collider Class

class Collider
{
public:
	Collider() {};
	~Collider() {};
protected:
	virtual HRESULT Create() = 0;
private:
};

class BoxCollider : Collider
{
public:
	BoxCollider() {};
	~BoxCollider() {};
protected:
	virtual HRESULT Create() override {};
private:
};

class SphereCollider : Collider
{
public:
	SphereCollider() {};
	~SphereCollider() {};
protected:
	virtual HRESULT Create() override {};
private:
};

#pragma endregion

Physics* g_pPhysics = new Physics();