#pragma once


class Scene
{
public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);

	unordered_set<shared_ptr<GameObject>> GetObjects() { return _objects; }
	// 메인 카메라 반환
	shared_ptr<GameObject> GetCamera() { return _camera.empty() ? nullptr : *_camera.begin(); }
	// 메인 Light 반환
	shared_ptr<GameObject> GetLight() { return _light.empty() ? nullptr : *_light.begin(); }


private:
	// gameobject가 삽입/삭제가 자주 일어나기 때문에 해쉬 테이블 형태를 사용함
	unordered_set<shared_ptr<GameObject>> _objects;

	// Camera
	unordered_set<shared_ptr<GameObject>> _camera;
	// Light 
	unordered_set<shared_ptr<GameObject>> _light;

};

