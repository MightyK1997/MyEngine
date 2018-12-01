#include "Sound.h"
#include <mmdeviceapi.h>
#include <combaseapi.h>
#include "Engine/Platform/Platform.h"
namespace
{
	uint32_t g_numberofframes;
	uintptr_t dataOffset;
	uintptr_t finalOffset;
}

//-----------------------------------------------------------
// Play an audio stream on the default audio rendering
// device. The PlayAudioStream function allocates a shared
// buffer big enough to hold one second of PCM audio data.
// The function uses this buffer to stream data to the
// rendering device. The inner loop runs every 1/2 second.
//-----------------------------------------------------------

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

HRESULT PlayAudioStream(eae6320::audio::MyAudioSource *pMySource)
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	EXIT_ON_ERROR(hr)

		hr = pEnumerator->GetDefaultAudioEndpoint(
			eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr)

		hr = pDevice->Activate(
			IID_IAudioClient, CLSCTX_ALL,
			NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			hnsRequestedDuration,
			0,
			pwfx,
			NULL);
	EXIT_ON_ERROR(hr)

		// Tell the audio source which format to use.
		hr = pMySource->SetFormat(pwfx);
	EXIT_ON_ERROR(hr)

		// Get the actual size of the allocated buffer.
		hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetService(
			IID_IAudioRenderClient,
			(void**)&pRenderClient);
	EXIT_ON_ERROR(hr)

		// Grab the entire buffer for the initial fill operation.
		hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
	EXIT_ON_ERROR(hr)

		// Load the initial data into the shared buffer.
		hr = pMySource->LoadData(bufferFrameCount, pData, &flags);
	EXIT_ON_ERROR(hr)

		hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
	EXIT_ON_ERROR(hr)

		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec);

	hr = pAudioClient->Start();  // Start playing.
	EXIT_ON_ERROR(hr)

		// Each loop fills about half of the shared buffer.
		while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
		{
			// Sleep for half the buffer duration.
			Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

			// See how much buffer space is available.
			hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
			EXIT_ON_ERROR(hr)

				numFramesAvailable = bufferFrameCount - numFramesPadding;

			// Grab all the available space in the shared buffer.
			hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);
			EXIT_ON_ERROR(hr)

				// Get next 1/2-second of data from the audio source.
				hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
			EXIT_ON_ERROR(hr)

				hr = pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
			EXIT_ON_ERROR(hr)
		}

	// Wait for last data in buffer to play before stopping.
	Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

	hr = pAudioClient->Stop();  // Stop playing.
	EXIT_ON_ERROR(hr)

		Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
		SAFE_RELEASE(pDevice)
		SAFE_RELEASE(pAudioClient)
		SAFE_RELEASE(pRenderClient)

		return hr;
}

HRESULT eae6320::audio::MyAudioSource::SetFormat(WAVEFORMATEX *i_WaveFormat)
{
	m_WaveFormat = i_WaveFormat;
	return NOERROR;
}

HRESULT eae6320::audio::MyAudioSource::LoadData(UINT32 NumberOfAudioFrames, BYTE*& o_Data, DWORD* o_flags)
{
	if (finalOffset >= dataOffset + NumberOfAudioFrames * 4)
	{
		memcpy(o_Data, reinterpret_cast<void*>(dataOffset), NumberOfAudioFrames * 4);
		dataOffset += NumberOfAudioFrames *4;
		o_flags = 0;
		return NOERROR;
	}
	return E_FAIL;
}

HRESULT eae6320::audio::MyAudioSource::OpenFile(const char * i_FileName)
{
	HRESULT hr = NOERROR;
	eae6320::Platform::sDataFromFile fileData;
	WAVDATA wavData;
	std::string errorMessage;
	if (eae6320::Platform::LoadBinaryFile(i_FileName, fileData, &errorMessage))
	{
		uintptr_t offset = reinterpret_cast<uintptr_t>(fileData.data);
		finalOffset = offset + fileData.size;
		wavData.chunckID = reinterpret_cast<char*>(offset);
		offset += 4;
		wavData.chunkSize = *reinterpret_cast<uint32_t*>(offset);
		offset += sizeof(uint32_t);
		wavData.format = reinterpret_cast<char*>(offset);
		offset += 4;
		wavData.subChunk1ID = reinterpret_cast<char*>(offset);
		offset += 4;
		wavData.subchunk1Size = *reinterpret_cast<uint32_t*>(offset);
		offset += sizeof(uint32_t);
		wavData.audioFormat = *reinterpret_cast<uint16_t*>(offset);
		offset += sizeof(uint16_t);
		wavData.numChannels = *reinterpret_cast<uint16_t*>(offset);
		offset += sizeof(uint16_t);
		wavData.sampleRate = *reinterpret_cast<uint32_t*>(offset);
		offset += sizeof(uint32_t);
		wavData.byteRate = *reinterpret_cast<uint32_t*>(offset);
		offset += sizeof(uint32_t);
		wavData.blockAlign = *reinterpret_cast<uint16_t*>(offset);
		offset += sizeof(uint16_t);
		wavData.bitsPerSample = *reinterpret_cast<uint16_t*>(offset);
		offset += sizeof(uint16_t);
		wavData.subChunk2ID = reinterpret_cast<char*>(offset);
		offset += 4;
		wavData.subChunk2Size = *reinterpret_cast<uint32_t*>(offset);
		offset += sizeof(uint32_t);
		wavData.data = reinterpret_cast<void*>(offset);
		dataOffset = reinterpret_cast<uintptr_t>(wavData.data);
		data = wavData;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

void eae6320::audio::PlayAudio(std::string i_PathToAudioFile)
{
	MyAudioSource* m = new MyAudioSource();
	m->OpenFile(i_PathToAudioFile.c_str());
	PlayAudioStream(m);
}
