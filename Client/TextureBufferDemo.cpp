#include "pch.h"
#include "TextureBufferDemo.h"
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


void TextureBufferDemo::Init()
{
	// CS() 연산된 Texture SRV를 가져옴
	auto newSrv = MakeComputeShaderTexture();

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
		_camera->GetTransform()->SetWorldPosition(Vec3(5.f, 5.f, -10.f));

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
				auto texture = make_shared<Texture>();
				texture->SetSRV(newSrv);

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

			CUR_SCENE->Add(obj);
		}
	}

	// RenderManager에 Constant 전달
	RENDER->Init(_shader);
}

void TextureBufferDemo::Update()
{

}

void TextureBufferDemo::Render()
{

}

ComPtr<ID3D11ShaderResourceView> TextureBufferDemo::MakeComputeShaderTexture()
{
	// Compute Shader를 생성
	auto shader = make_shared<Shader>(L"26. TextureBufferDemo.fx");

	auto texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Textures\\Wood.jpg");
	// Wood Texture을 TextureBuffer에 전달하여 CS()연산을 시킴.
	shared_ptr<TextureBuffer> textureBuffer = make_shared<TextureBuffer>(texture->GetTexture2D());

	// Shader에 SRV, UAV연결
	shader->GetSRV("Input")->SetResource(textureBuffer->GetSRV().Get());
	shader->GetUAV("Output")->SetUnorderedAccessView(textureBuffer->GetUAV().Get());

	uint32 width = textureBuffer->GetWidth();
	uint32 height = textureBuffer->GetHeight();
	uint32 arraySize = textureBuffer->GetArraySize();

	// width(height)값을 무조건 커버하는 양을 구함.
	// 하나의 ThreadGroup의 크기가 32 * 32이기 때문에 
	// Texture의 width가 35이면 (35 + 31) / 32 = 66 / 32 = 2
	// x가 2가 되도록 하여 남을 수 있지만 절대 부족하게 하지 않는 범위를 구함
	uint32 x = max(1, (width + 31) / 32);
	uint32 y = max(1, (height+ 31) / 32);
	shader->Dispatch(0, 0, x, y, arraySize);

	// CS() 연산된 Texture을 가지고 있는 SRV를 반환
	return textureBuffer->GetOutputSRV();
}
