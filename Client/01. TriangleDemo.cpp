#include "pch.h"
#include "01. TriangleDemo.h"

void TriangleDemo::Init()
{
	// ����� Shader �̸������ϰ� Shader�� ����
	_shader = make_shared<Shader>(L"01. Triangle.fx");

	{
		_vertices.resize(3);

		// ���� ��ġ
		_vertices[0].position = Vec3{-0.5f, -0.25f, 0.f};
		_vertices[1].position = Vec3{0.f, 0.5f, 0.f};
		_vertices[2].position = Vec3{0.5f, -0.25f, 0.f};
	}

	// vertexBuffer ����
	_buffer = make_shared<VertexBuffer>();
	_buffer->Create(_vertices);
}

void TriangleDemo::Update()
{

}

void TriangleDemo::Render()
{
	uint32 stride = _buffer->GetStride();
	uint32 offset = _buffer->GetOffset();

	// VertexBuffer�� Pipeline(DeviceContext)�� ����
	DC->IASetVertexBuffers(0, 1, _buffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// ���ϴ� ������ ����ŭ ���ϴ� technique, pass�� ����
	_shader->Draw(1, 0, _buffer->GetCount());
}
