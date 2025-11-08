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

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	// node의 계층 구조를 순회하면서 Bone도 같이 정보를 수집함
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// Relative Transform : 직속부모의 좌표계
	// mTransformation[0](Matrix[0]) :  Matrix의 첫번째 주소를 받아와서 16개의 숫자를 저장함
	Matrix transform(node->mTransformation[0]);
	bone->transform = transform.Transpose(); // 반대 반향으로 저장되어 있어 뒤집어야함.
	
	// Local(Root) 좌표계로 변환하기
	Matrix matParent = Matrix::Identity;
	if (parent >= 0) // 해당 node에 부모가 있음
		// 부모의 transform은  Local 좌표계로 되어 있음
		matParent = _bones[parent]->transform; 
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
		string pathStr = saveFolder + fileName;
		
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
