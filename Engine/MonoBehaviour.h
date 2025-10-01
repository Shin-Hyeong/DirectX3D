#pragma once
#include "Component.h"

// Script 상속받아 사용할 Class (Unity 참조)
class MonoBehaviour : public Component
{
	using Super = Component;

public:
	MonoBehaviour();
	~MonoBehaviour();

	virtual void Awake() override;
	virtual void BeginPlay() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FixedUpdate() override;
};

