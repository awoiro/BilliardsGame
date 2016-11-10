#include "Input.h"

HRESULT Input::Init()
{
	HRESULT hr = E_FAIL;

	// DirectInputの作成
	hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	// CHECKERROR()
	if (FAILED(hr)) { return E_FAIL; }

	// デバイスの設定
	InitKeyboardDevice();
	InitMouseDeviec();

	return S_OK;
}

HRESULT Input::InitKeyboardDevice()
{
	HRESULT hr = E_FAIL;
	// デバイスの作成
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, nullptr);
	// CHECKERROR()
	if (FAILED(hr)) { return E_FAIL; }

	// キーボードに設定
	hr = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	// CHECKERROR
	if (FAILED(hr)) { return E_FAIL; }

	// 協調レベル設定
	hr = m_keyboardDevice->SetCooperativeLevel(nullptr, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	// CHECKERROR
	if (FAILED(hr)) { return E_FAIL; }

	// デバイスを取得
	m_keyboardDevice->Acquire();

	return S_OK;
}
HRESULT Input::InitMouseDeviec()
{
	HRESULT hr = E_FAIL;
	// デバイスの作成
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, nullptr);
	// CHECKERROR()
	if (FAILED(hr)) { return E_FAIL; }

	// マウスに設定
	hr = m_mouseDevice->SetDataFormat(&c_dfDIMouse2);
	// CHECKERROR
	if (FAILED(hr)) { return E_FAIL; }

	// 協調レベル設定
	hr = m_mouseDevice->SetCooperativeLevel(nullptr, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//hr = m_mouseDevice->SetCooperativeLevel(nullptr, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	// CHECKERROR
	if (FAILED(hr)) { return E_FAIL; }

	// デバイスを取得
	m_mouseDevice->Acquire();

	return S_OK;
}

#pragma region Keyboard

bool Input::GetKey(int keyCode)
{
	bool isKeyDown = CheckKeyDown(keyCode);

	if (isKeyDown)
	{
		if (CheckHasKey(m_currentKeyDownList, keyCode) == false)
		{
			m_currentKeyDownList.push_back(keyCode);
		}
	}

	return isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	bool isKeyDown = CheckKeyDown(keyCode);

	if (isKeyDown)
	{
		if (CheckHasKey(m_currentKeyDownList, keyCode) == false)
		{
			m_currentKeyDownList.push_back(keyCode);
		}
	}
	else
	{
		return false;
	}

	//CheckHasKey(m_beforeKeyDownList, keyCode) == false ? true : false;
	if (CheckHasKey(m_beforeKeyDownList, keyCode) == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::GetKeyUp(int keyCode)
{
	bool isKeyDown = CheckKeyDown(keyCode);

	if (isKeyDown)
	{
		if (CheckHasKey(m_currentKeyDownList, keyCode) == false)
		{
			m_currentKeyDownList.push_back(keyCode);
		}
		return false;
	}

	if ((CheckHasKey(m_beforeKeyDownList, keyCode) == true) &&
		(CheckHasKey(m_currentKeyDownList, keyCode) == false))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::CheckHasKey(std::vector<int> keyDownList, int checkKeyCode)
{
	for (unsigned i = 0; i < keyDownList.size(); i++)
	{
		if (keyDownList[i] == checkKeyCode)
		{
			return true;
		}
	}
	return false;
}

bool Input::CheckKeyDown(int keyCode)
{
	HRESULT hr = m_keyboardDevice->Acquire();
	if (hr == DI_OK || hr == S_FALSE)
	{
		BYTE diks[256];
		m_keyboardDevice->GetDeviceState(sizeof(diks), &diks);
		if (diks[keyCode] & 0x80)
		{
			return true;
		}
	}
	return false;
}

void Input::UpdateKeyCode()
{
	m_beforeKeyDownList = m_currentKeyDownList;
	m_currentKeyDownList.clear();
}

#pragma endregion

#pragma region Mouse
bool Input::GetMouseButton(MOUSE_BUTTON mouseButton)
{
	if (CheckMouseKeyDown(mouseButton))
	{
		return true;
	}
	return false;
}

bool Input::GetMouseButtonDown(MOUSE_BUTTON mouseButton)
{
	// before:F current:T

	bool isMouseKeyDown = CheckMouseKeyDown(mouseButton);

	if (isMouseKeyDown)
	{
		if (CheckHasMouseKey(m_currentMouseKeyDownList, mouseButton) == false)
		{
			m_currentMouseKeyDownList.push_back(mouseButton);
		}
	}
	else
	{
		return false;
	}

	if (CheckHasMouseKey(m_beforeMouseKeyDownList, mouseButton) == false)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool Input::GetMouseButtonUp(MOUSE_BUTTON mouseButton)
{
	// before:T	current:F

	bool isMouseKeyDown = CheckMouseKeyDown(mouseButton);

	if (isMouseKeyDown)
	{
		if (CheckHasMouseKey(m_currentMouseKeyDownList, mouseButton) == false)
		{
			m_currentMouseKeyDownList.push_back(mouseButton);
		}
		return false;
	}

	if ((CheckHasMouseKey(m_beforeMouseKeyDownList, mouseButton) == true) &&
		(CheckHasMouseKey(m_currentMouseKeyDownList, mouseButton) == false))
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

D3DXVECTOR2 Input::GetMousePos(float width, float height, HWND hwnd)
{
#if false
	DIMOUSESTATE2 dims = { 0 };
	if (FAILED(m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
	{
		m_mouseDevice->Acquire();
		return D3DXVECTOR2();
	}

	//マウスの動き（カーソルの位置）を表示
	static float fPosX = 0, fPosY = 0;
	fPosX += dims.lX;
	fPosY += dims.lY;

	if (fPosX < 0)
	{
		fPosX = 0;
	}
	else if (fPosX > width)
	{
		fPosX = width;
	}
	if (fPosY < 0)
	{
		fPosY = 0;
	}
	else if (fPosY > height)
	{
		fPosY = height;
	}

	HDC hdc = GetDC(hwnd);
	RECT rect = { 10,10,1000,50 };
	char info[100] = { 0 };
	//std::cout << "XPos:" << fPosX << "YPos:" << fPosY << std::endl;
	//sprintf(info, "posX=%f posY=%f", fPosX, fPosY);
	//DrawTextA(hdc, info, strlen(info), &rect, 0);

	D3DXVECTOR2 outValue = D3DXVECTOR2(fPosX, fPosY);
	return outValue;
#else

	// マウス座標
	POINT pt;
	//マウスの現在の座標を取得する
	GetCursorPos(&pt);
	
	// D3DXVector2型に変換
	D3DXVECTOR2 outValue;
	outValue.x = pt.x;
	outValue.y = pt.y;

	return outValue;

#endif
}

bool Input::CheckMouseKeyDown(MOUSE_BUTTON mouseButton)
{
	DIMOUSESTATE2 dims = { 0 };
	if (FAILED(m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
	{
		m_mouseDevice->Acquire();
		return false;
	}

	// 入力判定
	if (dims.rgbButtons[mouseButton] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::CheckHasMouseKey(std::vector<int> mouseDownList, MOUSE_BUTTON mouseButton)
{
	for (unsigned i = 0; i < mouseDownList.size(); i++)
	{
		if (mouseDownList[i] == (int)mouseButton)
		{
			return true;
		}
	}
	return false;
}

void Input::UpdateMouseCode()
{
	m_beforeMouseKeyDownList = m_currentMouseKeyDownList;
	m_currentMouseKeyDownList.clear();
}

#pragma endregion