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

// Normal Map를 추가한 구조체
struct VertexTextureNormalTangentData
{
	Vec3 position = { 0, 0, 0 };	// 정점 위치
	Vec2 uv = { 0, 0 };				// Texture를 맵핑할 uv
	Vec3 normal = { 0, 0, 0 };		// 정점의 normal
	Vec3 tangent = { 0, 0, 0 };		// Tangent Space
};

// Blend(Animation)관련 추가
struct VertexTextureNormalTangentBlendData
{
	Vec3 position = { 0, 0, 0 };			// 정점 위치
	Vec2 uv = { 0, 0 };						// Texture를 맵핑할 uv
	Vec3 normal = { 0, 0, 0 };				// 정점의 normal
	Vec3 tangent = { 0, 0, 0 };				// Tangent Space
	Vec4 blendIndices = { 0, 0, 0, 0 };		// Animation 관련
	Vec4 blendWeights = { 0, 0, 0, 0 };
};

using ModelVertexType = VertexTextureNormalTangentBlendData;