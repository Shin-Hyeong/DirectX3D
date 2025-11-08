#pragma once

class GameObject;
class Transform;	

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	ModelRenderer,
	Camera,
	Animator,

	// 위로는 고정적으로 사용되는 Component
	Script,

	End, // Script는 여러개를 사용할 수 있기 때문에
};

enum
{
	// 고정적으로 사용하는 Component 개수
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1 
};


// GameObject에 추가하는 기능(부품)중 최상위 Class

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Awake() {}		// GameObject가 레벨에 배치되었을때
	virtual void BeginPlay() {}; // 게임이 실행되었을 때
	virtual void Update() {}; // 매 프레임마다
	virtual void LateUpdate() {}; // Tick 보다 늦게 실행되는 Tick함수
	virtual void FixedUpdate() {} // 물리엔진(Collision) 연산

public:
	ComponentType GetType() { return _type; }

	// Get / Set
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject; // GameObject에서 Component의 Private, Protected 영역 사용가능
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	// 해당 Component를 가지고 있는 Actor
	weak_ptr<GameObject>	_gameObject;

	// 해당 Component 타입
	ComponentType _type;

private:

	
};

