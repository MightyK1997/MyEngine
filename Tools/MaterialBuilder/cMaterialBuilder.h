#pragma once

#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <Engine/Graphics/Configuration.h>
#include <Engine/Platform/Platform.h>


namespace eae6320
{
	namespace Assets
	{
		class cMaterialBuilder : public cbBuilder
		{
		private:
			virtual cResult Build(const std::vector<std::string>& i_arguments) override;
		};
	}
}