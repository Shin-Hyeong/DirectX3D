#include "pch.h"
#include "05. TextureDemo.h"
#include "GeometryHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "ResourceManager.h"

void TextureDemo::Init()
{
	// ����� Shader �̸������ϰ� Shader�� ����
	_shader = make_shared<Shader>(L"04. World.fx");
	// ���� �����͸� �����ϴ� Geometry ����, Texture
	_geometry = make_shared<Geometry<VertexTextureData>>();
	// Geometry�� ������ ����
	// GeometryHelper::CreateCube(_geometry);
	// GeometryHelper::CreateSphere(_geometry);
	GeometryHelper::CreateSphere(_geometry, 20, 20);

	// vertexBuffer ����
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer ����
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// Camera�� ���� GameObject ���� & ��ġ �߰� & CameraScript �߰�
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>(ProjectionType::Perspective));
	_camera->AddComponent(make_shared<CameraScript>());
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -2.f));

	// Resource �ҷ�����
	_texture = RESOURCES->Load<Texture>(L"Desert", L"..\\Resources\\Texture\\Crimson Desert.png");
}

void TextureDemo::Update()
{
	_camera->Update();
}

void TextureDemo::Render()
{
	// Matrix�� flaot�� 4*4�����̱� ������ float*�� ��ȯ������
	// .fx���� World��� Matrix�� _world������ ������. ��, _world�� float* ���·� ����
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// .fx�� Texture0 ������ SRV ����
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// VertexBuffer�� Pipeline(DeviceContext)�� ����
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// IndexBuffer�� Pipelin(DeviceContext)�� ����
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���ϴ� ������ ����ŭ ���ϴ� technique, pass�� ����
	// technique, pass, ������ Index ����, ���� Index ��ȣ, ���� ���� ��ġ
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0 , 0);
}
