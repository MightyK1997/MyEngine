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
	std::wstring ConvertUtf8ToUtf16(const char* const i_string_utf8);
	class Sound
	{
	public:
		HRESULT Play(const std::string& i_SoundFilePath);
		HRESULT AddToDefaultQueue(const std::string& i_SoundFilePath);
		//~Sound();

		static HRESULT LoadFileData(const std::string& i_SoundFilePath, HANDLE& o_FileHandle);
		static HRESULT FindDataChunk(HANDLE i_FileHandle, DWORD fourcc, DWORD& o_dwChunkSize, DWORD& o_dwChunkPosition);
		static HRESULT ReadDataFromChunk(HANDLE i_FileHandle, void* i_Buffer, DWORD i_BufferSize, DWORD i_BufferOffset);
		static HRESULT LoadFileDataIntoBuffer(HANDLE i_FileHandle, XAUDIO2_BUFFER& o_Buffer, WAVEFORMATEX& o_WaveFormat);


	private:
		Audio* m_Audio = nullptr;
		std::unordered_map<std::string, IXAudio2SourceVoice*> m_ListOfAllSources;
	};
}
