#include "pch.h"
#include "18. NormalMappingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"


void NormalMappingDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"14. NormalMapping.fx");

	// Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->SetShader(_shader);		
		}
		// Diffuse
		{
			// 필요한 Texture를 불러와서 Material에 적용
			auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
			material->SetDiffuseMap(texture);
		}
		// Normal
		{
			auto texture = RESOURCES->Load<Texture>(L"WoodNormal", L"..\\Resources\\Texture\\Wood_Normal.jpg");
			material->SetNormalMap(texture);
		}

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(0.2f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		// desc.emissive = Color(0.3f, 0.f, 0.f, 0.5f);

		// ResourceManager에 Material 저장
		RESOURCES->Add(L"Wood", material);
	}


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
		// ResourceManager에 저장된 Mesh를 불러와서 MeshRenderer에 설정
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Material
		auto material = RESOURCES->Get<Material>(L"Wood");
		_obj->GetMeshRenderer()->SetMaterial(material);
	}

	// Object 2
	_obj2 = make_shared<GameObject>();
	// Transform 추가, 오른쪽으로 0.5, 뒤로 2.0
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.0f });
	// MeshRenderer 추가
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		// ResourceManager에 저장된 Mesh를 불러와서 MeshRenderer에 설정
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Material
		auto material = RESOURCES->Get<Material>(L"Wood")->Clone(); // 복사해서 수정
		MaterialDesc& desc = material->GetMaterialDesc();
		{
			auto texture = RESOURCES->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
			material->SetDiffuseMap(texture);
		}
		{
			auto texture = RESOURCES->Load<Texture>(L"LeatherNormal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
			material->SetNormalMap(texture);
		}
		// desc.emissive = Vec4(1.0f);

		_obj2->GetMeshRenderer()->SetMaterial(material);
	}

	RENDER->Init(_shader);
}

void NormalMappingDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	// Light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f);
		// lightDesc.emissive = Vec4(1.f, 0.f, 0.f, 1.f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		RENDER->PushLightData(lightDesc);
	}

	// _obj
	{
		_obj->Update();
	}

	// _obj2
	{
		_obj2->Update();
	}
}

void NormalMappingDemo::Render()
{


}
