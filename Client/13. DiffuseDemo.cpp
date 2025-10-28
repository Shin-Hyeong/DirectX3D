#include "pch.h"
#include "13. DiffuseDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"

void DiffuseDemo::Init()
{
	// �⺻ ���� ���� �� ����
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"10. Lighting_Diffuse.fx");

	// Camera�� ���� GameObject ���� & ��ġ �߰� & CameraScript �߰�
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -10.f));


	// Object 1
	_obj = make_shared<GameObject>();
	// Transform �߰�
	_obj->GetOrAddTransform();
	// MeshRenderer �߰�
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		// MeshRenderer�� Shader����
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		// ResourceManager�� ����� Mesh�� �ҷ��ͼ� MeshRenderer�� ����
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Texture
		auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	// Object 2
	_obj2 = make_shared<GameObject>();
	// Transform �߰�, ���������� 0.5, �ڷ� 2.0
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.0f });
	// MeshRenderer �߰�
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		// MeshRenderer�� Shader����
		_obj2->GetMeshRenderer()->SetShader(_shader);
	}
	{
		// ResourceManager�� ����� Mesh�� �ҷ��ͼ� MeshRenderer�� ����
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		// Texture
		auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");
		_obj2->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);
}

void DiffuseDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	// Light (�ӽ�)
	Vec4 lightDiffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
	_shader->GetVector("LightDiffuse")->SetFloatVector((float*)&lightDiffuse);

	Vec3 lightDir{ 1.0f, -1.f, 1.f };
	lightDir.Normalize();
	_shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);

	{
		// Material (�ӽ�)
		Vec4 material{ 1.0f, 0.0f, 0.0f, 0.0f };; // Vec4(1.0f, 1.0f, 1.0f, 1.0f)
		_shader->GetVector("MaterialDiffuse")->SetFloatVector((float*)&material);

		_obj->Update();
	}

	{
		// Material (�ӽ�)
		Vec4 material(1.0f); // Vec4(1.0f, 1.0f, 1.0f, 1.0f)
		_shader->GetVector("MaterialDiffuse")->SetFloatVector((float*)&material);

		_obj2->Update();
	}
}

void DiffuseDemo::Render()
{


}
