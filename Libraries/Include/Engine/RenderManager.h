// MeshRenderer에서 Mesh에 따라 Shader에 전달하는 값을 달리하여
// 이를 관리함
#pragma once
#include "ConstantBuffer.h"

class Shader;

struct GlobalDesc
{
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	Matrix VP = Matrix::Identity;
	Matrix VInv = Matrix::Identity;
};

struct TransformDesc
{
	Matrix World = Matrix::Identity;
};

// Light
struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(1.f, 1.f, 1.f, 1.f);

	Vec3 direction;
	float padding0;
};

// Material
struct MaterialDesc
{
	Color ambient = Color(0.f, 0.f, 0.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(0.f, 0.f, 0.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
};

// Bone
// Mesh의 Bone 허용 개수
#define MAX_MODEL_TRANSFORMS 250
// Animation의 Keyframe 허용 개수
#define MAX_MODEL_KEYFRAMES 500

struct BoneDesc
{
	// Bone의 Transform 저장 배열
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

// Animation
// 현재 진행중인 Animation에 대한 정보
struct KeyframeDesc
{
	int32 animIndex = 0;		// 현재 재생중인 Animation 인덱스
	uint32 currFrame = 0;		// 현재 재생중인 Frame
	uint32 nextFrame = 0;		// 다음 재생할 Frame
	float ratio = 0.f;			// frame blending 비율

	float sumTime = 0.f;		// 현재 Animation 재생된 시간
	float speed = 1.f;			// 재생 속도
	Vec2 padding;				// 16바이트에 맞추기 위한 Padding
};

// Tween
// 다음 Animation으로 넘어갈때 자연스럽게 하기 위한 정보
struct TweenDesc
{
	// 초기 값
	TweenDesc()
	{
		curr.animIndex = 0;
		next.animIndex = -1; // 없을 수 도 있음
	}

	// 다음 Animation을 넘어가면 Tween 설정값 초기화
	void ClearNextAnim()
	{
		next.animIndex = -1;
		next.currFrame = 0;
		next.nextFrame = 0;
		next.sumTime = 0;
		tweenSumTime = 0;
		tweenRatio = 0;
	}

	float tweenDuration = 1.f;		// Tween 하는 시간
	float tweenRatio = 0.f;			// Tween 비율
	float tweenSumTime = 0.f;		// Tween 경과 시간
	float padding = 0.f;			// 16byte padding

	KeyframeDesc curr;		// 현재 Animation의 frame
	// 다음 Animation을 재생하지 않으면 값을 넣지 않을 수 있음
	KeyframeDesc next;		// 다음 Animation의 frame
};

class RenderManager
{
	DECLARE_SINGLE(RenderManager)

public:
	void Init(shared_ptr<Shader> shader);
	void Update();

	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);
	void PushBoneData(const BoneDesc& desc);
	void PushKeyframeData(const KeyframeDesc& desc);
	void PushTweenData(const TweenDesc& desc);

private:
	shared_ptr<Shader>							_shader;

	// 행렬
	GlobalDesc									_globalDesc;
	// V, P, VP를 전달하는 ConstantBuffer
	shared_ptr<ConstantBuffer<GlobalDesc>>		_globalBuffer;
	// Shader에게 해당 Buffer를 사용하도록 함. 
	// shader->GetConstantBuffer()를 통해 얻은 것을 Cashing 할 용도
	ComPtr<ID3DX11EffectConstantBuffer>			_globalEffectBuffer;

	// Transform
	TransformDesc								_transformDesc;
	// W를 전달하는 ConstantBuffer
	shared_ptr<ConstantBuffer<TransformDesc>>	_transformBuffer;
	// Shader에게 해당 Buffer를 사용하도록 함. 
	// shader->GetConstantBuffer()를 통해 얻은 것을 Cashing 할 용도
	ComPtr<ID3DX11EffectConstantBuffer>			_transformEffectBuffer;

	//  Light
	LightDesc									_lightDesc;
	shared_ptr<ConstantBuffer<LightDesc>>		_lightBuffer;
	ComPtr<ID3DX11EffectConstantBuffer>			_lightEffectBuffer;

	// Material
	MaterialDesc								_materialDesc;
	shared_ptr<ConstantBuffer<MaterialDesc>>	_materialBuffer;
	ComPtr<ID3DX11EffectConstantBuffer>			_materialEffectBuffer;

	// Bone
	BoneDesc									_boneDesc;
	shared_ptr<ConstantBuffer<BoneDesc>>		_boneBuffer;
	ComPtr<ID3DX11EffectConstantBuffer>			_boneEffectBuffer;

	// Animation / Keyframe
	KeyframeDesc								_keyframeDesc;
	shared_ptr<ConstantBuffer<KeyframeDesc>>	_keyframeBuffer;
	ComPtr<ID3DX11EffectConstantBuffer>			_keyframeEffectBuffer;

	// Tween
	TweenDesc									_tweenDesc;
	shared_ptr<ConstantBuffer<TweenDesc>>		_tweenBuffer;
	ComPtr<ID3DX11EffectConstantBuffer>			_tweenEffectBuffer;
};

