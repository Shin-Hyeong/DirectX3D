#include "pch.h"
#include "Model.h"
#include "Utils.h"
#include "FileUtils.h"
#include "tinyxml2.h"
#include <filesystem>
#include "Material.h"
#include "ModelMesh.h"

Model::Model()
{

}

Model::~Model()
{

}

// .xml 양식에 맞게 다시 데이터를 가져와야함
void Model::ReadMaterial(wstring fileName)
{
	// 파일 위치
	wstring fullPath = _texturePath + fileName + L".xml";
	auto parentPath = filesystem::path(fullPath).parent_path();

	// 파일 위치에 .xml 파일이 있는지 확인
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();		// <Materials>
	tinyxml2::XMLElement* materialNode = root->FirstChildElement(); // <Material>

	while (materialNode)
	{
		shared_ptr<Material> material = make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement(); // 실질적  Material 정보 노드 계층
		// node의 Name값을 가져와서 Material의 이름으로 적용
		material->SetName(Utils::ToWString(node->GetText())); 

		// Diffuse Texture
		node = node->NextSiblingElement(); // 옆 노드로 이동
		if (node->GetText())
		{
			// DiffsueFile의 데이터를 저장
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				// 경로에 있는 파일을 가져오기 & 저장
				auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetDiffuseMap(texture);
			}
		}

		// Specular Texture
		node = node->NextSiblingElement(); // 옆 노드로 이동
		if (node->GetText())
		{
			// SpecularFile의 데이터를 저장
			wstring texture = Utils::ToWString(node->GetText());
			if (texture.length() > 0)
			{
				wstring textureStr = Utils::ToWString(node->GetText());
				if (textureStr.length() > 0)
				{
					// 경로에 있는 파일을 가져오기 & 저장
					auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
					material->SetSpecularMap(texture);
				}
			}
		}

		// Normal Texture
		node = node->NextSiblingElement(); // 옆 노드로 이동
		if (node->GetText())
		{
			// Normal Texture의 데이터를 저장
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetNormalMap(texture);
			}
		}

		// Ambient
		{
			node = node->NextSiblingElement(); // 옆 노드로 이동

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().ambient = color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement(); // 옆 노드로 이동

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().diffuse = color;
		}

		// Specular
		{
			node = node->NextSiblingElement(); // 옆 노드로 이동

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().specular = color;
		}

		// Emissive
		{
			node = node->NextSiblingElement(); // 옆 노드로 이동

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().emissive = color;
		}

		_materials.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement(); // 옆 노드로 이동
	}

	BindCacheInfo();
}

void Model::ReadModel(wstring fileName)
{
	// 파일 경로 위치
	wstring fullPath = _modelPath + fileName + L".mesh";

	// 파일 열기, Read 버전
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	// Bones
	{
		// 문자열 길이 확인
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelBone> bone = make_shared<ModelBone>();
			bone->index = file->Read<int32>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			_bones.push_back(bone);
		}
	}

	// Mesh
	{
		// 문자열 길이 확인
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();

			// Material
			mesh->materialName = Utils::ToWString(file->Read<string>());

			//VertexData
			{
				// 4바이트 읽기(문자열 길이 확인)
				const uint32 count = file->Read<uint32>();

				// 정점 데이터를 저장할 vector 
				vector<ModelVertexType> vertices;
				vertices.resize(count);

				// 파일에서 데이터 만큼 읽어오기
				void* data = vertices.data();
				file->Read(&data, sizeof(ModelVertexType) * count);
				mesh->geometry->AddVertices(vertices);
			}

			//IndexData
			{
				// 4바이트 읽기(문자열 길이 확인)
				const uint32 count = file->Read<uint32>();

				// 인덱스 데이터를 저장할 vector 
				vector<uint32> indices;
				indices.resize(count);

				// 파일에서 데이터 만큼 읽어오기
				void* data = indices.data();
				file->Read(&data, sizeof(uint32) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			_meshes.push_back(mesh);
		}
	}

	BindCacheInfo();
}

std::shared_ptr<Material> Model::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

std::shared_ptr<ModelMesh> Model::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

std::shared_ptr<ModelBone> Model::GetBoneByName(const wstring& name)
{
	for (auto& bone : _bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

void Model::BindCacheInfo()
{
	// Mesh에 Material 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 Caching 되어 있음
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 Caching 되어 있음
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (_root == nullptr && _bones.size() > 0)
	{
		_root = _bones[0];

		for (const auto& bone : _bones)
		{
			if (bone->parentIndex >= 0)
			{
				// bone의 부모 채우기
				bone->parent = _bones[bone->parentIndex];
				// bone의 부모에 자식에 자신 넣기
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}
}
