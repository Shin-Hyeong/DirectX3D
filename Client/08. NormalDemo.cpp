#include "pch.h"
#include "08. NormalDemo.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "ResourceManager.h"

void NormalDemo::Init()
{
	// 사용할 Shader 이름선택하고 Shader를 생성
	_shader = make_shared<Shader>(L"07. Normal.fx");

	// Resource 불러오기
	_texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Wood.jpg");

	// 정점 데이터를 저장하는 Geometry 생성, Texture
	_geometry = make_shared<Geometry<VertexTextureNormalData>>();
	// Grid 생성
	GeometryHelper::CreateSphere(_geometry);

	// vertexBuffer 생성
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer 생성
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// Camera을 담을 GameObject 생성 & 위치 추가 & CameraScript 추가
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -2.f));
	// _camera->GetTransform()->SetWorldRotation(Vec3(-25.f, 25.f, 0.f));
	
}

void NormalDemo::Update()
{
	_camera->Update();
}

void NormalDemo::Render()
{
	// Matrix은 flaot의 4*4형태이기 때문에 float*로 변환가능함
	// .fx에서 World라는 Matrix를 _world값으로 설정함. 단, _world를 float* 형태로 받음
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// .fx의 Texture0 변수에 SRV 전달
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
	// .fx의 LightDir 변수에 값 전달
	_shader->GetVector("LightDir")->SetFloatVector((float*)&_lightDir);

	enum ADDRESS_VALUE
	{
		ADDRESS_WARP = 0,
		ADDRESS_MIRROR = 1,
		ADDRESS_CLAMP = 2,
		ADDRESS_BORDER = 3,
	};
	// 06. Sampler.fx의 Address 값 설정
	_shader->GetScalar("Address")->SetInt(ADDRESS_WARP);

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// VertexBuffer을 Pipeline(DeviceContext)에 연결
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	// IndexBuffer을 Pipelin(DeviceContext)에 연결
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 원하는 정점의 수만큼 원하는 technique, pass을 적용
	// technique, pass, 적용할 Index 갯수, 시작 Index 번호, 시작 정점 위치
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0 , 0);
}
