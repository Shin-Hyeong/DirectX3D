#include "pch.h"
#include "Converter.h"
// 파일 로드 관련. C++17이상에서 사용 가능
#include <filesystem>
#include "Utils.h"
#include "FileUtils.h"

#include "tinyxml2.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();

}

Converter::~Converter()
{

}

void Converter::ReadAssetFile(wstring file)
{
	// 파일의 위치
	wstring fileStr = _assetPath + file;

	// 파일 경로를 관리하기 좋은 class에 저장
	auto p = ::filesystem::path(fileStr);
	// 해당 경로에 파일이 존재하는지 확인
	assert(::filesystem::exists(p));

	// 여러 옵션이 지원되기 때문에 로드하는데 시간이 걸림
	// - 한번 로드하면 사용자가 만든 형태로 따로 저장하여 사용하는 것이 용이함
	_scene = _importer->ReadFile(
		Utils::ToString(fileStr),		// wstring -> string
		aiProcess_ConvertToLeftHanded | // fbx 읽을때 포맷을 설정
		aiProcess_Triangulate |			// 삼각형 단위로 파싱
		aiProcess_GenUVCoords |			// UV 좌표 생성
		aiProcess_GenNormals |			// 정점의 Normal 연산
		aiProcess_CalcTangentSpace		// 탄젠트 Space 연산
	);

	// 로드 실패시
	assert(_scene != nullptr);
}

void Converter::ExportModelData(wstring savePath)
{
	// 저장한 경로
	wstring finalPath = _modelPath + savePath + L".mesh";
	// RootNode을 -1 -1으로 하여 재귀함수함
	// 재귀함수를 통하여 모든 Bone 계층구조를 순회할 수 있음
	ReadModelData(_scene->mRootNode, -1, -1); 
	// Model의 정점들이 어느 Bone에 영향을 받을지. 얼마나 영향을 받을지
	ReadSkinData();

	// 정점의 정보와 Bone 구조를 .csv 파일로 추출
	{
		FILE* file;
		::fopen_s(&file, "../Vertices.csv", "w");

		for (shared_ptr<asBone>& bone : _bones)
		{
			string name = bone->name;
			::fprintf(file, "%d,%s\n", bone->index, bone->name.c_str());
		}

		::fprintf(file, "\n");

		for (shared_ptr<asMesh>& mesh : _meshes)
		{
			string name = mesh->name;
			::printf("%s\n", name.c_str());

			for (UINT i = 0; i < mesh->vertices.size(); i++)
			{
				Vec3 p = mesh->vertices[i].position;
				Vec4 indices = mesh->vertices[i].blendIndices;
				Vec4 weights = mesh->vertices[i].blendWeights;

				::fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
				::fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
				::fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
			}
		}

		::fclose(file);
	}

	// 메모리에 있던 Model 데이터를 사용자가 설정한 데이터 형식으로 저장함
	WriteModelFile(finalPath);
}

void Converter::ExportMaterialData(wstring savePath)
{
	// 저장한 경로
	wstring finalPath = _texturePath + savePath + L".xml";
	// 로드된 Material 정보를 읽어서 필요한 정보만 material 배열에 저장
	ReadMaterialData();
	// 배열에 저장된 Material 데이터를 사용자가 설정한 데이터 형식으로 저장함
	WriteMaterialData(finalPath);
}

