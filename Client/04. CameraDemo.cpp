#include "pch.h"
#include "04. CameraDemo.h"
#include "GeometryHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"

void CameraDemo::Init()
{
	// 사용할 Shader 이름선택하고 Shader를 생성
	_shader = make_shared<Shader>(L"04. World.fx");
	// 정점 데이터를 저장하는 Geometry 생성
	_geometry = make_shared<Geometry<VertexColorData>>();
	// Geometry에 데이터 삽입
	GeometryHelper::CreateQuad(_geometry, Color(0.5f, 0.75f, 0.5f , 1.f));

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
}

void CameraDemo::Update()
{
	_camera->Update();
}

void CameraDemo::Render()
{
	// Matrix은 flaot의 4*4형태이기 때문에 float*로 변환가능함
	// .fx에서 World라는 Matrix를 _world값으로 설정함. 단, _world를 float* 형태로 받음
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);

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
