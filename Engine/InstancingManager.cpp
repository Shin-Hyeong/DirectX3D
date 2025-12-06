#include "pch.h"
#include "InstancingManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	ClearData();

	RenderMeshRenderer(gameObjects);
	RenderModelRenderer(gameObjects);
	RenderAnimRenderer(gameObjects);
}

void InstancingManager::ClearData()
{
	for (auto& pair : _buffers)
	{
		pair.second->ClearData();
	}
}

void InstancingManager::RenderMeshRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	// Scene에 있는 Object들 중에서 MeshRenderer가 있는 Object만 저장함
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	// MeshRenderer가 있는것만 찾기
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		// 동일한 Mesh와 Material를 사용하는 gameObject들끼리 모아둠.
		const InstanceID instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	// MeshRenderer가 있는 GameObject 중 
	// Mesh와 Material이 동일한 Object마다 한번 데이터 저장
	for (auto& pair : cache)
	{
		// vec = 동일한 Mesh와 Material를 사용하는 Object들
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		{
			// Mesh와 Material의 주소 pair
			const InstanceID instanceId = pair.first;

			// 동일한 Mesh와 Material를 사용하는 Object들의 world변환행렬을 _buffer에 저장함
			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];

				// gameObject의 World 변환 행렬을 가져옴
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				// _buffer에서 instancedId의 Key값에 데이터가 없으면 삽입
				AddData(instanceId, data);
			}

			// 실질적으로 Rendering 시키기
			// 동일한 mesh와 Material를 사용하는 GameObject중 하나만 대표로 렌더링함
			// - 동일한 Mesh와 Material를 사용하는 모든 GameObject들이 Rendering됨
			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::RenderModelRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	// Scene에 있는 Object들 중에서 MeshRenderer가 있는 Object만 저장함
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	// MeshRenderer가 있는것만 찾기
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelRenderer() == nullptr)
			continue;

		// 동일한 Mesh와 Material를 사용하는 gameObject들끼리 모아둠.
		const InstanceID instanceId = gameObject->GetModelRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	// MeshRenderer가 있는 GameObject 중 
	// Mesh와 Material이 동일한 Object마다 한번 데이터 저장
	for (auto& pair : cache)
	{
		// vec = 동일한 Mesh와 Material를 사용하는 Object들
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		{
			// Mesh와 Material의 주소 pair
			const InstanceID instanceId = pair.first;

			// 동일한 Mesh와 Material를 사용하는 Object들의 world변환행렬을 _buffer에 저장함
			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];

				// gameObject의 World 변환 행렬을 가져옴
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				// _buffer에서 instancedId의 Key값에 데이터가 없으면 삽입
				AddData(instanceId, data);
			}

			// 실질적으로 Rendering 시키기
			// 동일한 mesh와 Material를 사용하는 GameObject중 하나만 대표로 렌더링함
			// - 동일한 Mesh와 Material를 사용하는 모든 GameObject들이 Rendering됨
			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetModelRenderer()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::RenderAnimRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	// Scene에 있는 Object들 중에서 MeshRenderer가 있는 Object만 저장함
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	

	// MeshRenderer가 있는것만 찾기
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelAnimator() == nullptr)
			continue;

		// 동일한 Mesh와 Material를 사용하는 gameObject들끼리 모아둠.
		const InstanceID instanceId = gameObject->GetModelAnimator()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}


	// ModelAnimator가 있는 GameObject 중 
	// Model과 Shader가 동일한 Object별로 DrawCall 진행
	for (auto& pair : cache)
	{
		// 동일한 Object의 TweenDesc정보를 저장
		shared_ptr<InstancedTweenDesc> tweenDesc = make_shared<InstancedTweenDesc>();

		// vec = 동일한 Mesh와 Material를 사용하는 Object들
		const vector<shared_ptr<GameObject>>& vec = pair.second;
		{
			// Mesh와 Material의 주소 pair
			const InstanceID instanceId = pair.first;

			// 동일한 Mesh와 Material를 사용하는 Object들의 world변환행렬을 _buffer에 저장함
			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];

				// gameObject의 World 변환 행렬을 가져옴
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				// _buffer에서 instancedId의 Key값에 데이터 추가
				AddData(instanceId, data);

				// INSTANCING
				// Object의 TweenDesc 갱신
				gameObject->GetModelAnimator()->UpdateTweenData();
				TweenDesc& desc = gameObject->GetModelAnimator()->GetTweenDesc();
				tweenDesc->tweens[i] = desc;
			}

			// 셰이더에 데이터전달
			RENDER->PushTweenData(*tweenDesc.get());

			// 실질적으로 Rendering 시키기
			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetModelAnimator()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::AddData(InstanceID instanceId, InstancingData& data)
{
	// _buffer에 instanceId에 key값에 데이터가 없으면 InstanceBuffer 생성
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	// InstancingBuffer에서 key값(InstanceId)에 data 추가
	_buffers[instanceId]->AddData(data);
}
