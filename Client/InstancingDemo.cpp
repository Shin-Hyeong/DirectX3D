#include "pch.h"
#include "InstancingDemo.h"
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

void InstancingDemo::Init()
{
	// 기본 도형 생성 및 저장
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"19. InstancingDemo.fx");


	// Camera
	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// 카메라 추가
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// 카메라 이동 Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(5.f, 5.f, -10.f));



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

		// INSTANCING
		_material = material;
	}

	for (int32 i = 0; i < 10000; i++)
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

			// INSTANCING
			_mesh = mesh;
		}

		_objs.push_back(obj);
	}

	// RenderManager에 Constant 전달
	// RENDER->Init(_shader);


	// INSTANCING
	_instanceBuffer = make_shared<VertexBuffer>();

	// obj의 월드 좌표 저장
	for (auto& obj : _objs)
	{
		Matrix world = obj->GetTransform()->GetWorldMatrix();
		_worlds.push_back(world);
	}

	// slot 1로 전달하여 VS_IN()에서 world의 구조체 멤버에 전달할 수 있음.
	_instanceBuffer->Create(_worlds, 1);
}

void InstancingDemo::Update()
{
	_camera->Update();

	// Camera View행렬, Projection행렬 업데이트
	// RENDER->Update();

	// Light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.direction = Vec3(0.f, -1.f, 0.f);
		// RENDER->PushLightData(lightDesc);
	}

	// MaterialDesc를 Shader에 전달하고
	// DiffuseMap, NormalMap, SpecularMap이 있다면 SRV를 통해 전달함
	_material->Update();

	// Buffer에 있는 데이터를 다음 DrawCall에 사용할 것
	// IA에 연결함
	_mesh->GetVertexBuffer()->PushData();	// 원본 데이터의 정점 데이터 전달
	_instanceBuffer->PushData();			// Instancing Transform 데이터 전달
	_mesh->GetIndexBuffer()->PushData();	// 원본 데이터의 Index 데이터 전달

	_shader->DrawIndexedInstanced(0, 0, _mesh->GetIndexBuffer()->GetCount(), _objs.size());
}

void InstancingDemo::Render()
{

}
