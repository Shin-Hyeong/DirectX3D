#pragma once
#include "Component.h"

class MonoBehaviour;
class Transform;
class Camera;
class MeshRenderer;
class Animator;

// 하나의 오브젝트, Component을 추가하지 않으면 빈 깡통과 같음

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	~GameObject();
	
	// 오브젝트가 생성되어 있을때
	void Awake();
	// 게임이 시작할때
	void BeginPlay();
	// 매 프레임마다 
	void Update();
	// Update다음으로 호출, 보통 카메라가 사용함
	// Update에서 모든 물체의 이동이 이루어지고, 카메라가 이동이 이루어짐
	// 카메라도 같이 Update에 사용하면 어느물체는 이동이 안된상태에 카메라가 화면을 담을 수 있음.
	void LateUpdate();
	// 렌더링 관련
	void FixedUpdate();

	// GameObject에 Component 추가
	void AddComponent(shared_ptr<Component> component);

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	// Transform 반환
	shared_ptr<Transform> GetTransform();
	// Transform이 없으면 생성해서 반환
	shared_ptr<Transform> GetOrAddTransform();
	// Camera 반환
	shared_ptr<Camera> GetCamera();
	// Mesh Renderer 반환
	shared_ptr<MeshRenderer> GetMeshRenderer();
	// Animator 반환
	// shared_ptr<Animator> GetAnimator();

private:
	ComPtr<ID3D11Device>						_device;

protected:
	// array<> : std에서 지원하는 배열, 영역을 벗어나거나 디버깅을 진행해줌.
	// 고정적으로 사용하는 component을 저장하는 배열
	// Tranfrom, MeshRenderer, Camera, Animator
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _component;
	// 스크립트들을 저장하는 동적 배열
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

