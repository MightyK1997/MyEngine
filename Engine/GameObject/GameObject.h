#pragma once
#include <string>
#include <vector>
#include <map>

#include "..\Renderer\Renderer.h"
#include "..\Math\Vector2D.h"
#include "..\Math\Vector3D.h"
#include "..\SmartPointers\SharedPointer.h"

namespace Engine
{
	namespace GameObject
	{
		class GameObject;
		extern std::map < std::string, GameObject>* m_allGameObjects;
		void Initialize();
		void Shutdown();
		void Update();
		struct AABB
		{
			Vector3D Center;
			Vector3D Extents;
		};
		class GameObject
		{
		public:
			Vector3D& GetGameObjectPosition();
			Vector3D& GetGameObjectRotation();
			bool GetIsKinematic();
			Renderer::Sprites::Sprite* GetGameObjectSprite();
			void SetGameObjectPosition(Vector3D);
			void SetGameObjectRotation(Vector3D);
			void SetGameObjectAABB(Vector3D, Vector3D);
			void SetGameObjectSprite(Renderer::Sprites::Sprite*);
			void SetGameObjectName(std::string);
			void SetGameObjectClass(std::string);
			void SetIsKinematic(bool);
			AABB GetBoundingBox();
			AABB m_boundingBox;
			~GameObject()
			{
			}
		private:
			Renderer::Sprites::Sprite* m_gameObjectSprite;
			Vector3D m_gameObjectPosition;
			Vector3D m_gameObjectRotation;
			std::string m_name;
			std::string m_classType;
			bool m_isKinematic = false;
		};
	}
}