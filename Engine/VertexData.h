#pragma once

// �ܼ� ����
struct VertexData
{
	Vec3 position = { 0, 0, 0 };
};

// ������ Texture�� ������ UV ��ǥ
struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
};

// ������ ���� ������
struct VertexColorData
{
	Vec3 position = { 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };
};

// ������ Texture�� ������ UV ��ǥ, normal Vector
struct VertexTextureNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
};

// Normal Map�� �߰��� ����ü
struct VertexTextureNormalTangentData
{
	Vec3 position = { 0, 0, 0 };	// ���� ��ġ
	Vec2 uv = { 0, 0 };				// Texture�� ������ uv
	Vec3 normal = { 0, 0, 0 };		// ������ normal
	Vec3 tangent = { 0, 0, 0 };		// Tangent Space
};