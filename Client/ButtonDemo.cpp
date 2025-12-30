#include "pch.h"
#include "ButtonDemo.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "SceneDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Mesh.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Light.h"
#include "Graphics.h"
#include "SphereCollider.h"
#include "Scene.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "Terrain.h"
#include "Define.h"
#include "Button.h"

void ButtonDemo::Init()
{
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	// Main Camera
	{
		// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
		auto _camera = make_shared<GameObject>();
		_camera->GetOrAddTransform();
		// 카메라 추가
		_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
		// 카메라 이동 Script
		_camera->AddComponent(make_shared<CameraScript>());
		// UI는 그리지 않도록 함
		_camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, true);
		_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -5.f));

		CUR_SCENE->Add(_camera);
	}

	// UI Camera
	{
		// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
		auto _camera = make_shared<GameObject>();
		_camera->GetOrAddTransform()->SetWorldPosition(Vec3(0.f, 0.f, -5.f));;
		// 카메라 추가
		_camera->AddComponent(make_shared<Camera>(ProjectionType::Orthographic));
		_camera->GetCamera()->SetNear(1.f);
		_camera->GetCamera()->SetFar(100.f);

		// 모든 Layer를 Culling 하도록 함
		_camera->GetCamera()->SetCullingMaskAll();
		// Layer_UI만 Culling에서 제외함
		_camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, false);

		CUR_SCENE->Add(_camera);
	}
	

	// Light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());

		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.direction = Vec3(0.f, -1.f, 0.f);
		
		light->GetLight()->SetLightDesc(lightDesc);
		CUR_SCENE->Add(light);
	}


	// Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(_shader);
		{
			// CS() 연산된 Texture 가져오기
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

	// UI
	{
		// Button
		{
			auto obj = make_shared<GameObject>();
			// Component 추가
			obj->AddComponent(make_shared<Button>());
			// Button 생성
			obj->GetButton()->Create(Vec2(100, 100), Vec2(100, 100), RESOURCES->Get<Material>(L"Wood"));

			// Click 이벤트 바인딩
			// void(void)함수 바인딩
			//  - Click한 UI를 제거하도록 함수 추가
			obj->GetButton()->AddOnClickedEvent([obj]() {CUR_SCENE->Remove(obj); });  

			CUR_SCENE->Add(obj);
		} 
	}


	// Mesh Objects
	{
		// Mesh
		{
			// Object
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(-1.f, 0, 0));
			// MeshRenderer의 Update()에서 DrawCall이 발생함
			// MeshRenderer에 Mesh와 Material이 있어야 보이도록 그려짐
			obj->AddComponent(make_shared<MeshRenderer>());
			{
				obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Wood"));
			}
			{
				auto mesh = RESOURCES->Get<Mesh>(L"Cube");
				obj->GetMeshRenderer()->SetMesh(mesh);
				obj->GetMeshRenderer()->SetPass(0);
			}

			// Collider
			{
				obj->GetOrAddTransform()->SetWorldRotation(Vec3(0, 45, 0));

				auto collider = make_shared<OBBBoxCollider>();
				// Box 크기
				collider->GetBoundingBox().Extents = Vec3(0.5f);
				// Box 회전값
				collider->GetBoundingBox().Orientation = Quaternion::CreateFromYawPitchRoll(45, 0, 0);
				obj->AddComponent(collider);
			}

			CUR_SCENE->Add(obj);
		}
	}

	// RenderManager에 Constant 전달
	// RENDER->Init(_shader);
}

void ButtonDemo::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		int32 mouseX = INPUT->GetMousePos().x;
		int32 mouseY = INPUT->GetMousePos().y;

		// Picking
		auto pickObj = CUR_SCENE->Pick(mouseX, mouseY);
		if (pickObj)
		{
			CUR_SCENE->Remove(pickObj);
		}
	}
}

void ButtonDemo::Render()
{

}

void MoveScript::Update()
{
	auto pos = GetTransform()->GetWorldPosition();
	pos.x -= DT * 1.0f;
	GetTransform()->SetWorldPosition(pos);
}

