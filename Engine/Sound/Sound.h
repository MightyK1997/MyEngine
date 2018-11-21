#pragma once
#pragma comment(lib, "ole32.lib")
#include <Audioclient.h>
#include <audiopolicy.h>
#include <string>

namespace eae6320
{
	namespace audio
	{
		struct WAVDATA
		{
			char* chunckID;
			uint32_t chunkSize;
			char* format;
			char* subChunk1ID;
			uint32_t subchunk1Size;
			uint16_t audioFormat;
			uint16_t numChannels;
			uint32_t sampleRate;
			uint32_t byteRate;
			uint16_t blockAlign;
			uint16_t bitsPerSample;
			char* subChunk2ID;
			uint32_t subChunk2Size;
			void* data;
		};

		void PlayAudio(std::string i_PathToAudioFile);

		class MyAudioSource
		{
		public:
			HRESULT SetFormat(WAVEFORMATEX*);
			HRESULT LoadData(UINT32, BYTE*&, DWORD*);
			HRESULT OpenFile(const char* i_FileName);

		private:
			WAVEFORMATEX* m_WaveFormat;
			WAVDATA data;
		};
	}
}
