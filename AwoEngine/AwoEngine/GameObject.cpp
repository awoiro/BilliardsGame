#include "GameObject.h"

void GameObject::Render(MeshImporter* pMeshManager)
{
	D3DXMATRIX world = m_pTransform->GetWorld();
	pMeshManager->RenderMesh(&world, m_pRootMesh);
}