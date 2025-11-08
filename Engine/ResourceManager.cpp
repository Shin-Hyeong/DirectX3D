#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Shader.h"
#include <filesystem>
//#include "Mesh.h"

void ResourceManager::Init()
{
	CreateDefaultMesh();
}

shared_ptr<Texture> ResourceManager::GetOrAddTexture(const wstring& key, const wstring& path)
{
	// 저장된 동일한 키값으 가진 데이터가 있는지 확인
	shared_ptr<Texture> texture = Get<Texture>(key);

	// 파일이 없으면 실패
	if (filesystem::exists(filesystem::path(path)) == false)
		return nullptr;

	// 저장된 파일이 있는지 확인
	texture = Load<Texture>(key, path);

	// ResourceManger에 저장된 파일이 없으면 해당 경로에 있는 파일을 불러오기
	if (texture == nullptr)
	{
		texture = make_shared<Texture>();
		texture->Load(path);
		Add(key, texture);
	}

	return texture;
}

void ResourceManager::CreateDefaultMesh()
{
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateQuad();
		Add(L"Quad", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateCube();
		Add(L"Cube", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateSphere();
		Add(L"Sphere", mesh);
	}
}
