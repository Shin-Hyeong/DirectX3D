#include "pch.h"
#include "TweenDemo.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "ModelAnimator.h"
#include "Material.h"
#include "Model.h"

void TweenDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"17. TweenDemo.fx");

	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 6.f, -30.f));

	CreateKachujin();

	// RenderManager에 Constant 전달
	RENDER->Init(_shader);
}

void TweenDemo::Update()
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

void TweenDemo::Render()
{

}

void TweenDemo::CreateKachujin()
{
	shared_ptr<Model> model = make_shared<Model>();

	model->ReadModel(L"Kachujin/Kachujin");
	model->ReadMaterial(L"Kachujin/Kachujin");
	model->ReadAnimation(L"Kachujin/Idle");
	model->ReadAnimation(L"Kachujin/Run");
	model->ReadAnimation(L"Kachujin/Slash");

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetWorldPosition(Vec3(0, 0, 1));
	_obj->GetOrAddTransform()->SetWorldScale(Vec3(0.05f));

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	{
		_obj->GetModelAnimator()->SetModel(model);
		_obj->GetModelAnimator()->SetPass(0);
	}

}