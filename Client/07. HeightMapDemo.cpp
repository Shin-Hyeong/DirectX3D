#include "pch.h"
#include "07. HeightMapDemo.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "ResourceManager.h"

void HeightMapDemo::Init()
{
	// ����� Shader �̸������ϰ� Shader�� ����
	_shader = make_shared<Shader>(L"06. Terrain.fx");

	// Resource �ҷ�����
	_texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Texture\\Terrain\\grass.jpg");
	_heightMap = RESOURCES->Load<Texture>(L"Height", L"..\\Resources\\Texture\\Terrain\\height.png");

	// _heightMap�� ������ ������
	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;
	// _heightMap�� �ȼ� ���� ������
	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	uint8* pixelBuffer = info.GetPixels(); // 1�ȼ��� 8bit�� ǥ����, Texture���� �ٸ�

	// ���� �����͸� �����ϴ� Geometry ����, Texture
	_geometry = make_shared<Geometry<VertexTextureData>>();
	// Grid ����
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU���� Grid ����
	{
		// _geometry�� ���� �����͸� CPU���� ��ȭ�� ��
		 vector<VertexTextureData>& vtx = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());

		 // �� ���� ��ȸ
		 for (int32 z = 0; z < height; z++) // ����
		 {
			 for (int32 x = 0; x < width; x++) // ����
			 {
				 // ������ ����
				 int32 idx = width * z + x;

				 // _heightMap�� 8bit�̱� ������ 0~255�� ���� ������ ������ 255���� ������ ��հ��� ����
				 // * N�� �Ͽ� �ִ� ���̸� ������ (255 / 255) * 25 = �ְ� ���� 25
				 uint8 height = pixelBuffer[idx] / 255.f * 100.f;
				 // ������ ���̸� heightMap ������ ���� �����ɷ� ������. ���� ����
				 vtx[idx].position.y = height;
			 }
		 }
	}

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
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 20.f, -2.f));
	_camera->GetTransform()->SetWorldRotation(Vec3(-25.f, 25.f, 0.f));
	
}

void HeightMapDemo::Update()
{
	_camera->Update();
}

void HeightMapDemo::Render()
{
	// Matrix�� flaot�� 4*4�����̱� ������ float*�� ��ȯ������
	// .fx���� World��� Matrix�� _world������ ������. ��, _world�� float* ���·� ����
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// .fx�� Texture0 ������ SRV ����
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	enum ADDRESS_VALUE
	{
		ADDRESS_WARP = 0,
		ADDRESS_MIRROR = 1,
		ADDRESS_CLAMP = 2,
		ADDRESS_BORDER = 3,
	};
	// 06. Sampler.fx�� Address �� ����
	_shader->GetScalar("Address")->SetInt(ADDRESS_WARP);

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
