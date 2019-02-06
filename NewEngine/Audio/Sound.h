#pragma once

//C++ includes
#include <string>

//Engine Includes
#include "Audio.h"
#include <unordered_map>

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
		HRESULT Play(const std::string& i_SoundFilePath);
		HRESULT AddToDefaultQueue(const std::string& i_SoundFilePath);
		//~Sound();

	private:
		HRESULT LoadFileData(const std::string& i_SoundFilePath, HANDLE& o_FileHandle);
		HRESULT FindDataChunk(HANDLE i_FileHandle, DWORD fourcc, DWORD& o_dwChunkSize, DWORD& o_dwChunkPosition);
		HRESULT ReadDataFromChunk(HANDLE i_FileHandle, void* i_Buffer, DWORD i_BufferSize, DWORD i_BufferOffset);
		HRESULT LoadFileDataIntoBuffer(HANDLE i_FileHandle, XAUDIO2_BUFFER& o_Buffer, WAVEFORMATEX& o_WaveFormat);


	private:
		Audio* m_Audio = nullptr;
		std::unordered_map<std::string, IXAudio2SourceVoice*> m_ListOfAllSources;
	};
}
