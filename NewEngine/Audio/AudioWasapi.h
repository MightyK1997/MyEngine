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
public:
	AudioWasapi();
	~AudioWasapi();

	HRESULT PlayAudioData(const std::string& i_SoundFileName);

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

	//These are the duration for which the buffer is played. We initialize it
	//To reference time per second and then update the values.
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
};

