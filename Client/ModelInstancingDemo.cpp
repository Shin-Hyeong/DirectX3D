#include "pch.h"
#include "ModelInstancingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "ModelRenderer.h"

void ModelInstancingDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"21. ModelInstancingDemo.fx");


	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(5.f, 5.f, -10.f));


	// 3D Model Load
	shared_ptr<Model> model = make_shared<Model>();
	model->ReadModel(L"Tower/Tower");		// Resources/Models/Tower/Tower.mesh
	model->ReadMaterial(L"Tower/Tower");	// Resources/Texture/Tower/Tower.xml


	// Objects
	for (int32 i = 0; i < 500; i++)
	{
		// Object
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
		obj->GetOrAddTransform()->SetWorldScale(Vec3(0.005f));

		obj->AddComponent(make_shared<ModelRenderer>(_shader));
		{
			obj->GetModelRenderer()->SetModel(model);
		}

		_objs.push_back(obj);
	}

	// RenderManager에 Constant 전달
	RENDER->Init(_shader);
}

void ModelInstancingDemo::Update()
{
	_camera->Update();

	// Camera View행렬, Projection행렬 업데이트
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

	// Scene에 있는 모든 GameObject를 전달하여 
	// 동일한 Mesh와 Material를 사용하는 Object들끼리 묶어서 
	// InstancingManager를 통해 렌더링함
	// - GameObject들이 직접적으로 Update()하면 안됨
	INSTANCING->Render(_objs);
}

void ModelInstancingDemo::Render()
{

}
