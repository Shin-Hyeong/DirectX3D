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


//////////////
// Skinning //
//////////////

// 정점이 어느 Bone의 영향을 얼마나 받는지 저장하는 구조체
// 정점마다 (BoneIndex, Weight) 최대 4개를 가질 수 있음
struct asBlendWeight
{
	Vec4 indices = Vec4(0, 0, 0, 0);	// 영향을 주는 Bone Index
	Vec4 weights = Vec4(0, 0, 0, 0);	// 각 Bone들이 주는 가중치

	// 구조체의 indices와 weights에 값 넣기
	// @param index : 데이터를 넣을 위치
	// @param boneIndex : indices에 넣을 값
	// @param weights : weights에 넣을 값
	void Set(uint32 index, uint32 boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
			case 0: indices.x = i; weights.x = w; break;
			case 1: indices.y = i; weights.y = w; break;
			case 2: indices.z = i; weights.z = w; break;
			case 3: indices.w = i; weights.w = w; break;
		}
	}
};

// 정점에 영향을 주는 Bone의 갯수가 5개 이상일 경우
// 가중치가 높은 4개만 남도록 함
// AddWeights -> Normalize -> GetBlendWeights 순으로 진행하면 됨.
struct asBoneWeights
{
	using Pair = pair<int32, float>;
	vector<Pair> boneWeights;

	// 새로운 가중치 추가. 내림차순으로 정렬
	void AddWeights(uint32 boneIndex, float weight)
	{
		if (weight <= 0.0f) return;

		// 새로 추가될 weight의 값보다 작은 weight를 가지고 있는 It를 찾음
		auto findIt = ::find_if(boneWeights.begin(), boneWeights.end(),
			[weight](const Pair& p) {return weight > p.second; });

		// 작은 값 앞에 추가. 큰값이 앞으로 오도록 정렬하면서 삽입
		boneWeights.insert(findIt, Pair(boneIndex, weight));
	}

	// 가중치가 큰 4개의 값만 asBlendWeight형태(Vec4)로 만듦
	asBlendWeight GetBlendWeights()
	{
		asBlendWeight blendWeights;

		for (uint32 i = 0; i < boneWeights.size(); i++)
		{
			if (i >= 4)
				break;

			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
		}

		return blendWeights;
	}

	// Bone의 가중치 수치를 통합 1이 되도록 수정
	// (1, 0.3) (2, 0.2) -> (1, 0.6) (2, 0.4)
	void Normalize()
	{
		// 가중치 값이 4개 이상이면 4개로 조정. 4개의 가중치만 적용되도록 함
		if (boneWeights.size() >= 4)
			boneWeights.resize(4);

		// 가중치의 총합
		float totalWeight = 0.f;
		for (const auto& item : boneWeights)
			totalWeight += item.second;

		// Weight의 총합이 1이 되도록 값 수정
		float scale = 1.f / totalWeight;
		for (auto& item : boneWeights)
			item.second *= scale;
	}
};

///////////////
// Animation //
///////////////
// Keyframe에 SRT 정보를 저장하는 데이터 구조
struct asKeyframeData
{
	float time;				// 현재 진행된 Animation 시간
	Vec3 scale;			
	Quaternion rotation;
	Vec3 translation;
};

// Bone과 SRT를 연결하는 데이터 구조
struct asKeyframe
{
	string boneName;						// Bone 이름
	vector<asKeyframeData> transforms;		// SRT
};

// Animation
struct asAnimation
{
	string name;								// Animation 이름
	uint32 frameCount;							// keyframe의 갯수
	float frameRate;							// Animation 재생 프레임
	float duration;								// 몇초 짜리 Animation
	vector<shared_ptr<asKeyframe>> keyframes;	// 프레임마다 Animation이 어떤 정보(SRT)를 가지고 있는지
};

// Cache
// AnimationNode의 keyframe데이터를 추출해서 저장
struct asAnimationNode
{
	aiString name;
	vector<asKeyframeData> keyframe;
};