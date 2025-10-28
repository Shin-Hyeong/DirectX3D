#pragma once

#include "ResourceBase.h"

class Shader;
class Texture;
class Mesh;
class Material;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);
public:
	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	ResourceType GetResourceType();

private:
	void CreateDefaultMesh();

private:
	wstring _resourcePath;

private:
	using KeyObjMap = map<wstring/*key*/, shared_ptr<ResourceBase>>;
	// 리소스 자원 이름과 리소스 타입으로 RESOURCE_TYPE_COUNT(5)만큼 생성
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

// 저장된 리소스 가져오기 OR 없으면 경로에서 리소스 가져온 후 저장
template<typename T>
shared_ptr<T>
ResourceManager::Load(const wstring& key, const wstring& path)
{
	// 해당 리소스의 타입 번호를 가져옴.
	auto objectType = GetResourceType<T>();
	// 리소스 저장 배열에서 리소스 타입에 맞는 Map을 가져옴
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	// Map에서 찾는 Key값을 가진 리소스가 있는지 확인
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		// 있으면 리소스를 반환
		return static_pointer_cast<T>(findIt->second);

	// 저장된 리소스가 없으면 ResourceManager에 저장
	shared_ptr<T> object = make_shared<T>();
	// path 경로에서 리소스 불러오기
	object->Load(path);
	// Map에 저장
	keyObjMap[key] = object;

	return object;
}

// 불러온 리소스를 저장
template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	// 리소스 타입 
	ResourceType resourceType = GetResourceType<T>();
	// 리소스 저장 배열에서 리소스 타입에 맞는 Map을 가져옴
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// Map에 중복되는 리소스가 있는지 확인
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	// 중복된 리소스가 없으면 추가
	keyObjMap[key] = object;
	return true;
}

// _resources배열에서 맞는 키값에 있는 리소스를 반환
template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	// 리소스 타입
	ResourceType resourceType = GetResourceType<T>();
	// 리소스 저장 배열에서 리소스 타입에 맞는 Map을 가져옴
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// Map에서 찾는 Key값을 가진 리소스가 있는지 확인
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

// ResourceType에 맞는 인덱스 번호
template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;


	assert(false);
	return ResourceType::None;
}

