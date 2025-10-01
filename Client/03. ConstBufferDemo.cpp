#include "pch.h"
#include "03. ConstBufferDemo.h"
#include "GeometryHelper.h"

void ConstBufferDemo::Init()
{
	// ����� Shader �̸������ϰ� Shader�� ����
	_shader = make_shared<Shader>(L"03. ConstBuffer.fx");
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

void ConstBufferDemo::Update()
{
	float DeltaTime = TIME->GetDeltaTime();

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		// DeltaTime�� ���Ͽ� ��ǻ�� ���ɿ� ���� ��ȭ�� �ּ�ȭ��.
		_translation.x -= 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		// DeltaTime�� ���Ͽ� ��ǻ�� ���ɿ� ���� ��ȭ�� �ּ�ȭ��.
		_translation.x += 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::W))
	{
		// DeltaTime�� ���Ͽ� ��ǻ�� ���ɿ� ���� ��ȭ�� �ּ�ȭ��.
		_translation.y += 3.f * DeltaTime;
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		// DeltaTime�� ���Ͽ� ��ǻ�� ���ɿ� ���� ��ȭ�� �ּ�ȭ��.
		_translation.y -= 3.f * DeltaTime;
	}

	// SRT
	// Vec3�� Matrix(4 x 4)���κ�ȭ����
	_world = Matrix::CreateTranslation(_translation);
}

void ConstBufferDemo::Render()
{
	// Matrix�� flaot�� 4*4�����̱� ������ float*�� ��ȯ������
	// .fx���� World��� Matrix�� _world������ ������. ��, _world�� float* ���·� ����
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&_view);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&_projection);

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// VertexBuffer�� Pipeline(DeviceContext)�� ����
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// IndexBuffer�� Pipelin(DeviceContext)�� ����
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���ϴ� ������ ����ŭ ���ϴ� technique, pass�� ����
	// technique, pass, ������ Index ����, ���� Index ��ȣ, ���� ���� ��ġ
	_shader->DrawIndexed(0, 1, _indexBuffer->GetCount(), 0 , 0);
}
