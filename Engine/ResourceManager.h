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
	// ���ҽ� �ڿ� �̸��� ���ҽ� Ÿ������ RESOURCE_TYPE_COUNT(5)��ŭ ����
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

// ����� ���ҽ� �������� OR ������ ��ο��� ���ҽ� ������ �� ����
template<typename T>
shared_ptr<T>
ResourceManager::Load(const wstring& key, const wstring& path)
{
	// �ش� ���ҽ��� Ÿ�� ��ȣ�� ������.
	auto objectType = GetResourceType<T>();
	// ���ҽ� ���� �迭���� ���ҽ� Ÿ�Կ� �´� Map�� ������
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	// Map���� ã�� Key���� ���� ���ҽ��� �ִ��� Ȯ��
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		// ������ ���ҽ��� ��ȯ
		return static_pointer_cast<T>(findIt->second);

	// ����� ���ҽ��� ������ ResourceManager�� ����
	shared_ptr<T> object = make_shared<T>();
	// path ��ο��� ���ҽ� �ҷ�����
	object->Load(path);
	// Map�� ����
	keyObjMap[key] = object;

	return object;
}

// �ҷ��� ���ҽ��� ����
template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	// ���ҽ� Ÿ�� 
	ResourceType resourceType = GetResourceType<T>();
	// ���ҽ� ���� �迭���� ���ҽ� Ÿ�Կ� �´� Map�� ������
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// Map�� �ߺ��Ǵ� ���ҽ��� �ִ��� Ȯ��
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	// �ߺ��� ���ҽ��� ������ �߰�
	keyObjMap[key] = object;
	return true;
}

// _resources�迭���� �´� Ű���� �ִ� ���ҽ��� ��ȯ
template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	// ���ҽ� Ÿ��
	ResourceType resourceType = GetResourceType<T>();
	// ���ҽ� ���� �迭���� ���ҽ� Ÿ�Կ� �´� Map�� ������
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// Map���� ã�� Key���� ���� ���ҽ��� �ִ��� Ȯ��
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

// ResourceType�� �´� �ε��� ��ȣ
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

