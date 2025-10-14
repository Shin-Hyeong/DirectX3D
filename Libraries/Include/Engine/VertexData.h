#pragma once

// 단순 정점
struct VertexData
{
	Vec3 position = { 0, 0, 0 };
};

// 정점과 Texture을 적용할 UV 좌표
struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
};

// 정점과 색상 데이터
struct VertexColorData
{
	Vec3 position = { 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };
};

// 정점과 Texture을 적용할 UV 좌표, normal Vector
struct VertexTextureNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
};