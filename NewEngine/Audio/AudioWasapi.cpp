#include "AudioWasapi.h"

//I've already created a working sound system using XAudio2, which i'll show in the stream later.
//Let me know in the chat, if the font is too small.

AudioWasapi::AudioWasapi()
{

	HRESULT hr;
	//First ch;eck for suitable audio endpoint device.

	const CLSID CLSID_mmEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	if (FAILED(hr = CoCreateInstance(CLSID_mmEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator
		, reinterpret_cast<void**>(&pDeviceEnumerator))))
	{
		return;
	}
	//This creates a reference to the IMMDeviceEnumerator which we use to access the interfaces of the 
	//MMDevice API.

	//This gets us all the audio endpoints present in the system.
	if (FAILED(hr = pDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pAudioEndpointDevices)))
	{
		return;
	}

	//This gets the default output device for the audio. We will use this for now and
	//Expand as we add more features.
	if (FAILED(hr = pDeviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, eConsole, &pDefaultDevice)))
	{
		return;
	}

	//Activate the default device
	if (FAILED(hr = pDefaultDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, reinterpret_cast<void**>(&pAudioClient)))) return;
	
	
}


AudioWasapi::~AudioWasapi()
{
}

//We use this method to play the audio data. We will add parameters as we need them.
HRESULT AudioWasapi::PlayAudioData(const std::string& i_SoundFileName)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = pAudioClient->GetMixFormat(&pWaveFormat))) return hr;

	if (FAILED(hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0,
		pWaveFormat, NULL))) return hr;

	//You can either use return hr or EXIT_ON_ERROR(hr). Does the same job.
	//Get actual buffer size
	if (FAILED(hr = pAudioClient->GetBufferSize(&mBufferSize))) return hr;
	if (FAILED(hr = pAudioClient->GetService(IID_IAudioClient, reinterpret_cast<void**>(&pAudioRenderClient)))) return hr;

	//Now we get the buffer in which we store data.




	return hr;
}
