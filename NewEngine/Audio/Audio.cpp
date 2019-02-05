#include "Audio.h"

using namespace Sound;
Audio::Audio()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR))) { pXAudio = nullptr; }
	if (FAILED(hr = pXAudio->CreateMasteringVoice(&pMasteringVoice))) { pMasteringVoice = nullptr; }
}

Audio::~Audio()
{
	pXAudio->Release();
}

HRESULT Audio::PlayBuffer(const XAUDIO2_BUFFER i_Buffer, const WAVEFORMATEX* i_WaveFormat) const
{
	HRESULT hr = S_OK;
	IXAudio2SourceVoice* pSourceVoice;
	if (FAILED(hr = pXAudio->CreateSourceVoice(&pSourceVoice, i_WaveFormat))) return hr;
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&i_Buffer))) return hr;
	if (FAILED(hr = pSourceVoice->Start())) return hr;
	return hr;
}
