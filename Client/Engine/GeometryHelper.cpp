#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	// 4개의 정점을 저장하는 배열
	vector<VertexColorData> vtx;
	vtx.resize(4);

	// 정점 위치 설정
	// 13
	// 02
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].color = color;
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].color = color;
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].color = color;
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].color = color;

	// Geometry에 정점을 저장함
	geometry->SetVertices(vtx);

	// 정점 인덱스
	// 시계 방향으로 진행(Culling 설정에 따라)
	vector<uint32> idx = { 1, 2, 0, 2, 1, 3 };

	// Geometry에 인덱스 저장
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 4개의 정점을 저장하는 배열
	vector<VertexTextureData> vtx;
	vtx.resize(4);

	// 정점 위치 설정
	// 13
	// 02
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);

	// Geometry에 정점을 저장함
	geometry->SetVertices(vtx);

	// 정점 인덱스
	// 시계 방향으로 진행(Culling 설정에 따라)
	vector<uint32> idx = { 1, 2, 0, 2, 1, 3 };

	// Geometry에 인덱스 저장
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 수학적으로 Cube의 정점은 8개이지만
	// UV좌표를 사용하기 위해서는 사각형 6개를 Cube형태로 붙이는 방식으로 해야한다.
	// 사각형(4) * 6면(6) = 24 정점

	// Cube의 중점(0, 0, 0)을 기준으로
	float w2 = 0.5f;  
	float h2 = 0.5f; 
	float d2 = 0.5f;

	vector<VertexTextureData> vtx(24);

	// 앞면
	// 1 2
	// 0 3
	vtx[0] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[1] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[2] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[3] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// 뒷면
	// 6 7
	// 5 4
	vtx[4] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f) };
	vtx[5] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[6] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[7] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	// 윗면
	// 9  8
	// 10 11
	vtx[8] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[9] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[10] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[11] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f) };
	// 아랫면
	// 14 13
	// 12 15
	vtx[12] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	vtx[13] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[14] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[15] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f) };
	// 왼쪽면
	vtx[16] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[17] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[18] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[19] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// 오른쪽면
	vtx[20] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[21] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[22] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[23] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f) };

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;	// 정점 0,1,2을 연결해 삼각형
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry, uint32 CircleCount, uint32 SliceCount)
{
	// 1. 구를 구성할 원(고리)을 몇개 그릴지 
	// 2. 원(고리)마다 얼마의 각도마다 정점을 찍을지
	// 3. 정점마다 삼각형으로 연결
	// 
	// DX 물방울 책의 구 제작 공식

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = CircleCount; // 가로 분할. 원(고리)을 몇개 그릴 것인지
	uint32 sliceCount = SliceCount; // 세로 분할. 원(고리)을 몇도마다 정점을 찍을 것인지

	// 정점 데이터 배열
	vector<VertexTextureData> vtx;
	// 정점 데이터
	VertexTextureData v;

	// 북극(구의 맨 위 정점)
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount; // 180 / 원(고리) 갯수 = 180 / 20 = 9개의 원을 그림.
	float sliceAngle = XM_2PI / sliceCount; // 360 / 조각 갯수 = 360 / 20 = 18˚마다 자름

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 원(고리)마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		// 정점을 찍을 원(고리)의 층수(세로 각도). 북극에서 부터 아래로 
		float phi = y * stackAngle;

		// 고리에 위치한 정점. 
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			// 원(고리)에 정점을 찍을 각도
			float theta = x * sliceAngle;

			// 정점의 좌표
			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			// 정점의 UV 좌표
			v.uv = Vec2(deltaU * x, deltaV * y);

			// 정점 데이터 좌표에 추가
			vtx.push_back(v);
		}
	}

	// 남극(구의 맨 아래 정점)
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	// Index
	vector<uint32> idx(36);

	// 북극 인덱스. 북극과 연결된 삼각형
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스. 북/남극과 연결되지 않는 정점들 연결
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스. 남극과 연결된 삼각형
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ)
{
	// 정점 저장 동적 배열
	vector<VertexTextureData> vtx;

	// Grid의 세로
	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		// Girde의 가로
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureData v;
			// 표면에 있는 정점 추가
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(z));

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}
