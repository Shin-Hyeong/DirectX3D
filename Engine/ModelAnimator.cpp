#include "pch.h"
#include "ModelAnimator.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Model.h"
#include "ModelAnimation.h"
#include "Light.h"
#include "Camera.h"

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader) 
	: Super(ComponentType::Animator), _shader(shader)
{
	// test
	_tweenDesc.next.animIndex = rand() % 3;		// Animation 랜덤 선택 후 재생
	_tweenDesc.tweenSumTime += rand() % 100;	// Animation 재생 길이 램덤 설정
}

ModelAnimator::~ModelAnimator()
{

}

// Animation frame간의 보간 없는 버전
// void ModelAnimator::Update()
// {
// 	// Model이 없으면
// 	if (_model == nullptr)
// 		return;
// 
// 	if (_texture == nullptr)
// 		CreateTexture();
// 
// 	// Anim Update
// 	ImGui::InputInt("AnimIdex", &_keyframeDesc.animIndex); // Animation를 변경하는 기능
// 	_keyframeDesc.animIndex %= _model->GetAnimationCount(); // 오버플로우 방지
// 	ImGui::InputInt("CurrentFrame", (int*)&_keyframeDesc.curreFrame);	// 재생중인 Frame을 변경하는 기능
// 	_keyframeDesc.curreFrame %= _model->GetAnimationByIndex(_keyframeDesc.animIndex)->frameCount; // 오버플로우 방지
// 
// 	// ConstantBuffer에 전달
// 	RENDER->PushKeyframeData(_keyframeDesc);
// 
// 	// SRV를 통해 정보 전달
// 	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
// 
// 	// Bones
// 	BoneDesc boneDesc;
// 
// 	const uint32 boneCount = _model->GetBoneCount();
// 	for (uint32 i = 0; i < boneCount; i++)
// 	{
// 		// Model에 저장된 bone
// 		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
// 		boneDesc.transforms[i] = bone->transform;
// 	}
// 	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
// 	RENDER->PushBoneData(boneDesc);
// 
// 
// 	// Transform
// 	// GameObject의 World 좌표 가져오기
// 	auto world = GetTransform()->GetWorldMatrix();
// 	// ConstantBuffer에 해당 위치로 Model 이동
// 	RENDER->PushTransformData(TransformDesc{ world });
// 
// 	// Mesh 마다 갱신
// 	const auto& meshes = _model->GetMeshes();
// 	for (auto& mesh : meshes)
// 	{
// 		// Mesh에 적용된 Material 갱신
// 		if (mesh->material)
// 			mesh->material->Update();
// 
// 		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
// 		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
// 
// 		uint32 stride = mesh->vertexBuffer->GetStride();
// 		uint32 offset = mesh->vertexBuffer->GetOffset();
// 
// 		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
// 		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
// 
// 		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
// 	}
// }

// void ModelAnimator::Update()
// {
// 	// Model이 없으면
// 	if (_model == nullptr)
// 		return;
// 
// 	if (_texture == nullptr)
// 		CreateTexture();
// 
// 	_keyframeDesc.sumTime += DT;
// 
// 
// 	// 현재 frame과 다음 frame을 보간
// 	shared_ptr<ModelAnimation> current = _model->GetAnimationByIndex(_keyframeDesc.animIndex);
// 	if (current)
// 	{
// 		// 1 frame이 몇초인가 = 1 frame당 경과되는 시간
// 		float timePerFrame = 1 / (current->frameRate * _keyframeDesc.speed);
// 		if (_keyframeDesc.sumTime >= timePerFrame)
// 		{
// 			_keyframeDesc.sumTime = 0.f;
// 			// currFrame증가
// 			_keyframeDesc.currFrame = (_keyframeDesc.currFrame + 1) % current->frameCount; 
// 			// 증가된 currFrame의 +1 만큼 nextFrame에 저장
// 			_keyframeDesc.nextFrame = (_keyframeDesc.currFrame + 1) % current->frameCount;
// 		}
// 
// 		// currFrame과 nextFrame의 보간 비율 설정
// 		// frame 재생 경과 시간 / 1 frame 재생 시간
// 		_keyframeDesc.ratio = (_keyframeDesc.sumTime / timePerFrame);
// 	}
// 
// 
// 	// Anim Update
// 	ImGui::InputInt("AnimIdex", &_keyframeDesc.animIndex); // Animation를 변경하는 기능
// 	_keyframeDesc.animIndex %= _model->GetAnimationCount(); // 오버플로우 방지
// 	ImGui::InputFloat("Speed", &_keyframeDesc.speed, 0.5f, 4.f);
// 	
// 
// 	// ConstantBuffer에 전달
// 	RENDER->PushKeyframeData(_keyframeDesc);
// 
// 	// SRV를 통해 정보 전달
// 	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
// 
// 	// Bones
// 	BoneDesc boneDesc;
// 
// 	const uint32 boneCount = _model->GetBoneCount();
// 	for (uint32 i = 0; i < boneCount; i++)
// 	{
// 		// Model에 저장된 bone
// 		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
// 		boneDesc.transforms[i] = bone->transform;
// 	}
// 	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
// 	RENDER->PushBoneData(boneDesc);
// 
// 
// 	// Transform
// 	// GameObject의 World 좌표 가져오기
// 	auto world = GetTransform()->GetWorldMatrix();
// 	// ConstantBuffer에 해당 위치로 Model 이동
// 	RENDER->PushTransformData(TransformDesc{ world });
// 
// 	// Mesh 마다 갱신
// 	const auto& meshes = _model->GetMeshes();
// 	for (auto& mesh : meshes)
// 	{
// 		// Mesh에 적용된 Material 갱신
// 		if (mesh->material)
// 			mesh->material->Update();
// 
// 		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
// 		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
// 
// 		uint32 stride = mesh->vertexBuffer->GetStride();
// 		uint32 offset = mesh->vertexBuffer->GetOffset();
// 
// 		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
// 		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
// 
// 		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
// 	}
// }