void Converter::ExportAnimationData(wstring savePath, uint32 index)
{
	wstring finalPath = _modelPath + savePath + L".clip";
	// 열고 있는 파일(FBX)에서 추출할려는 Animation의 갯수가 
	// 열고 있는 파일에 저장된 Animation 갯수보다 크면 프로그램 종료
	assert(index < _scene->mNumAnimations);

	shared_ptr<asAnimation> animation = ReadAnimationData(_scene->mAnimations[index]);
	// asAnimation으로 저장된 animation을 별도의 파일로 저장함.
	WriteAnimationData(animation, finalPath);
}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	// node의 계층 구조를 순회하면서 Bone도 같이 정보를 수집함
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// Relative Transform : 직속부모의 좌표계
	// mTransformation[0](Matrix[0]) :  Matrix의 첫번째 주소를 받아와서 16개의 숫자(4x4 행렬)를 저장함
	Matrix transform(node->mTransformation[0]);
	bone->transform = transform.Transpose(); // 반대 반향으로 저장되어 있어 뒤집어야함.
	
	// Local(Root) 좌표계로 변환하기
	Matrix matParent = Matrix::Identity;
	if (parent >= 0) // 해당 node에 부모가 있음
		// 부모의 transform은  Local 좌표계로 되어 있음
		matParent = _bones[parent]->transform; 
	// Local 좌표계로 전환
	bone->transform = bone->transform * matParent;

	_bones.push_back(bone);

	// Mesh. 
	// Bone과 연관된 MeshData 수집
	ReadMeshData(node, index);

	// 트리 구조의 자식들을 순회
	for (uint32 i = 0; i < node->mNumChildren; i++)
		ReadModelData(node->mChildren[i], _bones.size(), index);
}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	// 해당 Node에 Mesh 관련 정보가 없음
	if (node->mNumMeshes < 1) return;

	shared_ptr<asMesh> mesh = make_shared<asMesh>();

	// Name & BoneIndex
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		// 해당 노드가 가지고  있는 Mesh의 Scene 기준으로 된 인덱스 저장
		uint32 index = node->mMeshes[i];
		// 노드가 가지고 있는 Mesh정보(Scene에서)를 가져옴
		const aiMesh* srcMesh = _scene->mMeshes[index];

		// Mesh에서 Material 정보 얻기
		// mMaterials[] : aiScene이 가지고 있는 FBX의 Material 배열
		// mMaterialIndex : mMaterial[]에서 해당 Mesh가 사용하고 있는 Material 인덱스
		// Material Name
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		// IndexBuffer을 저장할때 사용하는 Offset
		const uint32 startVertex = mesh->vertices.size();

		// Mesh의 정점 정보 저장
		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			VertexType vertex;
			// Vertex
			// vertex의 position에 Mesh의 정점 정보를 Vec3만큼 복사
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));

			// vertiecs
			mesh->vertices.push_back(vertex);
		}

		// Index
		// srcMesh->mNumFaces : Mesh의 폴리곤 갯수
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			// 해당 폴리곤에 있는 인덱스 만큼 저장
			for (uint32 k = 0; k < face.mNumIndices; k++)
				// startVertex을 더하여 다른 Mesh와 IndexBuffer를 합칠때 충돌을 없앰
				mesh->indices.push_back(face.mIndices[k] + startVertex);
		}
	}

	_meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	for (uint32 i = 0; i < _scene->mNumMeshes; i++)
	{
		// Mesh 접근
		aiMesh* srcMesh = _scene->mMeshes[i];

		// Mesh에 Bone이 없으면 패스
		if (srcMesh->HasBones() == false)
			continue;


		// 현재 Mesh의 데이터(aiMesh)를 사용자 설정 데이터로 변환한 데이터(asMesh) 가져오기
		// asMesh와 Skinning을 연동하기 위함
		shared_ptr<asMesh> mesh = _meshes[i];

		// 임의로 저장하는 정점별로 Bone의 가중치
		vector<asBoneWeights> tempVertexBoneWeights;
		tempVertexBoneWeights.resize(mesh->vertices.size());

		// 정점과 연관된 Bone과 가중치의 데이터는 Bone에 저장되어있음.
		// Bone을 순회하면서 연관된 Vertex ID, Weight를 구해서 정점에 기록함
		for (uint32 b = 0; b < srcMesh->mNumBones; b++)
		{
			// Mesh에 연관된 Bone
			aiBone* srcMeshBone = srcMesh->mBones[b];
			// Bone의 이름으로 Bone의 Index 구하기
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());

			// srcMeshBone->mNumWeights : Bone이 가지고 있는 정점의 가중치 갯수
			for (uint32 w = 0; w < srcMeshBone->mNumWeights; w++)
			{
				// 해당 Bone의 영향을 받는 Vertex
				uint32 index = srcMeshBone->mWeights[w].mVertexId;
				// 영향 받은 Vertex의 가중치
				float weight = srcMeshBone->mWeights[w].mWeight;

				// 정점에 가중치 추가
				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}
		}

		// 정점의 bone 가중치 최종 결과 계산
		for (uint32 v = 0; v < tempVertexBoneWeights.size(); v++)
		{
			// 정점의 가중치를 4개만 남기고 가중치 총합을 1로 만듦.
			tempVertexBoneWeights[v].Normalize();

			// float형태인 가중치를 Vec4형태로 변형해서 mesh의 asMesh에 저장함
			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeights();
			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
	}
}

shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	shared_ptr<asAnimation> animation = make_shared<asAnimation>();

	// Name
	animation->name = srcAnimation->mName.C_Str();
	// frameRate
	animation->frameRate = static_cast<float>(srcAnimation->mTicksPerSecond);
	// frameCount
	// 10초이면 -> 프레임은 0번 부터 시작함 0을 채우고 동일한 10을 유지하기 위해 + 1을 함
	animation->frameCount = static_cast<uint32>(srcAnimation->mDuration + 1);

	// 임시 저장
	map<string, shared_ptr<asAnimationNode>> cacheAnimNodes;

	// aiAnimation에서 SRT 데이터 추출
	// aiAnimation이 가지고 있는 Channel의 개수만큼 반복
	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++)
	{
		// Animation의 노드 데이터
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// srcNode의 keyFrame데이터(SRT) 추출
		shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// 현재 찾은 노드 중에 제일 긴 시간으로 애니메이션 시간 갱신
		animation->duration = max(animation->duration, node->keyframe.back().time);

		// <AnimNode이름, asAnimationNode>로 임시 저장
		cacheAnimNodes[srcNode->mNodeName.C_Str()] = node;
	}

	// asAnimation의 keyframes을 채움
	ReadKeyframeData(animation, _scene->mRootNode, cacheAnimNodes);

	return animation;
}

shared_ptr<asAnimationNode> Converter::ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode)
{
	shared_ptr<asAnimationNode> node = make_shared<asAnimationNode>();

	// name
	node->name = srcNode->mNodeName;

	// position, Scal, Rotation의 Key값을 동시에 관리하여 3개중 가장 큰값을 keyCount에 저장
	uint32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);
	// 최대 key 개수 만큼 반복
	for (uint32 k = 0; k < keyCount; k++)
	{
		asKeyframeData frameData;

		bool found = false;
		// asNode에 채운 keyframe 개수
		uint32 t = node->keyframe.size();

		// Position
		// fabsf : float형으로 절대값 반환
		// t초에 Position Keyframe의 데이터가 있는지 확인
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			// keyframe의 Time 저장
			frameData.time = (float)key.mTime;
			// Position값 복사
			::memcpy_s(&frameData.translation, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		// t초에 Rotation Keyframe의 데이터가 있는지 확인
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			// keyframe의 Time 저장
			frameData.time = (float)key.mTime;

			// Rotation값 복사
			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		// t초에 Scale Keyframe의 데이터가 있는지 확인
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			// keyframe의 Time 저장
			frameData.time = (float)key.mTime;
			// Scale 값 복사
			::memcpy_s(&frameData.scale, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// 해당 시간에 keyframeData가 있다면 데이터를 추가한 askeyframe를 추가 
		if (found == true)
			node->keyframe.push_back(frameData);
	}

	// Keyframe 늘려주기
	// 오류에 의해 animation의 keyframe이 넘어오지 않을 수 있음.
	if (node->keyframe.size() < animation->frameCount)
	{
		// animation의 keyframe와 asAnimationNode에 저장된 frame 수 차이
		uint32 count = animation->frameCount - node->keyframe.size();
		// 가장 마지막 포즈의 keyframe을 복사
		asKeyframeData keyFrame = node->keyframe.back();

		// 부족한 frame수만큼 마지막 포즈를 유지시킴
		for (uint32 n = 0; n < count; n++)
			node->keyframe.push_back(keyFrame);
	}

	return node;
}

void Converter::ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* srcNode, map<string, shared_ptr<asAnimationNode>>& cache)
{
	// asAnimation을 채울 asKeyframe
	shared_ptr<asKeyframe> keyframe = make_shared<asKeyframe>();

	// boneName
	keyframe->boneName = srcNode->mName.C_Str();
	// asKeyframe에 추가될 SRT 데이터
	shared_ptr<asAnimationNode> findNode = cache[srcNode->mName.C_Str()];


	// keyframe의 갯수만큼 진행
	for (uint32 i = 0; i < animation->frameCount; i++)
	{
		asKeyframeData frameData;

		// asAnimationNode 데이터가 없다면
		if (findNode == nullptr)
		{
			// Node의 Transform을 가져와서 
			Matrix transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (float)i;
			// transfrom 행령를 position, rotation, scale을 각각 분리해서 frameData에 저장
			// 현재 포즈를 유지 시킴.
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);
		}
		else
		{
			// 데이터 이전
			frameData = findNode->keyframe[i];
		}

		keyframe->transforms.push_back(frameData);
	}

	// 애니메이션 키프레임 채우기
	animation->keyframes.push_back(keyframe);

	// 해당 노드의 자식 노드의 keyframe도 진행하여 재귀
	for (uint32 i = 0; i < srcNode->mNumChildren; i++)
		ReadKeyframeData(animation, srcNode->mChildren[i], cache);
}

