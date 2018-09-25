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
		public:
			sRigidBodyState m_RigidBody;
			Graphics::sEffectsAndMeshesToRender m_EffectMeshPairForRigidBody;
		};
	}
}