#include "Audio.h"

using namespace Sound;
Audio::Audio()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio, 0, 0))) { pXAudio = nullptr; }
	if (FAILED(hr = pXAudio->CreateMasteringVoice(&pMasteringVoice))) { pMasteringVoice = nullptr; }
}

Audio::~Audio()
{
	pXAudio->Release();
}