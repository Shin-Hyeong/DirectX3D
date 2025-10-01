#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,	// 원근 투영(원근법 적용, 3D)
	Orthographic,	// 직교 투영(크기가 거리와 상관없이 사야에 들어오면 투영됨, 2D)
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera(ProjectionType type);
	virtual ~Camera();

	virtual void Update() override;

	// 카메라의 뷰, 투영 변환 행렬 갱신
	void UpdateMatrix();

	// Get / Set
	ProjectionType GetProjectionType() { return _type; }
	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }

	void SetProjectionType(ProjectionType type) { _type = type; }
	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

private:
	ProjectionType _type = ProjectionType::Orthographic;

	// View 변환 행렬
	Matrix _matView = Matrix::Identity;
	// Projection  변환 행렬
	Matrix _matProjection = Matrix::Identity;

	// 원근 투영 최소 거리
	float _near = 1.f;
	// 원근 투영 최대 거리
	float _far = 1000.f;
	// FOV, 카메라 시야각도
	float _fov = XM_PI / 4.f;
	// 카메라 가로
	float _width = 0.f;
	// 카메라 세로
	float _height = 0.f;
	
public:
	// 뷰 변환 행렬과 투영 변환 행렬은 카메라만 사용하기 때문에
	static Matrix S_MatView; // View 변환 행렬
	static Matrix S_MatProjection; // 투영 변환 행렬
};