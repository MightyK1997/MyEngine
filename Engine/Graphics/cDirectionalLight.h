#pragma once
#include "../Physics/sRigidBodyState.h"

#include <Engine/Results/Results.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Math/cMatrix_transformation.h>
#include "sRGB.h"

namespace eae6320
{
	namespace Graphics
	{
		class cLight
		{
		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
				EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cLight)
				static cResult CreateDirectionalLight(cLight*& o_Light);
			static cResult CreateDirectionalLight(Math::sVector i_InputPosition, Math::cQuaternion i_InputRotation,sColor i_Color, cLight*& o_OutputLight);


			void SetLightPosition(Math::sVector i_InputPosition) { m_RigidBody.position = i_InputPosition; }
			void SetLightRotation(Math::cQuaternion i_InputRotation) { m_RigidBody.orientation = i_InputRotation; }
			void SetLightVelocity(Math::sVector i_InputVelocity) { m_RigidBody.velocity = i_InputVelocity; }
			void SetLightAcceleration(Math::sVector i_InputAcceleration) { m_RigidBody.acceleration = i_InputAcceleration; }
			void SetLightColor(sColor i_Color) { m_LightColor = i_Color; }
			void SetAngularSpeed(const float i_AngularSpeed) { m_RigidBody.angularSpeed = i_AngularSpeed; }

			Math::sVector GetLightPosition() const { return m_RigidBody.position; }
			Math::cQuaternion GetLightRotation() const { return m_RigidBody.orientation; }
			float GetAngularSpeed() const { return m_RigidBody.angularSpeed; }

			sColor GetLightColor() const { return m_LightColor; }

			Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;
			Math::cMatrix_transformation GetLocalToWorldTransformation(float i_secondCountToExtrapolate) const;

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);

		private:
			cLight() = default;
			~cLight() = default;
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
				Physics::sRigidBodyState m_RigidBody;
			sColor m_LightColor = { 0,0,0,1 };

		};
	}
}