// Tween을 적용한 버전
// void ModelAnimator::Update()
// {
// 	// Model이 없으면
// 	if (_model == nullptr)
// 		return;
// 
// 	if (_texture == nullptr)
// 		CreateTexture();
// 
// 	TweenDesc& desc = _tweenDesc;
// 	desc.curr.sumTime += DT;
// 
// 	// 현재 애니메이션
// 	{
// 		// 현재 재생할 Animation 가져오기
// 		shared_ptr<ModelAnimation> currentAnim = _model->GetAnimationByIndex(desc.curr.animIndex);
// 		if (currentAnim)
// 		{
// 			// 1 frame 당 재생할 시간
// 			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
// 			// 지금 frame이 재생할 시간을 초과하면 다음 frame으로 넘어감
// 			if (desc.curr.sumTime >= timePerFrame)
// 			{
// 				desc.curr.sumTime = 0;
// 				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
// 				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
// 			}
// 
// 			// 현재 frame과 다음 frame과의 차이를 경과시간에 따라 정점을 보간함
// 			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
// 		}
// 	}
// 
// 	// 다음 애니메이션이 예약 되어 있다면
// 	if (desc.next.animIndex >= 0)
// 	{
// 		// Tween이 진행
// 		desc.tweenSumTime += DT;
// 		// Tween의 총 시간과 진행된 시간에 비례해서 비율 조정
// 		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;
// 
// 		// 다음 Animation으로 다 넘어감
// 		if (desc.tweenRatio >= 1.f)
// 		{
// 			// 애니메이션 교체 성공
// 			desc.curr = desc.next;
// 			// 설정 초기화
// 			desc.ClearNextAnim();
// 		}
// 		else
// 		{
// 			// 교체중
// 			// 다음 재생할 Animation 가져오기
// 			shared_ptr<ModelAnimation> nextAnim = _model->GetAnimationByIndex(desc.next.animIndex);
// 			desc.next.sumTime += DT;
// 
// 			// 1frame당 재생할 시간
// 			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);
// 
// 			// 다음 Animation의 frame과 frame간의 보간
// 			if (desc.next.ratio >= 1.f)
// 			{
// 				desc.next.sumTime = 0;
// 
// 				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
// 				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
// 			}
// 
// 			desc.next.ratio = desc.next.sumTime / timePerFrame;
// 		}
// 	}
// 
// 
// 	// Anim Update
// 	ImGui::InputInt("AnimIdex", &desc.curr.animIndex); // Animation를 변경하는 기능
// 	_keyframeDesc.animIndex %= _model->GetAnimationCount(); // 오버플로우 방지
// 	
// 	// 다음 Animation 설정
// 	static int32 nextAnimIndex = 0;
// 	if (ImGui::InputInt("NextAnimIndex", &nextAnimIndex))
// 	{
// 		nextAnimIndex %= _model->GetAnimationCount();
// 		desc.ClearNextAnim(); // 기존 데이터 제거
// 		desc.next.animIndex = nextAnimIndex; // 새로운 데이터 넣기
// 	}
// 	// Animation index 오버플로우 방지
// 	if (_model->GetAnimationCount() > 0)
// 		desc.curr.animIndex %= _model->GetAnimationCount();
// 
// 	ImGui::InputFloat("Speed", &desc.curr.speed, 0.5f, 4.f);
// 	ImGui::InputFloat("TweenDuration", &desc.tweenDuration, 0.2f, 4.f);
// 
// 
// 	// ConstantBuffer에 전달
// 	RENDER->PushTweenData(desc);
// 
// 	// SRV를 통해 정보 전달
// 	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
// 
// 	// Bones
// 	BoneDesc boneDesc;
// 
// 	const uint32 boneCount = _model->GetBoneCount();
// 	for (uint32 i = 0; i < boneCount; i++)
// 	{
// 		// Model에 저장된 bone
// 		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
// 		boneDesc.transforms[i] = bone->transform;
// 	}
// 	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
// 	RENDER->PushBoneData(boneDesc);
// 
// 
// 	// Transform
// 	// GameObject의 World 좌표 가져오기
// 	auto world = GetTransform()->GetWorldMatrix();
// 	// ConstantBuffer에 해당 위치로 Model 이동
// 	RENDER->PushTransformData(TransformDesc{ world });
// 
// 	// Mesh 마다 갱신
// 	const auto& meshes = _model->GetMeshes();
// 	for (auto& mesh : meshes)
// 	{
// 		// Mesh에 적용된 Material 갱신
// 		if (mesh->material)
// 			mesh->material->Update();
// 
// 		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
// 		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
// 
// 		uint32 stride = mesh->vertexBuffer->GetStride();
// 		uint32 offset = mesh->vertexBuffer->GetOffset();
// 
// 		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
// 		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
// 
// 		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
// 	}
// }


