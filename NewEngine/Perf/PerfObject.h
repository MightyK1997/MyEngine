#pragma once
#include "../Debug/Log.h"
#include <chrono>
#include <string>

class PerfObject
{
public:
	PerfObject(const char* i_taskName)
	{
		m_taskName = i_taskName;
		m_startTime = std::chrono::system_clock::now();
	}

	~PerfObject()
	{
		auto endTime = std::chrono::system_clock::now();
		std::string opString = "Task" + m_taskName + "Took" + std::to_string( ( endTime - m_startTime ).count() );
		Engine::Debug::Log((void*)opString.c_str(), "%s");
	}

private:
	std::string m_taskName = "";
	std::chrono::time_point<std::chrono::system_clock> m_startTime;

};