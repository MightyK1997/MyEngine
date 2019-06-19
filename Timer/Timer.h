#pragma once
#include "../Common/Includes.h"
#include <cstdint>

namespace Engine
{
	namespace Timer
	{
		
		float GetTimeDifference(long i_InputTime);

		void Initialize();
		void Update();


		float deltaTime;
	}
}