void ModelAnimator::UpdateTweenData()
{
	TweenDesc& desc = _tweenDesc;
	desc.curr.sumTime += DT;

	// 현재 애니메이션
	{
		// 현재 재생할 Animation 가져오기
		shared_ptr<ModelAnimation> currentAnim = _model->GetAnimationByIndex(desc.curr.animIndex);
		if (currentAnim)
		{
			// 1 frame 당 재생할 시간
			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
			// 지금 frame이 재생할 시간을 초과하면 다음 frame으로 넘어감
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			// 현재 frame과 다음 frame과의 차이를 경과시간에 따라 정점을 보간함
			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션이 예약 되어 있다면
	if (desc.next.animIndex >= 0)
	{
		// Tween이 진행
		desc.tweenSumTime += DT;
		// Tween의 총 시간과 진행된 시간에 비례해서 비율 조정
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		// 다음 Animation으로 다 넘어감
		if (desc.tweenRatio >= 1.f)
		{
			// 애니메이션 교체 성공
			desc.curr = desc.next;
			// 설정 초기화
			desc.ClearNextAnim();
		}
		else
		{
			// 교체중
			// 다음 재생할 Animation 가져오기
			shared_ptr<ModelAnimation> nextAnim = _model->GetAnimationByIndex(desc.next.animIndex);
			desc.next.sumTime += DT;

			// 1frame당 재생할 시간
			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);

			// 다음 Animation의 frame과 frame간의 보간
			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;

				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}
}

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelAnimator::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	// Model이 없으면
	if (_model == nullptr)
		return;

	if (_texture == nullptr)
		CreateTexture();

	{
		// GlobalData
		_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		// Light
		auto lightObj = SCENE->GetCurrentScene()->GetLight();
		if (lightObj)
			_shader->PushLightData(lightObj->GetLight()->GetLightDesc());
	}

	// Animation의 SRV정보를 texture 통해 정보 전달
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());

	// Bones
	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		// Model에 저장된 bone
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
	_shader->PushBoneData(boneDesc);


	// // Transform -> Instancing Buffer에서 진행됨
	// // GameObject의 World 좌표 가져오기
	// auto world = GetTransform()->GetWorldMatrix();
	// // ConstantBuffer에 해당 위치로 Model 이동
	// RENDER->PushTransformData(TransformDesc{ world });

	// Mesh 마다 갱신
	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		// Mesh에 적용된 Material 갱신
		if (mesh->material)
			mesh->material->Update();

		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		// position, uv, normal, tangent, blendIndices, blendWeights
		// 정점데이터를 slot0으로 전달
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		buffer->PushData();

		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}

InstanceID ModelAnimator::GetInstanceID()
{
	return make_pair((uint64)_shader.get(), (uint64)_model.get());
}

