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
	float GetWidth() { return _width; }
	float GetHeight() { return _height; }

	void SetProjectionType(ProjectionType type) { _type = type; }
	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }



private:
	// 카메라 타입
	ProjectionType _type = ProjectionType::Perspective;

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


// ****************
// Camera Culling
// ****************
public:
	// 현재 Camera가 볼 수 있는 Layer에 있는 GameObject를 가져옴
	void SortGameObject();
	// Forward Rending : 오브젝트를 픽셀화 이후 해당 픽셀에 Scene에서 영향을 주는 Light을 계산해서 적용함
	// - 장점 : 구현이 단순하고 메모리 대역폭이 적게 들음.
	// - 단점 : 광원이 많아지면 픽셀 셰이더 연산이 폭증해 성능 저하가 큼.
	void Render_Forward();


	// 특정 Layer만 On/Off 시킴
	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			// bit OR 연산으로 해당 Layer를 1로 만듦
			_cullingMask |= (1 << layer);
		else
			// bit AND 연산으로 해당 Layer를 0으로 만듦
			_cullingMask &= ~(1 << layer);
	}

	// 모든 Layer를 Culling하도록 설정
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	// 특정 Layer를 Culling하도록 설정
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }

	// GameObject의 Layer가 해당 Camera에서 그려지도록 할지 확인
	// Camera의 _cullingMask(비트 연산)에서 GameObject의 Layer번 bit가 1이면 그리지 않음.
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

private:
	// 그리지 않을 Object를 비트 연산으로 설정함
	// 32bit -> 32개의 정보를 설정가능
	uint32 _cullingMask = 0;

	// 해당 Camera에서 그려질 GameObject
	vector<shared_ptr<GameObject>> _vecForward;
};