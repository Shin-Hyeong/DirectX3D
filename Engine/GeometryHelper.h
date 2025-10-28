#pragma once
#include "Geometry.h"

// 일반적인 도형 

class GeometryHelper
{
public:
	// 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);


	// Texture를 입히는 도형
	// 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry);
	// 큐브
	static void CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry);
	// 구
	static void CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// 바닥
	// 단순한 직사각형이 아닌 표면에 정점이 여러개인 큐브
	// 정점의 위치를 이동시켜 경사를 만들 수 있음
	static void CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);

	// Normal
	// 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// 큐브
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// 구
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// 바닥
	// 단순한 직사각형이 아닌 표면에 정점이 여러개인 큐브
	// 정점의 위치를 이동시켜 경사를 만들 수 있음
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);

	// Normal Map
	// 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	// 큐브
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	// 구
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// 바닥
	// 단순한 직사각형이 아닌 표면에 정점이 여러개인 큐브
	// 정점의 위치를 이동시켜 경사를 만들 수 있음
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);
};


