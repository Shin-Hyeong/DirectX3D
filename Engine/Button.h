#pragma once
#include "Component.h"
class Button : public Component
{
	using Super = Component;

public:
	Button();
	virtual ~Button();

	// 클릭한 위치가 설정한 RECT범위 안에 있는지
	// screenPos을 Pivot으로 설정한 RECT 범위 안을 클릭했는지
	// @params screenPos : button의 좌표
	bool Picked(POINT screenPos);

	// Button 생성
	// @pamas sreenPos : Screen상에서 Button 위치
	// @pamas size : Button 크기
	// @pamas material : Button Material
	void Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material);

	// _onClicke에 함수 바인딩
	// static함수나 전역함수는 상관없지만 지역함수나 멤버함수, 람다함수이면 추가 작업이 필요함
	// - 함수 내에서 포인터 변수를 사용하는 함수가 function에 바인딩됨
	// - 포인터 변수가 해제되어도 function에선 포인터를 가지고 있음 -> 해제된 메모리 접근
	// - 스마트 포인터 사용하면 function에 바인딩하여 스마트 포인터가 해제를 시키고 싶어도 못함
	void AddOnClickedEvent(std::function<void(void)> func);
	// _onClicked에 바인딩되어 있으면 바인딩 함수 호출
	void InvokeOnClicked();

private:
	// void로 입력하고 void로 반환하는 함수를 호출 할 수 있음
	// 콜백함수
	std::function<void(void)> _onClicked;

	// Button의 영역
	RECT _rect;
};

