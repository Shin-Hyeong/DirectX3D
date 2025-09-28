#include "pch.h"
#include "02. QuadDemo.h"
#include "GeometryHelper.h"

void QuadDemo::Init()
{
	// 사용할 Shader 이름선택하고 Shader를 생성
	_shader = make_shared<Shader>(L"02. Quad.fx");
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

void QuadDemo::Update()
{

}

void QuadDemo::Render()
{
	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// VertexBuffer을 Pipeline(DeviceContext)에 연결
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// IndexBuffer을 Pipelin(DeviceContext)에 연결
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 원하는 정점의 수만큼 원하는 technique, pass을 적용
	_shader->DrawIndexed(0, 1, _indexBuffer->GetCount(), 0 , 0);
}
