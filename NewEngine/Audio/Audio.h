#pragma once
#include "xaudio2.h"
#include "x3daudio.h"

namespace Sound
{
	class Audio
	{
	public:
		Audio();
		~Audio();

	private:
		IXAudio2* pXAudio = nullptr;
		IXAudio2MasteringVoice* pMasteringVoice = nullptr;
	};
}

