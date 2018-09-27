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
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cGameObject)
			//Static Factory Functions
			static cResult CreateGameObject(cGameObject*& o_GameObject);
			static cResult CreateGameObject(Math::sVector i_Position, Math::cQuaternion i_Rotation, cGameObject*& o_GameObject);
			static cResult CreateGameObject(Math::sVector i_Position, Math::cQuaternion i_Rotation, Graphics::cMesh* i_Mesh, 
				Graphics::cEffect* i_Effect, cGameObject*& o_GameObject);
			static cResult CreateGameObject(Math::sVector i_Position, Graphics::cMesh* i_Mesh, Graphics::cEffect* i_Effect, cGameObject*& o_GameObject);

			//Member Functios
			void SetGameObjectPosition(Math::sVector i_Position);
			void SetGameObjectRotation(Math::cQuaternion i_Rotation);
			Math::sVector GetGameObjectPosition();
			Math::cQuaternion GetGameObjectRotation();
			void SetGameObjectVelocity(Math::sVector i_Velocity);
			void SetGameObjectAcceleration(Math::sVector i_Acceleration);
			void SetGameObjectMesh(Graphics::cMesh* i_Mesh);
			void SetGameObjectEffect(Graphics::cEffect* i_Effect);
			Graphics::cMesh* GetGameObjectMesh();
			Graphics::cEffect* GetGameObjectEffet();
			Graphics::sEffectsAndMeshesToRender GetMeshEffectPair();
			void UpdateGameObject(const float i_elapsedSecondCount_sinceLastUpdate);
			Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;

			//Get the matrix transformation
			Math::cMatrix_transformation GetLocalToWorldTransformation(const float i_secondCountToExtrapolate);
		public:
		private:
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			cGameObject(){}
			~cGameObject(){}
			sRigidBodyState m_RigidBody;
			Graphics::sEffectsAndMeshesToRender m_EffectMeshPairForRigidBody;
		};
	}
}