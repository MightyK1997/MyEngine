#include "Physics.h"

using namespace Engine;

namespace Engine
{
	namespace Physics
	{
		std::vector<PhysicsObject*>* m_allPhysicsObjects;
		void PhysicsObject::Update()
		{
			//PreCollision
			Vector3D acceleration = (m_currentForce / m_objectMass);
			Vector3D newVelocity = (m_currentVelocity)+(acceleration * (Engine::Timer::g_elapsedTime / 5));
			m_currentVelocity = newVelocity;
			Vector3D newPosition = GetCurrentPosition() + newVelocity * (Engine::Timer::g_elapsedTime / 5);
			SetCurrentPosition(newPosition);
#ifdef PROFILING
			{
				Engine::Profiler::ScopedTimer(Engine::Profiler::g_Profiler->GetAccumulator("Collision"));
				CheckForCollision(Engine::Timer::g_elapsedTime);
			}
#endif
			//Post Collision
			m_currentForce = 0;
		}

		void Update()
		{
			for (const auto &value : *m_allPhysicsObjects)
			{
				value->Update();
			}
		}

		Vector3D PhysicsObject::GetCurrentPosition()
		{
			return this->m_go->GetGameObjectPosition();
		}

		Vector3D PhysicsObject::GetCurrentRotation()
		{
			return this->m_go->GetGameObjectRotation();
		}

		void PhysicsObject::SetCurrentPosition(Vector3D i_NewPosition)
		{
			this->m_go->SetGameObjectPosition(i_NewPosition);
		}


		void PhysicsObject::ApplyForce(Vector3D i_Force)
		{
			m_currentForce += i_Force;
		}

		void PhysicsObject::CheckForCollision(float i_DeltaTime)
		{
			std::vector<CollisionBody*>* m_allColliders = new std::vector<CollisionBody *>();
			for (std::vector<PhysicsObject*>::iterator it = m_allPhysicsObjects->begin(); it != m_allPhysicsObjects->end();++it)
			{
				(*it)->GenerateCollisionBody();
				m_allColliders->push_back((*it)->m_CollisionBody);
			}

			for (std::vector<CollisionBody*>::iterator itr = m_allColliders->begin(); itr != m_allColliders->end();++itr)
			{
				for (auto jtr = itr + 1; jtr != m_allColliders->end();++jtr)
				{
					auto m_isCollided = CheckForCollisionBetweenTwoColliders((*itr), (*jtr), i_DeltaTime);
					if (m_isCollided)
					{
							if ((m_isCollided->m_CollisionObjects[0]->po->m_go->GetIsKinematic()))
							{
								auto vel1 = m_isCollided->m_CollisionObjects[0]->po->m_currentVelocity;
								auto vel2 = m_isCollided->m_CollisionObjects[1]->po->m_currentVelocity;
								auto mass1 = m_isCollided->m_CollisionObjects[0]->po->m_objectMass;
								auto mass2 = m_isCollided->m_CollisionObjects[1]->po->m_objectMass;

								auto newVel1 = vel1 * ((mass1 - mass2) / (mass1 + mass2)) + vel2 * ((2 * mass2) / (mass1 + mass2));

								newVel1 = newVel1 - (m_isCollided->m_CollisionNormal * static_cast<float>(newVel1.dot(m_isCollided->m_CollisionNormal)) * 2);

								m_isCollided->m_CollisionObjects[0]->po->m_currentVelocity = newVel1;
							}
							if ((m_isCollided->m_CollisionObjects[1]->po->m_go->GetIsKinematic()))
							{
								auto vel1 = m_isCollided->m_CollisionObjects[0]->po->m_currentVelocity;
								auto vel2 = m_isCollided->m_CollisionObjects[1]->po->m_currentVelocity;
								auto mass1 = m_isCollided->m_CollisionObjects[0]->po->m_objectMass;
								auto mass2 = m_isCollided->m_CollisionObjects[1]->po->m_objectMass;

								auto newVel2 = vel2 * ((mass2 - mass1) / (mass1 + mass2)) + vel1 * ((2 * mass1) / (mass1 + mass2));

								newVel2 = newVel2 - (m_isCollided->m_CollisionNormal * static_cast<float>(newVel2.dot(m_isCollided->m_CollisionNormal)) * 2);

								m_isCollided->m_CollisionObjects[1]->po->m_currentVelocity = newVel2;
							}
							delete m_isCollided;
						//ConsolePrint_Windows("Collision!!!!!!!", __LINE__, __FILE__, "Collision");
					}
					else
					{
						//ConsolePrint_Windows("No Collision!!!!!!!", __LINE__, __FILE__, "Collision");
					}
				}
			}

			for (std::vector<PhysicsObject*>::iterator it = m_allPhysicsObjects->begin(); it != m_allPhysicsObjects->end();++it)
			{
				delete((*it)->m_CollisionBody);
			}
			delete(m_allColliders);
		}

