#include "Sound.h"

using namespace Sound;

void ::Sound::Sound::Play(const std::string& i_SoundFilePath)
{
	if (m_Audio == nullptr)	{ m_Audio = new Audio(); }
}
