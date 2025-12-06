#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	// 매 frame마다 갱신된 Instancing Mesh를 렌더링함
	// @params gameObjects : Scene에 존재하는 모든 gameObject
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	// InstancingBuffer의 _data를 지움.
	// 매 Frame마다 새로운 데이터를 넣기 위해 기존 데이터 삭제
	void ClearData();

private:
	// MeshRenderer를 가지고 있는 Mesh만 렌더링
	// @params gameObjects : Scene에 배치된 모든 gameObject들
	void RenderMeshRenderer(vector<shared_ptr<GameObject>>& gameObjects);

	// ModelRenderer를 가지고 있는 Mesh만 렌더링
	// @params gameObjects : Scene에 배치된 모든 gameObject들
	void RenderModelRenderer(vector<shared_ptr<GameObject>>& gameObjects);

	// ModelAnimator를 가지고 있는 Mesh만 렌더링
	// @params gameObjects : Scene에 배치된 모든 gameObject들
	void RenderAnimRenderer(vector<shared_ptr<GameObject>>& gameObjects);

private:
	// _buffer에 instanceId의 key의 value에 data를 넣음
	void AddData(InstanceID instanceId, InstancingData& data);

private:
	// 스마트 포인터를 사용하여 Mesh와 Material이 각각 같은 주소값이면 같은 물체으로 판단
	// - 물체 A<abcde, 1234>와 물체B<abcde, 1234> 는 같은 Mesh와 Material을 사용하므로 같은 물체
	map<InstanceID, shared_ptr<InstancingBuffer>> _buffers;
};