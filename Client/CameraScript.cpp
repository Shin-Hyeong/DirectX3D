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

	// ���� ī�޶� ���� ��ǥ
	Vec3 pos = GetTransform()->GetWorldPosition();

	// ������ �̵�
	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetForward() * _speed * DeltaTime;
	// �ڷ� �̵�
	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetForward() * _speed * DeltaTime;
	// ���� �̵�
	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() *_speed* DeltaTime;
	// ������ �̵�
	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DeltaTime;

	GetTransform()->SetWorldPosition(pos);


	// Pitch��
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DeltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DeltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}
	// Yaw
	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DeltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}
	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DeltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}
}

void CameraScript::LateUpdate()
{

}

void CameraScript::FixedUpdate()
{

}
