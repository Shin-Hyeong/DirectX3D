#include "pch.h"
#include "SkyDemo.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "ModelAnimator.h"
#include "Material.h"
#include "Model.h"
#include "MeshRenderer.h"

void SkyDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"18. SkyDemo.fx");

	// Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(_shader);
		auto texture = RESOURCES->Load<Texture>(L"Sky", L"..\\Resources\\Textures\\Sky01.jpg");
		material->SetDiffuseMap(texture);
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		RESOURCES->Add(L"Sky", material);
	}
	
	// Object
	{
		_obj = make_shared<GameObject>();
		_obj->GetOrAddTransform();
		_obj->AddComponent(make_shared<MeshRenderer>());
		{
			auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
			_obj->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			auto material = RESOURCES->Get<Material>(L"Sky");
			_obj->GetMeshRenderer()->SetMaterial(material);
		}
	}

	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 6.f, -30.f));

	// RenderManager에 Constant 전달
	RENDER->Init(_shader);
}

void SkyDemo::Update()
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

	{
		_obj->Update();
	}
}

void SkyDemo::Render()
{

}

