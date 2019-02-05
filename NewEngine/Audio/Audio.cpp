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

HRESULT Audio::PlayBuffer(const XAUDIO2_BUFFER i_Buffer, const WAVEFORMATEX* i_WaveFormat, IXAudio2SourceVoice*& o_AudioSource) const
{
	HRESULT hr = S_OK;
	IXAudio2SourceVoice* pSourceVoice;
	if (FAILED(hr = pXAudio->CreateSourceVoice(&pSourceVoice, i_WaveFormat))) return hr;
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&i_Buffer))) return hr;
	if (FAILED(hr = pSourceVoice->Start())) return hr;
	o_AudioSource = pSourceVoice;
	return hr;
}

HRESULT Audio::PlayBuffer(IXAudio2SourceVoice* pBuffer)
{
	HRESULT hr = S_OK;
	if (FAILED(hr = pBuffer->Stop())) return hr;
	if (FAILED(hr = pBuffer->Start())) return hr;
	return hr;
}
