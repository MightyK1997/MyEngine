#pragma once

//C++ includes
#include <string>

//Engine Includes
#include "Audio.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


namespace Sound
{
	class Sound
	{
	public:
		void Play(const std::string& i_SoundFilePath);

	private:
		HRESULT LoadFileData(const std::string& i_SoundFilePath, HANDLE& o_FileHandle);
		HRESULT FindDataChunk(HANDLE i_FileHandle, DWORD fourcc, DWORD& o_dwChunkSize, DWORD& o_dwChunkPosition);
		HRESULT ReadDataFromChunk(HANDLE i_FileHandle, void* i_Buffer, DWORD i_BufferSize, DWORD i_BufferOffset);


	private:
		Audio* m_Audio = nullptr;
		WAVEFORMATEX fileData = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };
	};
}
