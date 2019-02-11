#pragma once

//#include "cEffect.h"
#include "Graphics.h"
#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cShader.h"
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <utility>

namespace eae6320
{
	namespace Graphics
	{
		class cEffect;
		class cMaterial
		{
		public:
			using Handle = Assets::cHandle<cMaterial>;
			static Assets::cManager<cMaterial> s_Manager;

			static cResult Load(const std::string& i_FilePath, cMaterial*& o_Material);

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMaterial)


		private:
			cMaterial(){}
			~cMaterial(){}
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			//Member variables
			cEffect* m_Effect;
			sColor m_Color;
		};
	}
}