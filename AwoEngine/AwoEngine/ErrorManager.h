#pragma once
#include <Windows.h>
#include <iostream>

#define CHECK_ERROR(hr,msg) HresultErrorCheck(hr,msg,__LINE__,__FILE__)
#define IS_CHECK_ERROR(isCheck,msg) SimpleErrorMessage(isCheck,msg,__LINE__,__FILE__);

static LPSTR HresultToLPSTR(HRESULT hr)
{
	LPVOID textMsg = nullptr;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		, nullptr
		, hr
		, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		, (LPSTR)&textMsg
		, 0
		, nullptr);
	return (LPSTR)textMsg;
}

inline bool HresultErrorCheck(HRESULT hr, LPSTR msg, int lineNum, char* pFileName)
{
	if (FAILED(hr))
	{
		char* pErrorMsg = nullptr;

		switch (hr)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			pErrorMsg = "�t�@�C����������܂���ł����B";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			pErrorMsg = "����̎�ނ̃X�e�[�g �I�u�W�F�N�g�̈�ӂ̃C���X�^���X���������܂��B";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			pErrorMsg = "����̎�ނ̃r���[ �I�u�W�F�N�g�̈�ӂ̃C���X�^���X���������܂��B";
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			pErrorMsg = "���\�[�X���Ƃ� ID3D11Device::CreateDeferredContext �̌Ăяo���܂��� ID3D11DeviceContext::FinishCommandList �̌Ăяo���̌�ōŏ��ɌĂяo���� ID3D11DeviceContext::Map �� D3D11_MAP_WRITE_DISCARD �ł͂���܂���ł����B";
			break;
			/*
			case D3DERR_INVALIDCALL:
			pErrorMsg = "���\�b�h�̌Ăяo���������ł��B���Ƃ��΁A���\�b�h�̃p�����[�^�[���L���ȃ|�C���^�[�łȂ��\��������܂��B";
			break;
			case D3DERR_WASSTILLDRAWING:
			pErrorMsg = "���̃T�[�t�F�X�Ƃ̊Ԃŏ���]�����Ă���ȑO�̃r�b�g���Z���s���S�ł��B";
			break;
			*/
		case E_FAIL:
			pErrorMsg = "�f�o�b�O ���C���[��L���ɂ��ăf�o�C�X���쐬���悤�Ƃ��܂������A�Y�����郌�C���[���C���X�g�[������Ă��܂���B";
			break;
		case E_INVALIDARG:
			pErrorMsg = "�߂�֐��ɖ����ȃp�����[�^�[���n����܂����B";
			break;
		case E_OUTOFMEMORY:
			pErrorMsg = "Direct3D ���Ăяo�����������邤���ŏ\���ȃ������[�����蓖�Ă邱�Ƃ��ł��܂���ł����B";
			break;
		case S_FALSE:
			pErrorMsg = "����ɏ������ꂽ���̂́A��W���̊�����������֐����l�ł� (���m�ȈӖ��̓R���e�L�X�g�ɂ���ĈقȂ�܂�)�B";
			break;
		case S_OK:
			pErrorMsg = "S_OK";
			//return false;
			break;
		default:
			pErrorMsg = "�����s���̃G���[";
			break;
		}

		char message[500];
		//wsprintfA(message, "�t�@�C���� : %s\n�s�� : %d\n%s\n%s", pFileName, lineNum, msg, pErrorMsg);
		LPSTR hrMsg = HresultToLPSTR(hr);
		//char resultMsg[] = { hrMsg };
		wsprintfA(message, "�t�@�C���� : %s\n�s�� : %d\n%s\n%s%s", pFileName, lineNum, msg, hrMsg, pErrorMsg);
		MessageBoxA(nullptr, message, nullptr, MB_ICONERROR);
		PostQuitMessage(0);

		return true;
	}
	else
	{
		return false;
	}
}

inline void SimpleErrorMessage(bool isCheck, LPSTR msg, int lineNum, char* pFileName)
{
	if (isCheck == false)
	{
		char message[500];
		wsprintfA(message, "�t�@�C���� : %s\n�s�� : %d\n%s\n", pFileName, lineNum, msg);
		MessageBoxA(nullptr, message, nullptr, MB_ICONERROR);
		PostQuitMessage(0);
	}
}

