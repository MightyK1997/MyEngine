#pragma once
#include "../Physics/sRigidBodyState.h"

#include <Engine/Results/Results.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Math/cMatrix_transformation.h>

namespace eae6320
{
	namespace Graphics
	{
		class cCamera
		{
		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cCamera)
			static cResult CreateCamera(cCamera*& o_Camera);
			static cResult CreateCamera(Math::sVector i_InputPosition, Math::cQuaternion i_InputRotation, cCamera*& o_OutputCamera);


			void SetCameraPosition(Math::sVector i_InputPosition);
			void SetCameraRotation(Math::cQuaternion i_InputRotation);
			void SetCameraVelocity(Math::sVector i_InputVelocity);
			void SetCameraAcceleration(Math::sVector i_InputAcceleration);
			void SetAngularSpeed(const float i_AngularSpeed);

			Math::sVector GetCameraPosition();
			Math::cQuaternion GetCameraRotation();
			float GetAngularSpeed();

			Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;
			Math::cMatrix_transformation GetLocalToWorldTransformation(float i_secondCountToExtrapolate) const;

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);
			
		private:
			cCamera() {}
			~cCamera() {}
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			Physics::sRigidBodyState m_CameraRigidBody;
		};
	}
}
