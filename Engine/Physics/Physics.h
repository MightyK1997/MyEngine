#pragma once

#include <utility>
#include "..\GameObject\GameObject.h"
#include "..\Math\Matrix4.h"
#include "..\SmartPointers\SharedPointer.h"
#include "..\Messaging\Messaging.h"
#include "..\Timer\Timer.h"
#include "..\Profiling\Profiler.h"
#include "..\Math\Vector3D.h"

namespace Engine
{
	namespace Physics
	{
		class PhysicsObject;
		extern std::vector<PhysicsObject*>* m_allPhysicsObjects;
		void Initialize();
		void Update();
		void Shutdown();
		struct CollisionBody
		{
			PhysicsObject* po;
			Matrix4 m_RotationMatrix;
			Matrix4 m_TranslationMatrix;
			Matrix4 m_ThisBodyToWord;
			Matrix4 m_WorldToThisBody;
			Vector4D m_CenterInWorld;
		};
		struct CollisionPair
		{
			float m_CollisionTime;
			Vector3D m_CollisionNormal;
			std::array<CollisionBody*, 2> m_CollisionObjects;
		};

		class PhysicsObject
		{
		public:
			PhysicsObject(Engine::GameObject::GameObject* i_gameObject, float i_mass, float i_drag) : m_go( new Engine::GameObject::GameObject(*i_gameObject)), m_objectMass(i_mass), m_dragThroughMedium(i_drag)
			{
				Engine::Physics::m_allPhysicsObjects->push_back(this);
			}
			void Update();
			Vector3D GetCurrentPosition();
			Vector3D GetCurrentRotation();
			void SetCurrentPosition(Vector3D i_NewPosition);
			Vector3D GetCurrentVelocity()
			{
				return m_currentVelocity;
			}
			void ApplyForce(Vector3D i_Force);
			void SetCurrentVelocity(const Vector3D& i_Velocity) { m_currentVelocity = i_Velocity; }
			void CheckForCollision(float i_DeltaTime);
			CollisionPair* CheckForCollisionBetweenTwoColliders(CollisionBody*, CollisionBody*, float i_DeltaTime);
			void GenerateCollisionBody();

		private:
			SharedPtr<Engine::GameObject::GameObject> m_go;
			float m_objectMass;
			float m_dragThroughMedium;
			Vector3D m_previousVelocity;
			Vector3D m_currentVelocity;
			Vector3D m_previousPosition;
			CollisionBody* m_CollisionBody;
			Vector3D m_currentForce;
			float CalculateDrag();
		};
	}
}