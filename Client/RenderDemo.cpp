#include "pch.h"
#include "RenderDemo.h"
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
#include "ModelAnimator.h"

void RenderDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");


	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(5.f, 5.f, -10.f));



	// Animation Model
	{
		// 3D Model Load
		shared_ptr<Model> model = make_shared<Model>();
		model->ReadModel(L"Kachujin/Kachujin");
		model->ReadMaterial(L"Kachujin/Kachujin");
		model->ReadAnimation(L"Kachujin/Idle");
		model->ReadAnimation(L"Kachujin/Run");
		model->ReadAnimation(L"Kachujin/Slash");

		for (int32 i = 0; i < 500; i++)
		{
			// Object
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->GetOrAddTransform()->SetWorldScale(Vec3(0.01f));

			obj->AddComponent(make_shared<ModelAnimator>(_shader));
			{
				obj->GetModelAnimator()->SetModel(model);
				obj->GetModelAnimator()->SetPass(2);
			}

			_objs.push_back(obj);
		}
	}


	// 3D Model
	{
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
				obj->GetModelRenderer()->SetPass(1);
			}

			_objs.push_back(obj);
		}
	}


	// Mesh
	{
		// Material
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(_shader);
			{
				// 필요한 Texture를 불러와서 Material에 적용
				auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Textures\\Wood.jpg");
				material->SetDiffuseMap(texture);
			}
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(1.f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f);

			// ResourceManager에 Material 저장
			RESOURCES->Add(L"Wood", material);
		}

		// Mesh
		for (int32 i = 0; i < 500; i++)
		{
			// Object
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
			// MeshRenderer의 Update()에서 DrawCall이 발생함
			// MeshRenderer에 Mesh와 Material이 있어야 보이도록 그려짐
			obj->AddComponent(make_shared<MeshRenderer>());
			{
				obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Wood"));
			}
			{
				auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
				obj->GetMeshRenderer()->SetMesh(mesh);
				obj->GetMeshRenderer()->SetPass(0);
			}

			_objs.push_back(obj);
		}
	}

	// RenderManager에 Constant 전달
	//RENDER->Init(_shader);
}

void RenderDemo::Update()
{
	_camera->Update();

	// Camera View행렬, Projection행렬 업데이트
	//RENDER->Update();

	// Light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.direction = Vec3(0.f, -1.f, 0.f);
		//RENDER->PushLightData(lightDesc);
	}

	// Scene에 있는 모든 GameObject를 전달하여 
	// 동일한 Mesh와 Material를 사용하는 Object들끼리 묶어서 
	// InstancingManager를 통해 렌더링함
	// - GameObject들이 직접적으로 Update()하면 안됨
	INSTANCING->Render(_objs);
}

void RenderDemo::Render()
{

}
