#pragma once
#include "MonoBehaviour.h"

class CameraScript : public MonoBehaviour
{
public:
	CameraScript();
	~CameraScript();

	virtual void Awake() override;
	virtual void BeginPlay() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FixedUpdate() override;

private:
	float	_speed = 10.f;
};