void Converter::WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	// 쓰기 타입으로 file Open
	file->Open(finalPath, FileMode::Write);

	// Animation 데이터 기입
	file->Write<string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<uint32>(animation->frameCount);

	file->Write<uint32>(animation->keyframes.size());

	for (shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<string>(keyframe->boneName);

		file->Write<uint32>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

void Converter::WriteModelFile(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
	filesystem::create_directory(path.parent_path());
	
	// 파일 읽기 / 쓰기 기능
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	// Bone Data
	file->Write<uint32>(_bones.size());
	for (shared_ptr<asBone>& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	// Mesh Data
	file->Write<uint32>(_meshes.size());
	for (shared_ptr<asMesh>& meshData : _meshes)
	{
		file->Write<string>(meshData->name);
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(meshData->materialName);

		// Vertex Data
		file->Write<uint32>(meshData->vertices.size());
		// vertices 0번부터 가지고 있는 데이터 만큼 기록
		file->Write(&meshData->vertices[0], sizeof(VertexType) * meshData->vertices.size());

		// Index Data
		file->Write<uint32>(meshData->indices.size());
		// indices 0번부터 가지고 있는 데이터 만큼 기록
		file->Write(&meshData->indices[0], sizeof(uint32) * meshData->indices.size());
	}

}

void Converter::ReadMaterialData()
{
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		// 원본 Material 추출
		aiMaterial* srcMaterial = _scene->mMaterials[i];
		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		
		// Name
		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		// Ambient
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.f);

		// Specualr
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w); // Specular 강도

		// Diffuse
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		// Emissive
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.f);

		aiString file;
		// Diffuse Texture
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		// Specular Texture
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		// Normal Texture
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		_materials.push_back(material);
	}
}

void Converter::WriteMaterialData(wstring finalPath)
{
	// 파일 경로
	auto path = filesystem::path(finalPath);

	// 파일 경로에 맞는 폴더가 없으면 생성
	filesystem::create_directory(path.parent_path());

	// 저장할 폴더 이름
	string folder = path.parent_path().string();

	// XML 형식에 맞게 저장하기
	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<asMaterial> material : _materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = nullptr;

		// Name
		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		// DiffuseFile
		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		// SpecularFile
		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		// NormaFile
		element = document->NewElement("NormaFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		// Ambient
		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		// Diffuse
		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		// Specular
		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		// Emissive
		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}

	// 별도의 파일로 저장
	document->SaveFile(Utils::ToString(finalPath).c_str());
}

string Converter::WriteTexture(string saveFolder, string file)
{
	// 파일 이름
	string fileName = filesystem::path(file).filename().string();
	// 폴더 이름
	string folderName = filesystem::path(saveFolder).filename().string();

	// FBX내에 Texture가 포함되어 있으면 따로 분리하여 저장
	const aiTexture* srcTexture = _scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		
		// Height가 0이면
		if (srcTexture->mHeight == 0)
		{
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			// Write 타입으로 파일을 열기
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			// Texture의 Data 쓰기
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;
		
			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;
		
			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			CHECK(hr);
		
			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), DC.Get(), texture.Get(), img);
		
			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			CHECK(hr);
		}
	}
	// 일반적으로 Texture이 따로 분리 되어있을 때
	else
	{
		// 추출할 파일 경로
		string originStr = (filesystem::path(_assetPath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");
		// 저장할 파일 경로
		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		// originStr -> pathStr위치로 파일 복사
		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;
}

uint32 Converter::GetBoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
			return bone->index;
	}

	// 찾는 Bone이 없으면 프로그램 종료
	assert(false);
	return 0;
}
