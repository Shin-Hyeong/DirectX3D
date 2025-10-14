#pragma once
#include "Component.h"

class MonoBehaviour;
class Transform;
class Camera;
class MeshRenderer;
class Animator;

// �ϳ��� ������Ʈ, Component�� �߰����� ������ �� ����� ����

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	~GameObject();
	
	// ������Ʈ�� �����Ǿ� ������
	void Awake();
	// ������ �����Ҷ�
	void BeginPlay();
	// �� �����Ӹ��� 
	void Update();
	// Update�������� ȣ��, ���� ī�޶� �����
	// Update���� ��� ��ü�� �̵��� �̷������, ī�޶� �̵��� �̷����
	// ī�޶� ���� Update�� ����ϸ� �����ü�� �̵��� �ȵȻ��¿� ī�޶� ȭ���� ���� �� ����.
	void LateUpdate();
	// ������ ����
	void FixedUpdate();

	// GameObject�� Component �߰�
	void AddComponent(shared_ptr<Component> component);

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	// Transform ��ȯ
	shared_ptr<Transform> GetTransform();
	// Transform�� ������ �����ؼ� ��ȯ
	shared_ptr<Transform> GetOrAddTransform();
	// Camera ��ȯ
	shared_ptr<Camera> GetCamera();
	// Mesh Renderer ��ȯ
	shared_ptr<MeshRenderer> GetMeshRenderer();
	// Animator ��ȯ
	// shared_ptr<Animator> GetAnimator();

private:
	ComPtr<ID3D11Device>						_device;

protected:
	// array<> : std���� �����ϴ� �迭, ������ ����ų� ������� ��������.
	// ���������� ����ϴ� component�� �����ϴ� �迭
	// Tranfrom, MeshRenderer, Camera, Animator
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _component;
	// ��ũ��Ʈ���� �����ϴ� ���� �迭
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

