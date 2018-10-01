#include "cMeshBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	cResult result = Results::Success;
	std::string m_errorString;
	result = eae6320::Platform::CopyFileW(m_path_source, m_path_target, false, true, &m_errorString);
	if (!result)
	{
		OutputErrorMessageWithFileInfo(m_path_source, m_errorString.c_str());
	}
	return result;
}
