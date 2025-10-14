#include "pch.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
// #include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
// #include "Animator.h"



GameObject::GameObject()
{
}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _component)
	{
		if(component)
			component->Awake();
	}
	for (shared_ptr<MonoBehaviour>& scripts : _scripts)
	{
		if (scripts)
			scripts->Awake();
	}
}

void GameObject::BeginPlay()
{
	for (shared_ptr<Component>& component : _component)
	{
		if (component)
			component->BeginPlay();
	}
	for (shared_ptr<MonoBehaviour>& scripts : _scripts)
	{
		if (scripts)
			scripts->BeginPlay();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _component)
	{
		if (component)
			component->Update();
	}
	for (shared_ptr<MonoBehaviour>& scripts : _scripts)
	{
		if (scripts)
			scripts->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _component)
	{
		if (component)
			component->LateUpdate();
	}
	for (shared_ptr<MonoBehaviour>& scripts : _scripts)
	{
		if (scripts)
			scripts->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _component)
	{
		if (component)
			component->FixedUpdate();
	}
	for (shared_ptr<MonoBehaviour>& scripts : _scripts)
	{
		if (scripts)
			scripts->FixedUpdate();
	}
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// class�� enable_shared_from_this<className>�� ��ӹ޾ƾ���
	// ����Ʈ �����ͷ� ��������. 
	// shared_ptr<GameObject>(this) �ȵ�. �޸� ������ �߻� ����
	// �����ڿ��� ����ҷ��� �ϸ� ������
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	// ���� Component
	if (index < FIXED_COMPONENT_COUNT)
	{
		_component[index] = component;
	}
	// Script
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT); // index�� ������������ ������ ũ����

	return _component[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	// Transform�� ����
	if (GetTransform() == nullptr)
	{
		// ���� �� �߰�
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Camera);
	return static_pointer_cast<Camera>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::MeshRenderer);
	return static_pointer_cast<MeshRenderer>(component);
}

//shared_ptr<Animator> GameObject::GetAnimator()
//{
//	shared_ptr<Component> component = GetFixedComponent(ComponentType::Animator);
//	return static_pointer_cast<Animator>(component);
//}
