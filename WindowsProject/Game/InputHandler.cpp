#include "InputHandler.h"

using namespace Engine;
using namespace std;
namespace Game
{
	void InputClass::ProcessMessage(const string& i_Message, void* i_Payload)
	{
		Physics::PhysicsObject* po = static_cast<Physics::PhysicsObject*>(i_Payload);
		if (i_Message == string("Move Up"))
		{
			po->ApplyForce(Vector3D(0, 4000.0f, 0));
		}
		else if (i_Message == string("Move Down"))
		{
			po->ApplyForce(Vector3D(0, -4000.0f, 0));
		}
		else if (i_Message == string("Add Force Left"))
		{
			po->ApplyForce(Vector3D(-10, 0, 0));
		}
		else if (i_Message == string("Add Force Right"))
		{
			po->ApplyForce(Vector3D(10, 0, 0));
		}
		po->SetCurrentVelocity(Vector3D(0));
	}
}