void ModelAnimator::CreateTexture()
{
	// 3D model Animation이 없음
	if (_model->GetAnimationCount() == 0)
		return;

	// 3D이 가지고 있는 Animation 개수만큼 resize
	_animTransforms.resize(_model->GetAnimationCount());

	for (uint32 i = 0; i < _model->GetAnimationCount(); i++)
		// Animation의 Keyframe단위로 Bone의 SRT행렬을 구함
		CreateAnimationTransform(i);

	// Animation Transfrom 행렬을 가지고 Texture 생성
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		
		// Matrix는 4(float) * 4(행) * 4(열) = 64바이트이고 
		// 한칸에 들어가는 데이터는 Format으로 정해진 16바이트 
		// 4칸에 하나의 Matrix를 나눠서 넣어야함.
		// 그렇기 때문에 Width에 * 4를함.
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_TRANSFORMS;
		desc.ArraySize = _model->GetAnimationCount(); // shader의 Texture2D 배열에 사용
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 32bit * 4 = 128 = 16바이트
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;



		// Matrix(64바이트) * bone의 최대 개수 (frame 1개의 메모리 크기)
		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		// dataSize * keyframe 최대 개수 (Animation 1개의 메모리 크기)
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		// pageSize * Animation 개수의 크기만큼 메모리할당
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());



		// 데이터 조립
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			// c번째 Animation의 시작 주소 offset
			uint32 startOffset = c * pageSize;
			// reinterpret_cast<>() : 데이터의 실제 값 또는 내용물은 바뀌지 않고, 타입 해석만 강제로 바꿈.
			// 원하는 byte만큼의 주소를 이동하고 싶으면 char(1byte)형으로 변경하고 + offset를 하면
			// offset 바이트만큼 이동함
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				// pageStartPtr번 Animation의 f번째 frame의 메모리 주소
				void* ptr = pageStartPtr + dataSize * f;
				// ptr 메모리에 Keyframe의 SRT행렬을 저장함
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}


		// 리소스 만들기
		// Animation 개수만큼 SUBREOURCES 생성
		vector<D3D11_SUBRESOURCE_DATA> subResources(_model->GetAnimationCount());

		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			// c번째 Animation 메모리 시작 주소
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResources.data(), _texture.GetAddressOf());
		CHECK(hr);

		// 메모리 해제
		::free(mallocPtr);
	}

	// SRV 생성
	// _texture의 데이터를 SRV에 저장
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = _model->GetAnimationCount();

		HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &desc, &_srv);
		CHECK(hr);
	}
}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	// Bone의 Transform을 임시 cache
	vector<Matrix> tempAnimBoneTransform(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	// 3D모델에 저장한 Animation을 순차적으로 꺼내기
	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	// 가져온 animation의 keyframe개수만큼 반복
	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		// 3D 모델에 저장된 Bone의 개수만큼 반복
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);

			// Bone의 부모좌표계에서 SRT 좌표
			Matrix matAnimation;

			// keyframe의 key를 bone의 이름으로 해둠.
			// Animation에 들어가 있는 bone과 연관된 keyframe가져오기
			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
			if (frame != nullptr)
			{
				// b번 bone이 가지고 있는 k번째 keyframe에서의 SRT정보
				ModelKeyframeData& data = frame->transforms[f];

				// SRT 정보를 행렬로 만듦
				Matrix S, R, T;
				S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				// bone의 SRT 행렬
				matAnimation = S * R * T;
			}
			// keyframe에 데이터가 없음
			else
			{
				matAnimation = Matrix::Identity;
			}


			// Animation을 진행 과정
			// 1. Mesh의 정점데이터는 기본 포즈인 T(A)포즈 기준으로 되어 있음.
			// 2. Animation 데이터는 bone의 부모좌표계 기준으로 되어있음
			// 3. 정점데이터에 Animation 데이터를 적용함
			//  - 정점 데이터를 bone의 부모 좌표계로 변환함
			//  - 부모 좌표계로 전환한 데이터에 애니메이션 데이터를 적용함
			// 4. Animation 데이터를 적용한 정점데이터를 다시 Local 좌표계로 변환함


			// T 포즈 상태관련
			// ! bone의 SRT행렬을 Wordl 좌표계로 변환하기
			// bone의 부모 좌표계에서 Mesh의 Local 좌표계 변환 행렬
			Matrix toRootMatrix = bone->transform;
			// Local좌표계에서 bone의 원래 좌표계(bone의 부모 좌표계)로 변환 행렬
			Matrix invGlobal = toRootMatrix.Invert();


			// Animation 관련
			// 부모 bone이 있다면 Local 변환 행렬을 가져와서 
			int32 parentIndex = bone->parentIndex;
			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransform[parentIndex];
			// f프레임에서의 해당 bone의 Local 좌표계의 SRT행렬을 저장
			tempAnimBoneTransform[b] = matAnimation * matParent;

			// index번의 Animation의 f번째 frame의 b번째 bone의 SRT를 부모좌표계로 변환해서 저장
			_animTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransform[b];
		}
	}
}
