#include "AudioWasapi.h"
#include "Sound.h"

//I've already created a working sound system using XAudio2, which i'll show in the stream later.
//Let me know in the chat, if the font is too small.


AudioWasapi::AudioWasapi()
{

	HRESULT hr;
	//First ch;eck for suitable audio endpoint device.
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

	HANDLE soundHandle;
	Sound::Sound::LoadFileData(i_SoundFileName, soundHandle);

	if (FAILED(hr = pAudioClient->GetMixFormat(&pWaveFormat))) return hr;

	if (FAILED(hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0,
		pWaveFormat, NULL))) return hr;

	//You can either use return hr or EXIT_ON_ERROR(hr). Does the same job.
	//Get actual buffer size
	if (FAILED(hr = pAudioClient->GetBufferSize(&mBufferSize))) return hr;
	if (FAILED(hr = pAudioClient->GetService(IID_IAudioRenderClient, reinterpret_cast<void**>(&pAudioRenderClient)))) return hr;


	//Now we get the buffer in which we store data.

	XAUDIO2_BUFFER tempBuffer;

	Sound::Sound::LoadFileDataIntoBuffer(soundHandle, tempBuffer, *pWaveFormat);

	wavData.pData = tempBuffer.pAudioData;
	wavData.lengthOfData = tempBuffer.AudioBytes;

	BYTE* pData;


	if (FAILED(hr = pAudioRenderClient->GetBuffer(mBufferSize, &pData))) return hr;

		// Load the initial data into the shared buffer.
	hr = LoadData(mBufferSize, pData);

	hr = pAudioRenderClient->ReleaseBuffer(mBufferSize, flags);

	hnsActualDuration = static_cast<double>(REFTIMES_PER_SEC) *
		mBufferSize / pWaveFormat->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start playing.
		// Each loop fills about half of the shared buffer.
		while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
		{
			// Sleep for half the buffer duration.
			Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

			// See how much buffer space is available.
			hr = pAudioClient->GetCurrentPadding(&mNumberOfPaddingFrames);
			mNumberOfAvailableFrames = mBufferSize - mNumberOfPaddingFrames;

			// Grab all the available space in the shared buffer.
			hr = pAudioRenderClient->GetBuffer(mNumberOfAvailableFrames, &pData);
				// Get next 1/2-second of data from the audio source.
			hr = LoadData(mNumberOfAvailableFrames, pData);

			hr = pAudioRenderClient->ReleaseBuffer(mNumberOfAvailableFrames, flags);
		}

	// Wait for last data in buffer to play before stopping.
	Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

	hr = pAudioClient->Stop();  // Stop playing.

	return hr;
}

HRESULT AudioWasapi::LoadData(uint32_t i_BufferSize, BYTE*& o_Data)
{
	HRESULT hr = S_OK;
	if (m_CurrentPointerLocation + i_BufferSize < wavData.lengthOfData)
	{
		memcpy(o_Data, wavData.pData + m_CurrentPointerLocation, i_BufferSize);
		m_CurrentPointerLocation += i_BufferSize;
		return hr;
	}
	else
	{
		flags = AUDCLNT_BUFFERFLAGS_SILENT;
		return hr;
	}
}
