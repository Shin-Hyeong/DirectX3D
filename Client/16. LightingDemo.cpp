#include "pch.h"
#include "16. LightingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"

void LightingDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"13. Lighting.fx");

	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -10.f));


	// Object 1
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
		// ResourceManager에 저장된 Mesh를 불러와서 MeshRenderer에 설정
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Texture
		auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	// Object 2
	_obj2 = make_shared<GameObject>();
	// Transform 추가, 오른쪽으로 0.5, 뒤로 2.0
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.0f });
	// MeshRenderer 추가
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		// MeshRenderer에 Shader설정
		_obj2->GetMeshRenderer()->SetShader(_shader);
	}
	{
		// ResourceManager에 저장된 Mesh를 불러와서 MeshRenderer에 설정
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Texture
		auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
		_obj2->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);
}

void LightingDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	// Light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.direction = Vec3(0.f, -1.f, 0.f);
		RENDER->PushLightData(lightDesc);
	}

	// _obj
	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.2f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		// desc.emissive = Color(0.3f, 0.f, 0.f, 0.5f);

		RENDER->PushMaterialData(desc);
		_obj->Update();
	}

	// _obj2
	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		// desc.specular = Color(0.5f, 0.5f, 0.5f, 1.f);
		// desc.emissive = Color(1.f, 0.f, 0.f, 1.f);

		RENDER->PushMaterialData(desc);
		_obj2->Update();
	}
}

void LightingDemo::Render()
{


}
