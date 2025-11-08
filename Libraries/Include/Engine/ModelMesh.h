#pragma once

// Bone와 Mesh가 가져야할 데이터 구조체

struct ModelBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ModelBone> parent; // Cache

	Matrix transform;
	vector<shared_ptr<ModelBone>> children; // Cache
};

struct ModelMesh
{
	void CreateBuffers();

	wstring name;

	// Mesh
	// vertices, indices정보
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L""; // ResourceManager를 통해 찾아 올 수 있음
	shared_ptr<Material> material;	// Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone;		// Cache;
};