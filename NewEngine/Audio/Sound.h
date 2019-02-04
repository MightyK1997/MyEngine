#pragma once

//C++ includes
#include <string>

//Engine Includes
#include "Audio.h"


namespace Sound
{
	class Sound
	{
	public:
		void Play(const std::string& i_SoundFilePath);

	private:
		Audio* m_Audio = nullptr;
	};
}
