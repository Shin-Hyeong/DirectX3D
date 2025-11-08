#pragma once

// Bone와 여러 Material으로 이루어진 복잡한 Mesh

struct ModelBone;
struct ModelMesh;

class Model : public enable_shared_from_this<Model>
{
public:
	Model();
	~Model();

public:
	// @param fileName : 사용자 설정한 데이터가 있는 파일 이름
	void ReadMaterial(wstring fileName);
	void ReadModel(wstring fileName);

	// Material
	// 저장된 Material 갯수
	uint32 GetMaterialCount() { return static_cast<uint32>(_materials.size()); }
	// 저장된 Material 배열
	vector<shared_ptr<Material>>& GetMaterials() { return _materials; }
	// 저장된 Material에서 특정 인덱스 반환
	shared_ptr<Material> GetMaterialByIndex(uint32 index) { return _materials[index]; }
	// 저장된 Material에서 해당 이름으로 된 Material 반환
	shared_ptr<Material> GetMaterialByName(const wstring& name);

	// Mesh
	// 저장된 Mesh 갯수
	uint32 GetMeshCount() { return static_cast<uint32>(_meshes.size()); }
	// 저장된 Mesh 배열
	vector<shared_ptr<ModelMesh>>& GetMeshes() { return _meshes; }
	// 저장된 Mesh에서 특정 인덱스 반환
	shared_ptr<ModelMesh> GetMeshByIndex(uint32 index) { return _meshes[index]; }
	// 저장된 Mesh에서 해당 이름으로 된 Mesh 반환
	shared_ptr<ModelMesh> GetMeshByName(const wstring& name);

	// Bone
	// 저장된 Bone 갯수
	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	// 저장된 Bone 배열
	vector<shared_ptr<ModelBone>>& GetBones() { return _bones; }
	// 저장된 Bone에서 특정 인덱스 반환
	shared_ptr<ModelBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
	// 저장된 Bone에서 해당 이름으로 된 Bone 반환
	shared_ptr<ModelBone> GetBoneByName(const wstring& name);

private:
	// 파일을 한번 불러오면 저장하여 따로 불러올 필요 없도록 함
	void BindCacheInfo();

private:
	// Model와 Texture을 로드할 경로
	wstring _modelPath = L"../Resources/Models/";
	wstring _texturePath = L"../Resources/Textures/";


private:
	// 해당 3D 모델이 가지고 있는 데이터
	shared_ptr<ModelBone>				_root;
	vector<shared_ptr<Material>>		_materials;
	vector<shared_ptr<ModelBone>>		_bones;
	vector<shared_ptr<ModelMesh>>		_meshes;
};

