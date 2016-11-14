#pragma once
#include "DeviceManager.h"

class Transform
{
public:
	Transform() {};
	Transform(D3DXVECTOR3 pos, D3DXQUATERNION angle, D3DXVECTOR3 scale)
	{
		m_position = pos;
		m_angle = angle;
		m_scale = scale;
	}

	D3DXVECTOR3 m_position = D3DXVECTOR3(0, 0, 0);
	D3DXQUATERNION m_angle = D3DXQUATERNION(0, 0, 0, 1);//0,0,0,1
	D3DXVECTOR3 m_scale = D3DXVECTOR3(1,1,1);//1000,1000,1000

	D3DXMATRIX GetWorld()
	{
		D3DXMATRIX matRotation;
		D3DXMatrixRotationQuaternion(&matRotation, &m_angle);

		D3DXMATRIX matScaling;
		D3DXMatrixScaling(&matScaling, m_scale.x, m_scale.y, m_scale.z);

		D3DXMATRIX matTranslate;
		D3DXMatrixTranslation(&matTranslate, m_position.x, m_position.y, m_position.z);

		return matScaling * matRotation * matTranslate;
	}
};
