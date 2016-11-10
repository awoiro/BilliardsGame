#include "Audio.h"

HRESULT Audio::Init()
{
	HRESULT result;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;

	//
	if (m_coInit == false) { IS_CHECK_ERROR(false, "COMが初期化されていません。"); return E_FAIL; }

	// エンジンの作成
	result = XACT3CreateEngine(2, &m_pXACTEngine);
#if false
	if (FAILED(result) || m_pXACTEngine == nullptr)
	{
		return E_FAIL;
	}
#else
	if (CHECK_ERROR(result, "XACTエンジンの作成に失敗"))
	{
		return E_FAIL;
	}
#endif

	// ランタイムの初期化と作成
	XACT_RUNTIME_PARAMETERS xactParams = { 0 };
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = m_pXACTEngine->Initialize(&xactParams);
	if (CHECK_ERROR(result, "XACT初期化失敗"))
	{
		return E_FAIL;
	}

	// ウェブバンクの読み込みと登録
	result = E_FAIL;
	hFile = CreateFileA(WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, nullptr);
		if (fileSize != -1)
		{
			hMapFile = CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, fileSize, nullptr);

			if (hMapFile)
			{
				m_pMapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				if (m_pMapWaveBank)
				{
					result = m_pXACTEngine->CreateInMemoryWaveBank(m_pMapWaveBank, fileSize, 0, 0, &m_pWaveBank);
				}
				CloseHandle(hMapFile);
			}
		}
		CloseHandle(hFile);
	}
	else
	{
		IS_CHECK_ERROR(false, "ウェブバンクデータのファイル取得失敗");
		return E_FAIL;
	}
	if (CHECK_ERROR(result, "ウェブバンクの登録失敗"))
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	// サウンドバンクの読み込みと登録
	result = E_FAIL;
	hFile = CreateFileA(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, nullptr);
		if (fileSize != -1)
		{
			m_pSoundBankData = new BYTE[fileSize];
			if (m_pSoundBankData)
			{
				if (0 != ReadFile(hFile, m_pSoundBankData, fileSize, &bytesRead, nullptr))
				{
					result = m_pXACTEngine->CreateSoundBank(m_pSoundBankData, fileSize, 0, 0, &m_pSoundBank);
				}
			}
		}
		CloseHandle(hFile);
	}
	else
	{
		IS_CHECK_ERROR(false, "サウンドバンクのファイル取得失敗");
		return E_FAIL;
	}
	if (CHECK_ERROR(result, "サウンドバンクの登録失敗"))
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	return S_OK;
}

void Audio::PlayCue(const char cue[])
{
	if (m_pSoundBank == nullptr)
	{
		MessageBoxA(nullptr, "not sound data", nullptr, 0);
		return;
	}

	m_cueIndex = m_pSoundBank->GetCueIndex(cue);
	m_pSoundBank->Play(m_cueIndex, 0, 0, nullptr);
}

void Audio::StopCue(const char cue[])
{
	if (m_pSoundBank == nullptr) { return; }

	m_cueIndex = m_pSoundBank->GetCueIndex(cue);
	m_pSoundBank->Stop(m_cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}