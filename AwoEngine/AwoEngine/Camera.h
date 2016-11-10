#pragma once
#include "DeviceManager.h"

struct ShaderGlobal_Camera
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
	// lookVector
	// sideVector
	// upVector
	// nearPlane
	// farPlane
};


class Camera
{
public:
	Camera() {};
	~Camera() {};
};

