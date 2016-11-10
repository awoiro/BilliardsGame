#include "Audio.h"

HRESULT Audio::Init()
{
	HRESULT result;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;

	//
	if (m_coInit == false) { IS_CHECK_ERROR(false, "COM������������Ă��܂���B"); return E_FAIL; }

	// �G���W���̍쐬
	result = XACT3CreateEngine(2, &m_pXACTEngine);
#if false
	if (FAILED(result) || m_pXACTEngine == nullptr)
	{
		return E_FAIL;
	}
#else
	if (CHECK_ERROR(result, "XACT�G���W���̍쐬�Ɏ��s"))
	{
		return E_FAIL;
	}
#endif

	// �����^�C���̏������ƍ쐬
	XACT_RUNTIME_PARAMETERS xactParams = { 0 };
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = m_pXACTEngine->Initialize(&xactParams);
	if (CHECK_ERROR(result, "XACT���������s"))
	{
		return E_FAIL;
	}

	// �E�F�u�o���N�̓ǂݍ��݂Ɠo�^
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
		IS_CHECK_ERROR(false, "�E�F�u�o���N�f�[�^�̃t�@�C���擾���s");
		return E_FAIL;
	}
	if (CHECK_ERROR(result, "�E�F�u�o���N�̓o�^���s"))
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	// �T�E���h�o���N�̓ǂݍ��݂Ɠo�^
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
		IS_CHECK_ERROR(false, "�T�E���h�o���N�̃t�@�C���擾���s");
		return E_FAIL;
	}
	if (CHECK_ERROR(result, "�T�E���h�o���N�̓o�^���s"))
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