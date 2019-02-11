#pragma once

#include "Mmdeviceapi.h"
#include "Audioclient.h"
#include "Audiopolicy.h"
#include "Devicetopology.h"
#include "Endpointvolume.h"
#include <string>


//Defining some variables which will be used later
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000



class AudioWasapi
{
	struct Data
	{
		const BYTE* pData;
		uint64_t lengthOfData;
	};

public:
	AudioWasapi();
	~AudioWasapi();

	HRESULT PlayAudioData(const std::string& i_SoundFileName);


private:
	HRESULT LoadData(uint32_t i_BufferSize, BYTE*& o_Data);

private:
	IMMDeviceEnumerator* pDeviceEnumerator;
	IMMDeviceCollection* pAudioEndpointDevices;
	IMMDevice* pDefaultDevice;
	IAudioClient* pAudioClient;
	IAudioRenderClient* pAudioRenderClient;

	//Used to store the data for wav files, which are the ones we will be
	//using for now.
	WAVEFORMATEX* pWaveFormat;
	uint32_t mBufferSize;
	uint32_t mNumberOfPaddingFrames;
	uint32_t mNumberOfAvailableFrames;
	Data wavData;
	DWORD flags = 0;

	uintptr_t m_CurrentPointerLocation = 0;

	//These are the duration for which the buffer is played. We initialize it
	//To reference time per second and then update the values.
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;


	//const values
	const CLSID CLSID_mmEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
};

