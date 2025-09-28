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
