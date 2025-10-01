#include "pch.h"
#include "GameObject.h"
#include "Component.h"

Component::Component(ComponentType type) : _type(type)
{

}

Component::~Component()
{

}

// GameObject
shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

// Transform
shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}
