#include "pch.h"
#include "10. GlobalTestDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"

void GlobalTestDemo::Init()
{
	_shader = make_shared<Shader>(L"08. GlobalTest.fx");

	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -2.f));
	// _camera->GetTransform()->SetWorldRotation(Vec3(-25.f, 25.f, 0.f));

	// Obj
	_obj = make_shared<GameObject>();
	// Transform 추가
	_obj->GetOrAddTransform();
	// MeshRenderer 추가
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		// MeshRenderer에 Shader설정
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		// 기본 도형 생성 및 저장
		RESOURCES->Init();
		// ResourceManager에 저장된 Mesh를 불러와서 MeshRenderer에 설정
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Texture
		auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);
}

void GlobalTestDemo::Update()
{
	_camera->Update();
	RENDER->Update();
	_obj->Update();
}

void GlobalTestDemo::Render()
{


}
