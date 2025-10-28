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
	// �⺻ ���� ���� �� ����
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
			// �ʿ��� Texture�� �ҷ��ͼ� Material�� ����
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

		// ResourceManager�� Material ����
		RESOURCES->Add(L"Wood", material);
	}


	// Camera�� ���� GameObject ���� & ��ġ �߰� & CameraScript �߰�
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	// ī�޶� �߰�
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	// ī�޶� �̵� Script
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -10.f));


	// Object 1
	_obj = make_shared<GameObject>();
	// Transform �߰�
	_obj->GetOrAddTransform();
	// MeshRenderer �߰�
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		// ResourceManager�� ����� Mesh�� �ҷ��ͼ� MeshRenderer�� ����
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
	// Transform �߰�, ���������� 0.5, �ڷ� 2.0
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.0f });
	// MeshRenderer �߰�
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		// ResourceManager�� ����� Mesh�� �ҷ��ͼ� MeshRenderer�� ����
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Material
		auto material = RESOURCES->Get<Material>(L"Wood")->Clone(); // �����ؼ� ����
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
