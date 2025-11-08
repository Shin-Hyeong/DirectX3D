#pragma once
#include "AsTypes.h"
// Assimp를 로드하는 기능

class Converter
{
public:
	Converter();
	~Converter();

public:
	// 파일을 로드하는 함수
	// @param file : 찾고자하는 파일 이름
	void ReadAssetFile(wstring file);

	// 로드한 파일에서 Model관련 정보만 추출
	// @param savePath : 추출한 정보를 저장할 경로
	void ExportModelData(wstring savePath);

	// 로드한 파일에서 Material관련 정보만 추출
	// @param savePath : 추출한 정보를 저장할 경로
	void ExportMaterialData(wstring savePath);

private:
	// Bone관련 정보 로드
	// @param node : Assimp의 Node
	// @param index : Node(Bone)의 순서. 임의로 적용한 데이터
	// @param parent : 해당 Bone의 부모 Bone 인덱스
	void ReadModelData(aiNode* node, int32 index, int32 parent);

	// Mesh관련 정보 로드
	// @param node : Assimp의 Node
	// @param bone : 해당 Mesh와 관련된 Bone 인덱스. 임의로 적용한 데이터
	void ReadMeshData(aiNode* node, int32 bone);

	// 사용자가 설정한 데이터 형식으로 데이터를 저장함
	// @param finalPath : 데이터를 저장할 경로
	void WriteModelFile(wstring finalPath);

private:
	// Material 관련 정보를 사용자가 설정한 데이터 형식에 맞게 변환함
	void ReadMaterialData();

	// 추출한 Material 정보를 xml형식으로 따로 저장함
	// @param finalPath : Material 정보를 저장할 파일 경로
	void WriteMaterialData(wstring finalPath);

	// Asset에 있는 Texture을 Texture 파일로 복사이동시킴
	// @param saveFolder : Texture을 복사 이동할 폴더
	// @param file : 복사 이동할 Texture
	string WriteTexture(string saveFolder, string file);

private:
	// 파일의 상대 경로
	wstring _assetPath = L"../Resources/Assets/";
	wstring _modelPath = L"../Resources/Models/";
	wstring _texturePath = L"../Resources/Textures/";

private:
	// Assimp 파일 로드
	shared_ptr<Assimp::Importer>	_importer;	
	// aiScene : Assimp가 Import하여 3D모델의 전체 데이터를 담고 있는 최상위 루트 객체.
	// Import가 파일을 읽으면 aiScene 포인터가 반환됨.
	const aiScene*					_scene;

private:
	// 사용자가 설정한 데이터 형식으로 저장된 Bone 정보 vector
	vector<shared_ptr<asBone>>		_bones;
	// 사용자가 설정한 데이터 형식으로 저장된 Mesh 정보 vector
	vector<shared_ptr<asMesh>>		_meshes;
	// 사용자가 설정한 데이터 형식으로 저장된 Material 정보 vector
	vector<shared_ptr<asMaterial>>	_materials;
};

