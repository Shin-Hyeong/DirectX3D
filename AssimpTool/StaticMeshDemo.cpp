#include "pch.h"
#include "StaticMeshDemo.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "ModelRenderer.h"
#include "Material.h"
#include "Model.h"

void StaticMeshDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"15. ModelDemo.fx");

	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -10.f));

	// CreateTower();
	CreateTank();

	// RenderManager에 Constant 전달
	RENDER->Init(_shader);
}

void StaticMeshDemo::Update()
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

void StaticMeshDemo::Render()
{

}

void StaticMeshDemo::CreateTower()
{
	shared_ptr<Model> model = make_shared<Model>();
	model->ReadModel(L"Tower/Tower");		// Resources/Models/Tower/Tower.mesh
	model->ReadMaterial(L"Tower/Tower");	// Resources/Texture/Tower/Tower.xml

	// Transform
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetWorldPosition(Vec3(0, 0, 20));
	_obj->GetOrAddTransform()->SetWorldRotation(Vec3(90, 0, 0));
	_obj->GetOrAddTransform()->SetWorldScale(Vec3(1.f));

	// ModelRenderer
	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(model);
		_obj->GetModelRenderer()->SetPass(0);
	}
}

void StaticMeshDemo::CreateTank()
{
	shared_ptr<Model> model = make_shared<Model>();
	model->ReadModel(L"Tank/Tank");		// Resources/Models/Tank/Tank.mesh
	model->ReadMaterial(L"Tank/Tank");	// Resources/Texture/Tank/Tank.xml

	// Transform
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetWorldPosition(Vec3(0, 0, 20));
	_obj->GetOrAddTransform()->SetWorldScale(Vec3(1.f));

	// ModelRenderer
	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(model);
		_obj->GetModelRenderer()->SetPass(1);
	}
}
