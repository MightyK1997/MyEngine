#pragma once

#include "cEffect.h"
#include "cTexture.h"
#include "ConstantBufferFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <utility>

namespace eae6320
{
	namespace Graphics
	{
		class cMaterial
		{
		public:
			using Handle = Assets::cHandle<cMaterial>;
			static Assets::cManager<cMaterial> s_Manager;

			static cResult Load(const std::string& i_FilePath, cMaterial*& o_Material);

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMaterial)

			eae6320::Graphics::cEffect::Handle GetEffectHandle() { return m_EffectHandle; }
			eae6320::Graphics::cTexture::Handle GetTextureHandle() { return m_TextureHandle; }

			eae6320::Graphics::sColor GetMaterialColor() const { return m_Color; }


		private:
			//Member variables
			cEffect::Handle m_EffectHandle;
			cTexture::Handle m_TextureHandle;
			sColor m_Color = sColor{ 0,0,0,0 };
			void Shutdown();

			cMaterial() = default;
			~cMaterial()
			{
				eae6320::Graphics::cTexture::s_manager.Release(m_TextureHandle);
				Shutdown();
			}

			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			
		};
	}
}