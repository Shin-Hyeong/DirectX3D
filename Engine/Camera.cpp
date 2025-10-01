#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera(ProjectionType type) 
	: Super(ComponentType::Camera), _type(type)
{
	// ȭ���� ����/���� ũ�� ��ŭ ī�޶� ���� ����
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	// ī�޶� �ִ� ��ġ
	Vec3 eyePosition = GetTransform()->GetWorldPosition(); 
	// ī�޶� �ٶ󺸰� �ִ� ����
	Vec3 focusPosition = eyePosition + GetTransform()->GetForward();
	// ī�޶��� �� ����
	Vec3 upDirection = GetTransform()->GetUp();
	// ī�޶� ��ġ, �ٶ󺸴� ����, �� ������ ���� View ��ȯ ��� ����
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// Ȥ�� ī�޶��� matWorld�� ������� ����ص� ��
	// S_MatView = GetTransform()->GetWorldMatrix().Invert();
	
	// ������
	if (_type == ProjectionType::Perspective)
	{
		// ī�޶��� ����(FOV, 180 / 4) , ȭ�� ����(800 / 600), �ٰŸ�(1), ���Ÿ�(100)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width/ _height, _near, _far );
	}
	// ������
	else
	{
		// ȭ�� ����(8), ȭ�� ����(6), �ٰŸ�(0), ���Ÿ�(1)
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
	}
}
