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
