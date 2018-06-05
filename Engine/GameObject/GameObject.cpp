#include "GameObject.h"


namespace Engine
{
	namespace GameObject
	{
		std::map<std::string, GameObject>* m_allGameObjects;
		Engine::Vector3D& GameObject::GetGameObjectPosition()
		{
			return m_gameObjectPosition;
		}

		Engine::Vector3D& GameObject::GetGameObjectRotation()
		{
			return m_gameObjectRotation;
		}

		bool GameObject::GetIsKinematic()
		{
			return m_isKinematic;
		}

		Engine::Renderer::Sprites::Sprite* GameObject::GetGameObjectSprite()
		{
			return m_gameObjectSprite;
		}

		void GameObject::SetGameObjectPosition(Vector3D i_newPosition)
		{
			m_gameObjectPosition = i_newPosition;
		}

		void GameObject::SetGameObjectRotation(Vector3D i_newRotation)
		{
			m_gameObjectRotation = i_newRotation;
		}

		void GameObject::SetGameObjectAABB(Vector3D i_Center, Vector3D i_Extents)
		{
			m_boundingBox = { i_Center, i_Extents };
		}

		AABB GameObject::GetBoundingBox()
		{
			return m_boundingBox;
		}

		void GameObject::SetGameObjectSprite(Renderer::Sprites::Sprite* i_newSprite)
		{
			m_gameObjectSprite = i_newSprite;
		}

		void GameObject::SetGameObjectName(std::string i_ObjectName)
		{
			m_name = i_ObjectName;
		}

		void GameObject::SetGameObjectClass(std::string i_ClassName)
		{
			m_classType = i_ClassName;
		}

		void GameObject::SetIsKinematic(bool i_bInput)
		{
			m_isKinematic = i_bInput;
		}

		void Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Creating Game Object Container", __LINE__, __FILE__, "Game Objects");
#endif // _DEBUG
			m_allGameObjects = new std::map<std::string, GameObject>();
		}

		void Shutdown()
		{
			delete(m_allGameObjects);
		}

		void Update()
		{

		}
	}
}