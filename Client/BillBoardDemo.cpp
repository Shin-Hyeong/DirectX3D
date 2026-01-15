#include "pch.h"
#include "BillBoardDemo.h"
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
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
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
#include "BillBoard.h"

void BillBoardDemo::Init()
{
	_shader = make_shared<Shader>(L"28. BillBoardDemo.fx");

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

	// BillBoard
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
		obj->AddComponent(make_shared<BillBoard>());
		{
			// Material
			{
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(_shader);
				{
					auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Textures\\grass.png");
					material->SetDiffuseMap(texture);
				}
				MaterialDesc& desc = material->GetMaterialDesc();
				desc.ambient = Vec4(1.f);
				desc.diffuse = Vec4(1.f);
				desc.specular = Vec4(1.f);

				// ResourceManager에 Material 저장
				RESOURCES->Add(L"Grass", material);

				obj->GetBillBoard()->SetMaterial(material);
			}
		}

		for(int i = 0; i < 500; i++)
		{
			Vec2 scale = Vec2(1 + rand() % 3, 1 + rand() % 3); // 1 ~ 4
			Vec2 position = Vec2(-100 + rand() % 200, -100 + rand() % 200); // -100 ~ +100

			// x, z값에 position값을 넣어 땅에 깔린거처럼 펼쳐짐
			obj->GetBillBoard()->Add(Vec3(position.x, scale.y * 0.5f, position.y), scale);
		}

		CUR_SCENE->Add(obj);
	}
	
}

void BillBoardDemo::Update()
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

void BillBoardDemo::Render()
{

}

void BillBoardTest::Update()
{
	// Component을 가지고 있는 GameObject
	auto go = GetGameObject();

	// Camera의 World Position
	Vec3 cameraPos = CUR_SCENE->GetMainCamera()->GetTransform()->GetWorldPosition();
	// BillBoard UI의 World Position
	Vec3 myPos = GetTransform()->GetWorldPosition();

	// 임의의 Up 벡터
	Vec3 up = Vec3(0, 1, 0);

	// BillBoard Forward
	Vec3 forward = cameraPos - myPos;
	float len = forward.LengthSquared();
	if (len < 0.01f) return;
	forward.Normalize();

	// myPos에서 Forward를 바라보는 World 행렬 구하기
	Matrix lookMaterix = Matrix::CreateWorld(myPos, forward, up);

	// 행렬 분리
	Vec3 S, T;
	Quaternion R;
	lookMaterix.Decompose(S, R, T);

	// Quaternion -> Angles
	Vec3 rot = Transform::ToEulerAngles(R);

	// Rotation 설정
	GetTransform()->SetWorldRotation(rot);
}