#pragma once
#include "sRigidBodyState.h"
#include "Engine/Graphics/cMaterial.h"
#include "Engine/Graphics/cSprite.h"


namespace eae6320
{
	namespace Physics
	{
		class Sprite
		{
		public:
			static cResult CreateUISprite(Math::sVector i_Position, Graphics::cSprite::Handle i_SpriteHandle, Graphics::cMaterial::Handle i_MaterialHandle, Sprite*& o_Sprite);

			void SetSpritePosition(Math::sVector i_Position) { m_RigidBody.position = i_Position; }
			void SetSpriteMaterialHandle(Graphics::cMaterial::Handle i_Handle) { m_UIMaterial = i_Handle; }
			Math::sVector GetSpritePosition() const { return m_RigidBody.position; }
			Graphics::cMaterial::Handle GetSpriteMaterialHandle() const { return m_UIMaterial; }
			Graphics::cSprite::Handle GetSpriteHandle() const { return m_SpriteHandle; }

		private:
			Sprite() = default;
			~Sprite() = default;
			Graphics::cSprite::Handle m_SpriteHandle;
			sRigidBodyState m_RigidBody;
			Graphics::cMaterial::Handle m_UIMaterial;
		};
	}
}
