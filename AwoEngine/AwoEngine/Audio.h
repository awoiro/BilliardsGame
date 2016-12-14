#pragma once

#include <xact3.h>
#include "ErrorManager.h"

// Sound Data
const char WAVE_BANK[] = "../Sound/win/Wave Bank01.xwb";
const char SOUND_BANK[] = "../Sound/win/Sound Bank01.xsb";
// Sound Que
const char BGM1[] = "bgm1";
const char BGM2[] = "bgm2";
const char BGM3[] = "bgm3";
const char SE_BALL_HIT[] = "ball_hit";
const char SE_BALL_FALL[] = "ball_fall";

class Audio
{
public:
	Audio()
		:m_pXACTEngine(nullptr)
		, m_pWaveBank(nullptr)
		, m_pSoundBank(nullptr)
		, m_cueIndex(0)
		, m_pMapWaveBank(nullptr)
		, m_pSoundBankData(nullptr)
		, m_coInit(false)
	{
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		m_coInit = SUCCEEDED(hr) ? true : false;
	}
	~Audio()
	{
		if (m_pXACTEngine)
		{
#if false
			m_pSoundBank->Destroy();
			delete m_pSoundBank;

			m_pWaveBank->Destroy();
			delete m_pWaveBank;
#endif
			m_pXACTEngine->ShutDown();
			m_pXACTEngine->Release();
		}

		if (m_pSoundBankData)
		{
			delete[] m_pSoundBankData;
			m_pSoundBankData = nullptr;
		}

		if (m_pMapWaveBank)
		{
			UnmapViewOfFile(m_pMapWaveBank);
			//delete[] m_pMapWaveBank;
			m_pMapWaveBank = nullptr;
		}

		if (m_coInit)
		{
			CoUninitialize();
		}
	}

	HRESULT Init();
	void PlayCue(const char cue[]);
	void StopCue(const char cue[]);

private:

	IXACT3Engine* m_pXACTEngine;
	IXACT3WaveBank* m_pWaveBank;
	IXACT3SoundBank* m_pSoundBank;
	XACTINDEX m_cueIndex;
	void* m_pMapWaveBank;
	void* m_pSoundBankData;

	bool m_coInit;
};
