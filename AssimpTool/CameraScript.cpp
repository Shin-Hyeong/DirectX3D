#include "pch.h"
#include "CameraScript.h"
#include "Transform.h"

CameraScript::CameraScript()
{

}

CameraScript::~CameraScript()
{

}

void CameraScript::Awake()
{

}

void CameraScript::BeginPlay()
{

}

void CameraScript::Update()
{
	float DeltaTime = TIME->GetDeltaTime();

	// 현재 카메라 월드 좌표
	Vec3 pos = GetTransform()->GetWorldPosition();

	// 앞으로 이동
	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetForward() * _speed * DeltaTime;
	// 뒤로 이동
	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetForward() * _speed * DeltaTime;
	// 왼쪽 이동
	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() *_speed* DeltaTime;
	// 오른쪽 이동
	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DeltaTime;

	// Y축 이동
	if (INPUT->GetButton(KEY_TYPE::KEY_3))
		pos += GetTransform()->GetUp() * _speed * DeltaTime;
	if (INPUT->GetButton(KEY_TYPE::KEY_1))
		pos -= GetTransform()->GetUp() * _speed * DeltaTime;

	GetTransform()->SetWorldPosition(pos);


	// Pitch값
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DeltaTime * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DeltaTime * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}
	// Yaw
	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DeltaTime * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}
	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DeltaTime * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}

	
	
}

void CameraScript::LateUpdate()
{

}

void CameraScript::FixedUpdate()
{

}
