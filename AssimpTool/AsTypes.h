#pragma once

// 메모리에 있는 3D 모델 데이터를 
// 사용자의 CustomData 양식으로 저장하기 위한 구조체

// 정점 정보 : 정점, 텍스트(UV), Normal, Tangent, Blend(Animation)
using VertexType = VertexTextureNormalTangentBlendData;

struct asBone
{
	string			name;			// Bone 이름
	int32			index = -1;		// Bone 번호
	int32			parent = -1;	// 부모의 Bone 번호
	Matrix			transform;		// Bone을 본인의 좌표계->로컬좌표계로 변환하는 행렬
};

struct asMesh
{
	string				name;				// Mesh 이름
	aiMesh*				mesh;				// Assimp에서 로드한 결과물
	vector<VertexType>	vertices;			// 정점 정보
	vector<uint32>		indices;			// 정점의 인덱스 정보

	// Bone과 Material를 연결하는 정보
	int32				boneIndex;			// Bone의 계층 구조. 어느 뼈대에 있는지
	string				materialName;		// 어떤 Material를 사용중인지
};

struct asMaterial
{
	string		name;				// Material 이름
	Color		ambient;			
	Color		diffuse;
	Color		specular;
	Color		emissive;
	string		diffuseFile;		// diffuseMap 파일 경로
	string		specularFile;		// specularMap 파일 경로
	string		normalFile;			// normalMap 파일 경로
};