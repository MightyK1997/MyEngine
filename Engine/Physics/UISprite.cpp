#include "UISprite.h"

eae6320::cResult eae6320::Physics::Sprite::CreateUISprite(Math::sVector i_Position, Graphics::cSprite::Handle i_SpriteHandle,
	Graphics::cMaterial::Handle i_MaterialHandle, Sprite*& o_Sprite)
{
	cResult result = Results::Success;
	o_Sprite = new Sprite();
	o_Sprite->m_RigidBody.position = Math::sVector(i_Position.x, i_Position.y, 0);
	o_Sprite->m_UIMaterial = i_MaterialHandle;
	o_Sprite->m_SpriteHandle = i_SpriteHandle;
	return result;
}