		CollisionPair* PhysicsObject::CheckForCollisionBetweenTwoColliders(CollisionBody* i_CollisionBody1, CollisionBody* i_CollisionBody2, float i_DeltaTime)
		{
			std::array<float, 4> tClose = { -1,-1,-1,-1 };
			std::array<float, 4> tOpen = { i_DeltaTime + 1, i_DeltaTime + 1, i_DeltaTime + 1, i_DeltaTime + 1 };
			bool isSeperatedAX = false;
			bool isSeperatedAY = false;
			bool isSeperated = false;
			float DAXClose, DAXOpen, DAYClose, DAYOpen/*, tAXOpen, tAXClose, tAYOpen, tAYClose*/;
			for (int i = 0; i < 2; i++)
			{
				//First Collision Body Things
				Matrix4 m_MAtoB = i_CollisionBody2->m_WorldToThisBody * i_CollisionBody1->m_ThisBodyToWord;
				Vector4D m_AinB = i_CollisionBody2->m_WorldToThisBody * i_CollisionBody1->m_CenterInWorld;
				Vector4D AExtentsXinB = m_MAtoB * Vector4D(i_CollisionBody1->po->m_go->m_boundingBox.Extents.getX(), 0.0f, 0.0f, 0.0f);
				Vector4D AExtentsYinB = m_MAtoB * Vector4D(0.0f, i_CollisionBody1->po->m_go->m_boundingBox.Extents.getY(), 0.0f, 0.0f);
				Vector4D ACenterinB = m_MAtoB * Vector4D(i_CollisionBody1->po->m_go->m_boundingBox.Center.getX(), i_CollisionBody1->po->m_go->m_boundingBox.Center.getY(), 0.0f, 0.0f);
				Vector3D VelARelToB = i_CollisionBody1->po->m_currentVelocity - i_CollisionBody2->po->m_currentVelocity;
				Vector4D VelAInB = i_CollisionBody2->m_WorldToThisBody * Vector4D(VelARelToB);
				Vector4D m_BxInWorld = i_CollisionBody2->m_ThisBodyToWord * Vector4D(1.0f, 0.0f, 0.0f, 0.0f);
				Vector3D AProjectionOntoB = { std::fabs(AExtentsXinB.getX()) + std::fabs(AExtentsYinB.getX()), std::fabs(AExtentsXinB.getY()) + std::fabs(AExtentsYinB.getY()), 0.0f };

				//No Collision if Relative Velocity is Zero
				if (VelAInB.getX() == 0)
				{
					std::swap(i_CollisionBody1, i_CollisionBody2);
					if (i == 1)
					{
						isSeperated = true;
					}
					continue;
				}
				float BXExtents = i_CollisionBody2->po->m_go->m_boundingBox.Extents.getX() + AProjectionOntoB.getX();
				float BYExtents = i_CollisionBody2->po->m_go->m_boundingBox.Extents.getY() + AProjectionOntoB.getY();
				float BLeft = i_CollisionBody2->po->m_go->m_boundingBox.Center.getX() - BXExtents;
				float BRight = i_CollisionBody2->po->m_go->m_boundingBox.Center.getX() + BXExtents;
				float BTop = i_CollisionBody2->po->m_go->m_boundingBox.Center.getY() + BYExtents;
				float BBottom = i_CollisionBody2->po->m_go->m_boundingBox.Center.getY() - BYExtents;


				DAXClose = BLeft - m_AinB.getX();
				DAXOpen = BRight - m_AinB.getX();
				DAYClose = BTop - m_AinB.getY();
				DAYOpen = BBottom = m_AinB.getY();

				if (VelARelToB.getX() != 0)
				{
					tClose[i * 2] = DAXClose / VelARelToB.getX();
					tOpen[i * 2] = DAXOpen / VelARelToB.getX();
				}
				if (VelARelToB.getY() != 0)
				{
					tClose[i * 2 + 1] = DAYClose / VelARelToB.getY();
					tOpen[i * 2 + 1] = DAYOpen / VelARelToB.getY();
				}

				for (int j = i * 2; j < i * 2 + 2; j++)
				{
					if (0 <= tOpen[j] && 0 <= tClose[j] && tClose[j] < i_DeltaTime)
					{
						if (tOpen[j] < tClose[j])
							std::swap(tClose[j], tOpen[j]);
					}
				}
				isSeperatedAX = std::fabs(m_AinB.getX() - i_CollisionBody2->po->m_go->m_boundingBox.Center.getX()) > (i_CollisionBody2->po->m_go->m_boundingBox.Extents.getX() + AProjectionOntoB.getX());
				isSeperatedAY = std::fabs(m_AinB.getY() - i_CollisionBody2->po->m_go->m_boundingBox.Center.getY()) > (i_CollisionBody2->po->m_go->m_boundingBox.Extents.getY() + AProjectionOntoB.getY());
				if (isSeperatedAX || isSeperatedAY)
				{
					isSeperated = true;
				}
				std::swap(i_CollisionBody1, i_CollisionBody2);
			}
			if (isSeperated)
			{
				return nullptr;
			}
			else
			{
				//Checking for Collision During Frame
				int index = -1;
				float close = -1;
				float open = i_DeltaTime + 1;
				for (int i = 0; i < 4; i++)
				{
					if (tClose[i] >= close && tClose[i] < i_DeltaTime)
					{
						close = tClose[i];
						index = i;
					}
					if (tOpen[i] < open && tOpen[i] >= 0)
					{
						open = tOpen[i];
					}
				}
				if (close >= 0 && close <= open + 0.1 && close < i_DeltaTime)
				{
					ConsolePrint_Windows("Collision", __LINE__, __FILE__, "Collision");
					CollisionPair* m_CollisionPair = new CollisionPair();
					m_CollisionPair->m_CollisionTime = 0;
					m_CollisionPair->m_CollisionObjects = { i_CollisionBody1, i_CollisionBody2 };
					//m_CollisionPair->m_CollisionObjects = { i_CollisionBody1, i_CollisionBody2 };
					m_CollisionPair->m_CollisionNormal = (index % 2 == 0) ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
					return m_CollisionPair;
				}
				return nullptr;
			}
		}


