#pragma once
#include "Geometry.h"

// �Ϲ����� ���� 

class GeometryHelper
{
public:
	// �簢��
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);


	// Texture�� ������ ����
	// �簢��
	static void CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry);
	// ť��
	static void CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry);
	// ��
	static void CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// �ٴ�
	// �ܼ��� ���簢���� �ƴ� ǥ�鿡 ������ �������� ť��
	// ������ ��ġ�� �̵����� ��縦 ���� �� ����
	static void CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);

	// Normal
	// �簢��
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// ť��
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// ��
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// �ٴ�
	// �ܼ��� ���簢���� �ƴ� ǥ�鿡 ������ �������� ť��
	// ������ ��ġ�� �̵����� ��縦 ���� �� ����
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);

	// Normal Map
	// �簢��
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	// ť��
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	// ��
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, uint32 CircleCount = 20, uint32 SliceCount = 20);
	// �ٴ�
	// �ܼ��� ���簢���� �ƴ� ǥ�鿡 ������ �������� ť��
	// ������ ��ġ�� �̵����� ��縦 ���� �� ����
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int32 sizeX = 256, int32 sizeZ = 256);
};


