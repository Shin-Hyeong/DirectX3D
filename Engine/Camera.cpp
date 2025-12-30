#include "pch.h"
#include "Camera.h"
#include "Scene.h"

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

	// Camera View행렬, Projection행렬 업데이트
	// RENDER->Update();
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
	_matView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// 혹은 카메라의 matWorld의 역행렬을 사용해도 됨
	// S_MatView = GetTransform()->GetWorldMatrix().Invert();
	
	// 원근형
	if (_type == ProjectionType::Perspective)
	{
		// 카메라의 각도(FOV, 180 / 4) , 화면 비율(800 / 600), 근거리(1), 원거리(100)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width/ _height, _near, _far );
	}
	// 직교형
	else
	{
		// 화면 가로(8), 화면 세로(6), 근거리(0), 원거리(1)
		_matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = CUR_SCENE;
	unordered_set<shared_ptr<GameObject>>& gameObjects = scene->GetObjects();

	_vecForward.clear();

	for (auto& gameObject : gameObjects)
	{
		// 그릴 대상이 아님
		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		// Renderer를 가지고 있지 않음
		if (gameObject->GetMeshRenderer() == nullptr &&
			gameObject->GetModelRenderer() == nullptr &&
			gameObject->GetModelAnimator() == nullptr)
			continue;

		// 렌더링할 Object 저장
		_vecForward.push_back(gameObject);
	}
}

void Camera::Render_Forward()
{
	// Shader에 전달될 Viw, Projection 행렬
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	// _vecForward에 저장된 Object들을 Rendering
	GET_SINGLE(InstancingManager)->Render(_vecForward);
}
