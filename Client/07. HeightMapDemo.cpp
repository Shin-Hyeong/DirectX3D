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
	// 사용할 Shader 이름선택하고 Shader를 생성
	_shader = make_shared<Shader>(L"06. Terrain.fx");

	// Resource 불러오기
	_texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Texture\\Terrain\\grass.jpg");
	_heightMap = RESOURCES->Load<Texture>(L"Height", L"..\\Resources\\Texture\\Terrain\\height.png");

	// _heightMap의 사이즈 가져옴
	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;
	// _heightMap의 픽셀 정보 가져옴
	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	uint8* pixelBuffer = info.GetPixels(); // 1픽셀이 8bit로 표현됨, Texture마다 다름

	// 정점 데이터를 저장하는 Geometry 생성, Texture
	_geometry = make_shared<Geometry<VertexTextureData>>();
	// Grid 생성
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU에서 Grid 연산
	{
		// _geometry의 정점 데이터를 CPU에서 변화를 줌
		 vector<VertexTextureData>& vtx = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());

		 // 각 정점 순회
		 for (int32 z = 0; z < height; z++) // 세로
		 {
			 for (int32 x = 0; x < width; x++) // 가로
			 {
				 // 수정할 정점
				 int32 idx = width * z + x;

				 // _heightMap이 8bit이기 때문에 0~255의 값을 가지기 때문에 255으로 나누어 평균값을 구함
				 // * N을 하여 최대 높이를 설정함 (255 / 255) * 25 = 최고 높이 25
				 uint8 height = pixelBuffer[idx] / 255.f * 100.f;
				 // 정점의 높이를 heightMap 연산을 통해 얻은걸로 수정함. 높이 보정
				 vtx[idx].position.y = height;
			 }
		 }
	}

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
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 20.f, -2.f));
	_camera->GetTransform()->SetWorldRotation(Vec3(-25.f, 25.f, 0.f));
	
}

void HeightMapDemo::Update()
{
	_camera->Update();
}

void HeightMapDemo::Render()
{
	// Matrix은 flaot의 4*4형태이기 때문에 float*로 변환가능함
	// .fx에서 World라는 Matrix를 _world값으로 설정함. 단, _world를 float* 형태로 받음
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// .fx의 Texture0 변수에 SRV 전달
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

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
