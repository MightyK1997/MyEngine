#pragma once

#include "sRigidBodyState.h"

#include "../Graphics/cMesh.h"
#include "../Graphics/cEffect.h"


namespace eae6320
{
	namespace Physics
	{
		class cGameObject
		{
		private:
			sRigidBodyState m_RigidBody;
			Graphics::cMesh m_GameObjectMesh;
			Graphics::cEffect m_GameObjectEffect;
		};
	}
}