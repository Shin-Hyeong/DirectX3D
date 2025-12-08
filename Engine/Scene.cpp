#include "pch.h"
#include "Scene.h"
#include "GameObject.h"


void Scene::Start()
{
	// Start()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->BeginPlay();
	}
}

void Scene::Update()
{
	// Update()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Update();
	}

	// INSTANCING
	vector<shared_ptr<GameObject>> temp;
	temp.insert(temp.end(), objects.begin(), objects.end());
	INSTANCING->Render(temp);
}

void Scene::LateUpdate()
{
	// LateUpdate()함수중에 Object가 추가 / 삭제가 진행되면 문제 생길 수 있기 때문에
	// 한번 복사해서 사용함
	auto objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->LateUpdate();
	}
}

void Scene::Add(shared_ptr<GameObject> object)
{
	_objects.insert(object);


	if (object->GetCamera())
	{
		_camera.insert(object);
	}
	if (object->GetLight())
	{
		_light.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
	_objects.erase(object);

	_camera.erase(object);

	_light.erase(object);
}
