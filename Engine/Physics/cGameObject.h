#pragma once

#include "sRigidBodyState.h"

#include "../Graphics/cMesh.h"
#include "../Graphics/cEffect.h"
#include "../Graphics/cMaterial.h"


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
			Math::sVector GetGameObjectVelocity();
			void SetGameObjectVelocity(Math::sVector i_Velocity);
			void SetGameObjectAngularVelocity(float i_AngularVelocity);
			void SetGameObjectAcceleration(Math::sVector i_Acceleration);
			void SetGameObjectMesh(Graphics::cMesh* i_Mesh);
			void SetGameObjectEffect(Graphics::cEffect* i_Effect);
			void SetGameObjectHandles(Graphics::cMesh::Handle i_MeshHandle, Graphics::cEffect::Handle i_EffectHandle);
			void SetGameObjectHandles(Graphics::cMesh::Handle i_MeshHandle, Graphics::cMaterial::Handle i_EffectHandle);
			Graphics::cMesh* GetGameObjectMesh() const;
			Graphics::cEffect* GetGameObjectEffect() const;
			Graphics::sEffectsAndMeshesToRender GetMeshEffectPair() const;
			void UpdateGameObject(float i_elapsedSecondCount_sinceLastUpdate);
			Math::sVector PredictFuturePosition(float i_secondCountToExtrapolate) const;
			Math::cQuaternion PredictFutureOrientation(float i_secondCountToExtrapolate) const;
			Graphics::cMesh::Handle GetGameObjectMeshHandle() const { return m_MeshHandle; }
			Graphics::cEffect::Handle GetGameObjectEffectHandle() const { return m_EffectHandle; }
			Graphics::cMaterial::Handle GetGameObjectMaterialHandle() const { return m_MaterialHandle; }

			//Get the matrix transformation
			Math::cMatrix_transformation GetLocalToWorldTransformation(const float i_secondCountToExtrapolate);
		public:
		private:
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			cGameObject(){}
			~cGameObject(){}
			sRigidBodyState m_RigidBody;
			Graphics::sEffectsAndMeshesToRender m_EffectMeshPairForRigidBody;
			Graphics::cMesh::Handle m_MeshHandle;
			Graphics::cEffect::Handle m_EffectHandle;
			Graphics::cMaterial::Handle m_MaterialHandle;
		};
	}
}