#pragma once

namespace Engine
{
	class BaseClass
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}
