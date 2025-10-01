#include "pch.h"
#include "03. ConstBufferDemo.h"
#include "GeometryHelper.h"

void ConstBufferDemo::Init()
{
	// 사용할 Shader 이름선택하고 Shader를 생성
	_shader = make_shared<Shader>(L"03. ConstBuffer.fx");
	// 정점 데이터를 저장하는 Geometry 생성
	_geometry = make_shared<Geometry<VertexColorData>>();
	// Geometry에 데이터 삽입
	GeometryHelper::CreateQuad(_geometry, Color(1.f, 0.f, 0.f , 1.f));

	// vertexBuffer 생성
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer 생성
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}

void ConstBufferDemo::Update()
{
	float DeltaTime = TIME->GetDeltaTime();

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		// DeltaTime을 곱하여 컴퓨터 성능에 따른 변화를 최소화함.
		_translation.x -= 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		// DeltaTime을 곱하여 컴퓨터 성능에 따른 변화를 최소화함.
		_translation.x += 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::W))
	{
		// DeltaTime을 곱하여 컴퓨터 성능에 따른 변화를 최소화함.
		_translation.y += 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		// DeltaTime을 곱하여 컴퓨터 성능에 따른 변화를 최소화함.
		_translation.y -= 3.f * DeltaTime;
	}

	// SRT
	// Vec3을 Matrix(4 x 4)으로변화해줌
	_world = Matrix::CreateTranslation(_translation);
}

void ConstBufferDemo::Render()
{
	// Matrix은 flaot의 4*4형태이기 때문에 float*로 변환가능함
	// .fx에서 World라는 Matrix를 _world값으로 설정함. 단, _world를 float* 형태로 받음
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&_view);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&_projection);

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// VertexBuffer을 Pipeline(DeviceContext)에 연결
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// IndexBuffer을 Pipelin(DeviceContext)에 연결
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 원하는 정점의 수만큼 원하는 technique, pass을 적용
	// technique, pass, 적용할 Index 갯수, 시작 Index 번호, 시작 정점 위치
	_shader->DrawIndexed(0, 1, _indexBuffer->GetCount(), 0 , 0);
}
