#include "Sound.h"

std::wstring ConvertUtf8ToUtf16(const char* const i_string_utf8)
{

	//Code Copyright John-Paul Ownby.
	//Reproduced with permission.
	// Calculate how many UTF-16 code units are required
	int requiredCodeUnitCount_includingNullTerminator;
	constexpr DWORD flags = 0;
	constexpr int processEntireString = -1;
	{
		constexpr wchar_t* const noOutput = nullptr;
		constexpr int returnRequiredCount = 0;
		requiredCodeUnitCount_includingNullTerminator = MultiByteToWideChar(CP_UTF8, flags, i_string_utf8,
			processEntireString, noOutput, returnRequiredCount);
		if (requiredCodeUnitCount_includingNullTerminator != 0)
		{
		}
	}
	// Allocate space
	if (requiredCodeUnitCount_includingNullTerminator > 1)
	{
		std::wstring o_string_utf16(requiredCodeUnitCount_includingNullTerminator, L'\0');
		// Convert the string
		{
			const auto convertedCodeUnitCount = MultiByteToWideChar(CP_UTF8, flags, i_string_utf8,
				processEntireString, &o_string_utf16[0], requiredCodeUnitCount_includingNullTerminator);
			if (convertedCodeUnitCount != 0)
			{
				// std::wstring automatically manages NULL terminators
				o_string_utf16.resize(requiredCodeUnitCount_includingNullTerminator - 1);
			}
		}

		return o_string_utf16;
	}
	else
	{
		// The string is empty
		return std::wstring();
	}
}


void Sound::Sound::Play(const std::string& i_SoundFilePath)
{
	HRESULT hr = S_OK;
	if (i_SoundFilePath.substr(i_SoundFilePath.find('.') + 1) != "wav")  return;
	if (m_Audio == nullptr)	{ m_Audio = new Audio(); }

	auto iterator = m_ListOfAllSources.find(i_SoundFilePath);
	if (iterator != m_ListOfAllSources.end())
	{
		auto source = iterator->second;
		m_Audio->PlayBuffer(source);
		return;
	}

	HANDLE soundFileHandle;
	hr = LoadFileData(i_SoundFilePath, soundFileHandle);
	if (FAILED(hr)) return;

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	hr = FindDataChunk(soundFileHandle, fourccRIFF, dwChunkSize, dwChunkPosition);
	if (FAILED(hr)) return;
	DWORD fileType;
	hr = ReadDataFromChunk(soundFileHandle, &fileType, sizeof(DWORD), dwChunkPosition);
	if (fileType != fourccWAVE) return;

	//Read FMT data
	FindDataChunk(soundFileHandle, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadDataFromChunk(soundFileHandle, &fileData, dwChunkSize, dwChunkPosition);

	//Read data chunk from file
	FindDataChunk(soundFileHandle, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadDataFromChunk(soundFileHandle, pDataBuffer, dwChunkSize, dwChunkPosition);

	auto value = *pDataBuffer;

	//Populate data into xaudio buffer
	buffer.AudioBytes = dwChunkSize;
	buffer.pAudioData = pDataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	IXAudio2SourceVoice* source;
	if (FAILED(hr = m_Audio->PlayBuffer(buffer, &fileData, source))) return;
	m_ListOfAllSources.insert(std::make_pair(i_SoundFilePath, source));
}

HRESULT Sound::Sound::LoadFileData(const std::string& i_SoundFilePath, HANDLE& o_FileHandle)
{
	HRESULT hr = S_OK;
	o_FileHandle = CreateFileW(ConvertUtf8ToUtf16(i_SoundFilePath.c_str()).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == o_FileHandle) return HRESULT_FROM_WIN32(GetLastError());
	if (INVALID_SET_FILE_POINTER == SetFilePointer(o_FileHandle, 0, NULL, FILE_BEGIN)) return HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT Sound::Sound::FindDataChunk(HANDLE i_FileHandle, DWORD fourcc, DWORD& o_dwChunkSize, DWORD& o_dwChunkPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(i_FileHandle, 0, NULL, FILE_BEGIN)) return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(i_FileHandle, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(i_FileHandle, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(i_FileHandle, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(i_FileHandle, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			o_dwChunkSize = dwChunkDataSize;
			o_dwChunkPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::Sound::ReadDataFromChunk(HANDLE i_FileHandle, void* i_Buffer, DWORD i_BufferSize, DWORD i_BufferOffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(i_FileHandle, i_BufferOffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(i_FileHandle, i_Buffer, i_BufferSize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
