#pragma once

class GameObject;
class Transform;	

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	Camera,
	Animator,

	// ���δ� ���������� ���Ǵ� Component
	Script,

	End, // Script�� �������� ����� �� �ֱ� ������
};

enum
{
	// ���������� ����ϴ� Component ����
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1 
};


// GameObject�� �߰��ϴ� ���(��ǰ)�� �ֻ��� Class

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Awake() {}		// GameObject�� ������ ��ġ�Ǿ�����
	virtual void BeginPlay() {}; // ������ ����Ǿ��� ��
	virtual void Update() {}; // �� �����Ӹ���
	virtual void LateUpdate() {}; // Tick ���� �ʰ� ����Ǵ� Tick�Լ�
	virtual void FixedUpdate() {} // ��������(Collision) ����

public:
	ComponentType GetType() { return _type; }

	// Get / Set
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject; // GameObject���� Component�� Private, Protected ���� ��밡��
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	// �ش� Component�� ������ �ִ� Actor
	weak_ptr<GameObject>	_gameObject;

	// �ش� Component Ÿ��
	ComponentType _type;

private:

	
};

