#pragma once

#include <dinput.h>
#include <D3DX10.h>
#include <vector>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Input
{
public:
	Input()
		: m_directInput(nullptr)
		, m_keyboardDevice(nullptr)
	{
		Init();
		m_currentKeyDownList.clear();
		m_beforeKeyDownList.clear();
		m_currentMouseKeyDownList.clear();
		m_beforeMouseKeyDownList.clear();
	};
	~Input()
	{
		SAFE_RELEASE(m_mouseDevice);
		SAFE_RELEASE(m_keyboardDevice);
		SAFE_RELEASE(m_directInput);
	};

	HRESULT Input::Init();

	// �f�o�C�X
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_keyboardDevice;
	LPDIRECTINPUTDEVICE8 m_mouseDevice;

	// �L�[�{�[�h�C���v�b�g
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);
	void UpdateKeyCode();

	// �}�E�X�C���v�b�g
	enum MOUSE_BUTTON
	{
		MOUSE_BUTTON_LEFT = 0,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_CENTER,
	};
	bool GetMouseButton(MOUSE_BUTTON mouseButton);
	bool GetMouseButtonDown(MOUSE_BUTTON mouseButton);
	bool GetMouseButtonUp(MOUSE_BUTTON mouseButton);
	D3DXVECTOR2 GetMousePos(float width, float height, HWND hwnd);
	void UpdateMouseCode();

private:
	// �L�[�{�[�h
	std::vector<int> m_currentKeyDownList;
	std::vector<int> m_beforeKeyDownList;
	bool CheckHasKey(std::vector<int> keyDownList, int checkKeyCode); // �I�������L�[�����X�g�ɑ��݂��Ă��邩���`�F�b�N
	bool CheckKeyDown(int keyCode); // �I�������L�[��������Ă��邩���`�F�b�N
	HRESULT InitKeyboardDevice();

	// �}�E�X
	std::vector<int> m_currentMouseKeyDownList;
	std::vector<int> m_beforeMouseKeyDownList;
	bool CheckHasMouseKey(std::vector<int> mouseDownList, MOUSE_BUTTON mouseButton);
	bool CheckMouseKeyDown(MOUSE_BUTTON mouseButton);
	HRESULT InitMouseDeviec();
};
