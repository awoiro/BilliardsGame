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
			pErrorMsg = "ファイルが見つかりませんでした。";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			pErrorMsg = "特定の種類のステート オブジェクトの一意のインスタンスが多すぎます。";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			pErrorMsg = "特定の種類のビュー オブジェクトの一意のインスタンスが多すぎます。";
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			pErrorMsg = "リソースごとの ID3D11Device::CreateDeferredContext の呼び出しまたは ID3D11DeviceContext::FinishCommandList の呼び出しの後で最初に呼び出した ID3D11DeviceContext::Map が D3D11_MAP_WRITE_DISCARD ではありませんでした。";
			break;
			/*
			case D3DERR_INVALIDCALL:
			pErrorMsg = "メソッドの呼び出しが無効です。たとえば、メソッドのパラメーターが有効なポインターでない可能性があります。";
			break;
			case D3DERR_WASSTILLDRAWING:
			pErrorMsg = "このサーフェスとの間で情報を転送している以前のビット演算が不完全です。";
			break;
			*/
		case E_FAIL:
			pErrorMsg = "デバッグ レイヤーを有効にしてデバイスを作成しようとしましたが、該当するレイヤーがインストールされていません。";
			break;
		case E_INVALIDARG:
			pErrorMsg = "戻り関数に無効なパラメーターが渡されました。";
			break;
		case E_OUTOFMEMORY:
			pErrorMsg = "Direct3D が呼び出しを完了するうえで十分なメモリーを割り当てることができませんでした。";
			break;
		case S_FALSE:
			pErrorMsg = "正常に処理されたものの、非標準の完了を示す代替成功値です (正確な意味はコンテキストによって異なります)。";
			break;
		case S_OK:
			pErrorMsg = "S_OK";
			//return false;
			break;
		default:
			pErrorMsg = "原因不明のエラー";
			break;
		}

		char message[500];
		//wsprintfA(message, "ファイル名 : %s\n行数 : %d\n%s\n%s", pFileName, lineNum, msg, pErrorMsg);
		LPSTR hrMsg = HresultToLPSTR(hr);
		//char resultMsg[] = { hrMsg };
		wsprintfA(message, "ファイル名 : %s\n行数 : %d\n%s\n%s%s", pFileName, lineNum, msg, hrMsg, pErrorMsg);
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
		wsprintfA(message, "ファイル名 : %s\n行数 : %d\n%s\n", pFileName, lineNum, msg);
		MessageBoxA(nullptr, message, nullptr, MB_ICONERROR);
		PostQuitMessage(0);
	}
}

