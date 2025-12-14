#include "pch.h"
#include "ViewportDemo.h"
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
#include "Scene.h"
#include "Light.h"
#include "TextureBuffer.h"
#include "Viewport.h"


void ViewportDemo::Init()
{
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");


	// Camera
	{
		// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
		auto _camera = make_shared<GameObject>();
		_camera->GetOrAddTransform();
		// 카메라 추가
		_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
		// 카메라 이동 Script
		_camera->AddComponent(make_shared<CameraScript>());
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

	// Mesh
	{
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

		// Mesh
		for (int32 i = 0; i < 1; i++)
		{
			// Object
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(0, 0, 0));
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

			CUR_SCENE->Add(obj);
		}
	}

	// RenderManager에 Constant 전달
	// RENDER->Init(_shader);
}

void ViewportDemo::Update()
{
	static float width= 800.f;
	static float height = 600.f;
	static float x = 0.f;
	static float y = 0.f;

	// Viewport 설정
	ImGui::InputFloat("Width", &width, 10.f);
	ImGui::InputFloat("Height", & height, 10.f);
	ImGui::InputFloat("X", &x, 10.f);
	ImGui::InputFloat("Y", &y, 10.f);

	GRAPHICS->SetViewport(width, height, x, y);

	// 임의이 3D 좌표
	static Vec3 pos = Vec3(0, 0, 0);
	ImGui::InputFloat3("Pos", (float*)&pos);

	// 3D 좌표 -> 2D 좌표
	Viewport& vp = GRAPHICS->GetViewport();
	Vec3 pos2D = vp.Project(pos, Matrix::Identity, Camera::S_MatView, Camera::S_MatProjection);
	ImGui::InputFloat3("Pos2D", (float*)&pos2D);

	{
		Vec3 temp = vp.Unproject(pos2D, Matrix::Identity, Camera::S_MatView, Camera::S_MatProjection);
		ImGui::InputFloat3("Recalc", (float*)&temp);
	}
}

void ViewportDemo::Render()
{

}
