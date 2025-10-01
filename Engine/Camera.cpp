#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera(ProjectionType type) 
	: Super(ComponentType::Camera), _type(type)
{
	// 화면의 가로/세로 크기 만큼 카메라 범위 조정
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
	// 카메라가 있는 위치
	Vec3 eyePosition = GetTransform()->GetWorldPosition(); 
	// 카메라가 바라보고 있는 방향
	Vec3 focusPosition = eyePosition + GetTransform()->GetForward();
	// 카메라의 윗 방향
	Vec3 upDirection = GetTransform()->GetUp();
	// 카메라 위치, 바라보는 방향, 윗 방향을 통해 View 변환 행렬 구함
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// 혹은 카메라의 matWorld의 역행렬을 사용해도 됨
	// S_MatView = GetTransform()->GetWorldMatrix().Invert();
	
	// 원근형
	if (_type == ProjectionType::Perspective)
	{
		// 카메라의 각도(FOV, 180 / 4) , 화면 비율(800 / 600), 근거리(1), 원거리(100)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width/ _height, _near, _far );
	}
	// 직교형
	else
	{
		// 화면 가로(8), 화면 세로(6), 근거리(0), 원거리(1)
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
	}
}
