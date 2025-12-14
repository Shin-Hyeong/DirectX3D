#pragma once
#include "Component.h"

class Material;

class Terrain : public Component
{
	using Super = Component;
public:
	Terrain();
	virtual ~Terrain();

	// Grid 생성
	// MeshRenderer가 없으면 추가
	// @parmas sizeX : Terrain의 가로 길이
	// @parmas sizeZ : Terrain의 세로 길이
	// @parmas material : Terrain의 Material
	void Create(int32 sizeX, int32 sizeZ, shared_ptr<Material> material);

	int32 GetSizeX() { return _sizeX; }
	int32 GetSizeZ() { return _sizeZ; }


	bool Pick(int32 screenX, int32 screenY, Vec3& pickPos, float& distance);

private:
	shared_ptr<Mesh> _mesh;
	int32 _sizeX = 0;
	int32 _sizeZ = 0;

};

