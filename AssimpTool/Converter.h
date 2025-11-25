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

	// 로드한 파일에서 Animation 관련 정보만 추출
	// @param savePath : 추출한 정보를 저장할 경로
	// @param index : 저장할려는 Animation의 번호, _scene이 가지고 있는 Animation보다 많이 저장하지 않도록 방지
	void ExportAnimationData(wstring savePath, uint32 index = 0);


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

	// Skin관련 정보 로드
	// 정점들이 어느 Bone에 영향을 받아 움직일 것인지
	void ReadSkinData();


private:
	// Animation 관련 정보 로드
	// Assimp의 aiAnimation을 사용자 설정 데이터 구조인 asAnimation으로 변환함
	// @params srcAnimation : Assimp의 Animation 데이터
	shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);

	// Assimp의 AnimationNode에서 t초마다 keyframe의 데이터를 저장해서 asAnimationNode로 반환
	// @params animation : 사용자 설정 Animation 데이터 구조
	// @params srcNode : Assimp의 AnimationNode 데이터 구조
	shared_ptr<asAnimationNode> ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);

	// asAnimation에 asKeyframe을 채우는 함수
	// @params animation : 데이터를 채울 변수
	// @params srcNode : 3D 모델의 Node, RootNode부터 시작해서 자식 노드로 재귀 함수 진행
	// @params cache : ParseAnimationNode()을 통해 채워진 SRT데이터를 가짐.
	void ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* srcNode, map <string, shared_ptr<asAnimationNode>>& cache);


	// 사용자 설정된 데이터를 원하는 경로에 저장
	// @params animation : 저장할 데이터
	// @params finalPath : 저장할 경로
	void WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath);

private:
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

	// Bone의 이름을 가지고 Bone의 Index 구하기
	// @param name : Index을 구할려는 Bone의 이름
	uint32 GetBoneIndex(const string& name);


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

