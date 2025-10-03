#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	// 4���� ������ �����ϴ� �迭
	vector<VertexColorData> vtx;
	vtx.resize(4);

	// ���� ��ġ ����
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

	// Geometry�� ������ ������
	geometry->SetVertices(vtx);

	// ���� �ε���
	// �ð� �������� ����(Culling ������ ����)
	vector<uint32> idx = { 1, 2, 0, 2, 1, 3 };

	// Geometry�� �ε��� ����
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 4���� ������ �����ϴ� �迭
	vector<VertexTextureData> vtx;
	vtx.resize(4);

	// ���� ��ġ ����
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

	// Geometry�� ������ ������
	geometry->SetVertices(vtx);

	// ���� �ε���
	// �ð� �������� ����(Culling ������ ����)
	vector<uint32> idx = { 1, 2, 0, 2, 1, 3 };

	// Geometry�� �ε��� ����
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// ���������� Cube�� ������ 8��������
	// UV��ǥ�� ����ϱ� ���ؼ��� �簢�� 6���� Cube���·� ���̴� ������� �ؾ��Ѵ�.
	// �簢��(4) * 6��(6) = 24 ����

	// Cube�� ����(0, 0, 0)�� ��������
	float w2 = 0.5f;  
	float h2 = 0.5f; 
	float d2 = 0.5f;

	vector<VertexTextureData> vtx(24);

	// �ո�
	// 1 2
	// 0 3
	vtx[0] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[1] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[2] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[3] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// �޸�
	// 6 7
	// 5 4
	vtx[4] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f) };
	vtx[5] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[6] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[7] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	// ����
	// 9  8
	// 10 11
	vtx[8] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[9] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[10] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[11] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f) };
	// �Ʒ���
	// 14 13
	// 12 15
	vtx[12] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	vtx[13] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[14] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[15] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f) };
	// ���ʸ�
	vtx[16] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[17] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[18] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[19] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// �����ʸ�
	vtx[20] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[21] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[22] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[23] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f) };

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;	// ���� 0,1,2�� ������ �ﰢ��
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry, uint32 CircleCount, uint32 SliceCount)
{
	// 1. ���� ������ ��(��)�� � �׸��� 
	// 2. ��(��)���� ���� �������� ������ ������
	// 3. �������� �ﰢ������ ����
	// 
	// DX ����� å�� �� ���� ����

	float radius = 0.5f; // ���� ������
	uint32 stackCount = CircleCount; // ���� ����. ��(��)�� � �׸� ������
	uint32 sliceCount = SliceCount; // ���� ����. ��(��)�� ����� ������ ���� ������

	// ���� ������ �迭
	vector<VertexTextureData> vtx;
	// ���� ������
	VertexTextureData v;

	// �ϱ�(���� �� �� ����)
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount; // 180 / ��(��) ���� = 180 / 20 = 9���� ���� �׸�.
	float sliceAngle = XM_2PI / sliceCount; // 360 / ���� ���� = 360 / 20 = 18������ �ڸ�

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ��(��)���� ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		// ������ ���� ��(��)�� ����(���� ����). �ϱؿ��� ���� �Ʒ��� 
		float phi = y * stackAngle;

		// ���� ��ġ�� ����. 
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			// ��(��)�� ������ ���� ����
			float theta = x * sliceAngle;

			// ������ ��ǥ
			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			// ������ UV ��ǥ
			v.uv = Vec2(deltaU * x, deltaV * y);

			// ���� ������ ��ǥ�� �߰�
			vtx.push_back(v);
		}
	}

	// ����(���� �� �Ʒ� ����)
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	// Index
	vector<uint32> idx(36);

	// �ϱ� �ε���. �ϱذ� ����� �ﰢ��
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���. ��/���ذ� ������� �ʴ� ������ ����
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

	// ���� �ε���. ���ذ� ����� �ﰢ��
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
	// ���� ���� ���� �迭
	vector<VertexTextureData> vtx;

	// Grid�� ����
	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		// Girde�� ����
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureData v;
			// ǥ�鿡 �ִ� ���� �߰�
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
