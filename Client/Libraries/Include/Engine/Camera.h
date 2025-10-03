#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,	// ���� ����(���ٹ� ����, 3D)
	Orthographic,	// ���� ����(ũ�Ⱑ �Ÿ��� ������� ��߿� ������ ������, 2D)
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera(ProjectionType type);
	virtual ~Camera();

	virtual void Update() override;

	// ī�޶��� ��, ���� ��ȯ ��� ����
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

	// View ��ȯ ���
	Matrix _matView = Matrix::Identity;
	// Projection  ��ȯ ���
	Matrix _matProjection = Matrix::Identity;

	// ���� ���� �ּ� �Ÿ�
	float _near = 1.f;
	// ���� ���� �ִ� �Ÿ�
	float _far = 1000.f;
	// FOV, ī�޶� �þ߰���
	float _fov = XM_PI / 4.f;
	// ī�޶� ����
	float _width = 0.f;
	// ī�޶� ����
	float _height = 0.f;
	
public:
	// �� ��ȯ ��İ� ���� ��ȯ ����� ī�޶� ����ϱ� ������
	static Matrix S_MatView; // View ��ȯ ���
	static Matrix S_MatProjection; // ���� ��ȯ ���
};