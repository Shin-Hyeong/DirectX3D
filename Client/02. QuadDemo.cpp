#include "pch.h"
#include "02. QuadDemo.h"
#include "GeometryHelper.h"

void QuadDemo::Init()
{
	// ����� Shader �̸������ϰ� Shader�� ����
	_shader = make_shared<Shader>(L"02. Quad.fx");
	// ���� �����͸� �����ϴ� Geometry ����
	_geometry = make_shared<Geometry<VertexColorData>>();
	// Geometry�� ������ ����
	GeometryHelper::CreateQuad(_geometry, Color(1.f, 0.f, 0.f , 1.f));

	// vertexBuffer ����
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer ����
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

	// VertexBuffer�� Pipeline(DeviceContext)�� ����
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// IndexBuffer�� Pipelin(DeviceContext)�� ����
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���ϴ� ������ ����ŭ ���ϴ� technique, pass�� ����
	_shader->DrawIndexed(0, 1, _indexBuffer->GetCount(), 0 , 0);
}