		void PhysicsObject::GenerateCollisionBody()
		{
			m_CollisionBody = new CollisionBody();
			m_CollisionBody->po = this;
			m_CollisionBody->m_RotationMatrix = Matrix4::CreateZRotation(this->GetCurrentRotation().getZ());
			m_CollisionBody->m_TranslationMatrix = Matrix4::CreateTranslation(this->GetCurrentPosition());
			m_CollisionBody->m_ThisBodyToWord = m_CollisionBody->m_TranslationMatrix * m_CollisionBody->m_RotationMatrix;
			m_CollisionBody->m_WorldToThisBody = m_CollisionBody->m_ThisBodyToWord.GetInverse();
			m_CollisionBody->m_CenterInWorld = m_CollisionBody->m_ThisBodyToWord * Vector4D(this->m_go->GetBoundingBox().Center);
		}

		void Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Creating Physics object container", __LINE__, __FILE__, "Physics Objects");
#endif // _DEBUG
			Engine::Physics::m_allPhysicsObjects = new std::vector<PhysicsObject*>;
		}

		void Shutdown()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Physics Engine Shutdown", __LINE__, __FILE__, "Physics System");
#endif // _DEBUG
			delete(m_allPhysicsObjects);
		}

		float PhysicsObject::CalculateDrag()
		{
			return (m_dragThroughMedium * this->m_currentVelocity.GetMagnitude() * this->m_currentVelocity.GetMagnitude());
		}
	}
}