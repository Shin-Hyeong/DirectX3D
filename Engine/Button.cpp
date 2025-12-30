#include "pch.h"
#include "Button.h"
#include "MeshRenderer.h"
#include "Material.h"


Button::Button()
	: Super(ComponentType::Button)
{

}

Button::~Button()
{

}

bool Button::Picked(POINT screenPos)
{
	return ::PtInRect(&_rect, screenPos);
}

void Button::Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	// Component을 가지고 있는 GameObject
	auto go = _gameObject.lock();

	float height = GRAPHICS->GetViewport().GetHeight();
	float width = GRAPHICS->GetViewport().GetWidht();

	// Viewport상의 원점에서 화면의 중심을 원점으로 변경함
	float x = screenPos.x - width / 2;
	float y = height / 2 - screenPos.y;
	// 3차원에서의 UI 좌표
	Vec3 position = Vec3(x, y, 0);

	// Transform
	go->GetOrAddTransform()->SetWorldPosition(position);
	go->GetOrAddTransform()->SetWorldScale(Vec3(size.x, size.y, 1));

	// Layer UI에 설정
	go->SetLayerIndex(Layer_UI);

	// Renderer 추가
	if (go->GetMeshRenderer() == nullptr)
		go->AddComponent(make_shared<MeshRenderer>());

	// Material 추가
	if(material)
		go->GetMeshRenderer()->SetMaterial(material);

	// Mesh 추가
	auto mesh = RESOURCES->Get<Mesh>(L"Quad");
	go->GetMeshRenderer()->SetMesh(mesh);
	go->GetMeshRenderer()->SetPass(0);

	// Picking(_rect 범위 설정)
	_rect.left = screenPos.x - size.x / 2;
	_rect.right = screenPos.x + size.x / 2;
	_rect.top = screenPos.y - size.y / 2;		// 위가 값이 작음
	_rect.bottom = screenPos.y + size.y / 2;
}

void Button::AddOnClickedEvent(std::function<void(void)> func)
{
	// 포인터를 들고 있는 함수를 사용하는지 확인
	_onClicked = func;
}

void Button::InvokeOnClicked()
{
	if (_onClicked)
		_onClicked();
}
