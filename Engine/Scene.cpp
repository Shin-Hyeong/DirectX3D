#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "Camera.h"
#include "Terrain.h"

void Scene::Start()
{
	// Start()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->BeginPlay();
	}
}

void Scene::Update()
{
	// Update()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Update();
	}

	// INSTANCING
	// Update된 Object의 Instancing Render
	vector<shared_ptr<GameObject>> temp;
	temp.insert(temp.end(), objects.begin(), objects.end());
	INSTANCING->Render(temp);
}

void Scene::LateUpdate()
{
	// LateUpdate()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->LateUpdate();
	}

	// Update()에서 이동이나 삽입, 삭제가 일어난 뒤 충돌을 확인함
	CheckCollision();
}

void Scene::Add(shared_ptr<GameObject> object)
{
	_objects.insert(object);


	if (object->GetCamera())
	{
		_camera.insert(object);
	}
	if (object->GetLight())
	{
		_light.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
	_objects.erase(object);

	_camera.erase(object);

	_light.erase(object);
}

shared_ptr<class GameObject> Scene::Pick(int32 screenX, int32 screenY)
{
	// 카메라 가져오기
	shared_ptr<Camera> camera = GetCamera()->GetCamera();

	// 화면의 가로 / 세로
	float width = GRAPHICS->GetViewport().GetWidht();
	float height = GRAPHICS->GetViewport().GetHeight();

	// 투영 변환 행렬
	Matrix projectionMatrix = camera->GetProjectionMatrix();
	// View 변환 행렬
	Matrix viewMatrix = camera->GetViewMatrix();
	// View->World 변환 행렬
	Matrix viewMatrixInv = viewMatrix.Invert();

	// View 좌표계에 좌표로 변환
	float viewX = (2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	// Scene에 있는 모든 GameObject 가져오기
	const auto& gameObjects = GetObjects();

	// 가장 가까운 거리인지 확인하기 위함
	float minDistance = FLT_MAX;
	// Picking된 GameObject
	shared_ptr<GameObject> picked;

	// 모든 GameObject 순회
	for (auto& gameObject : gameObjects)
	{
		// Collider가 없음
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace에서 Ray 정의
		Vec4 rayOrigin = Vec4(0.f, 0.f, 0.f, 1.f); // View Space의 원점 = 카메라 위치
		// View Space 상의 마우스 위치(카메라에서 Ray할려는 방향)
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.f);

		// ray값들을 World 좌표계의 좌표로 만듦
		// Objects들이 World 좌표계에 있기때문에 동일한 좌표계에 맞춤
		Vec3 worldRayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		Vec3 worldRayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		worldRayDir.Normalize();

		// Ray에 대한 정보 설정
		Ray ray = Ray(worldRayOrigin, worldRayDir);

		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(ray, OUT distance) == false)
			continue;

		// 거리가 가장 가까운 GameObject를 찾기
		if (distance < minDistance)
		{
			distance = minDistance;
			picked = gameObject;
		}
	}

	for (auto& gameObject : gameObjects)
	{
		// Terrain Component가 없음
		if (gameObject->GetTerrain() == nullptr)
			continue;

		Vec3 pickPos;
		float distance = 0;
		// Terrain에 대한 Pick 판정 검사
		if (gameObject->GetTerrain()->Pick(screenX, screenY, OUT pickPos, OUT distance) == false)
			continue;

		// 가장 가까운 Terrain을 반환
		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
		}
	}

	return picked;
}

void Scene::CheckCollision()
{
	vector<shared_ptr<BaseCollider>> colliders;

	// 매 프레임마다 GameObject중 Collider가 있는 GameObject만 모음
	for(shared_ptr<GameObject> object : _objects)
	{
		if (object->GetCollider() == nullptr)
			continue;

		colliders.push_back(object->GetCollider());
	}

	// BruteForce
	// 모든 GameObject의 충돌 검사를 매 프레임 진행
	for (int32 i = 0; i < colliders.size(); i++)
	{
		for (int32 j = i + 1; j < colliders.size(); j++)
		{
			shared_ptr<BaseCollider>& other = colliders[j];
			if (colliders[i]->Intersects(other))
			{
				Remove(colliders[j]->GetGameObject());
			}
		}
	}
}
