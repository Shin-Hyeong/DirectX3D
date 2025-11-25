#pragma once
#include "Component.h"

// Animation 전용 Renderer

class Model;

// Animation의 Bone들의 SRT 변경에 대한 정보
struct AnimTransform
{
	// Bone의 SRT 행렬
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;

	// Bone의 Keyframe마다 SRT행렬
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

class ModelAnimator : public Component
{
	using Super = Component;
public:
	ModelAnimator(shared_ptr<Shader> shader);
	~ModelAnimator();

	virtual void Update() override;

	void SetModel(shared_ptr<Model> model);
	void SetPass(uint8 pass) { _pass = pass; }

private:
	// _animTransforms에 저장된 Matrix를 Texture으로 전환
	void CreateTexture();
	// Animation의 frame마다 bone의 SRT행렬을 구함.
	// @params index : _model에 저장된 Animation index(순서)
	void CreateAnimationTransform(uint32 index);

private:
	// Animation 개수 만큼 필요함
	// Animation의 keyframe마다의 bone의 SRT행렬을 저장함
	vector<AnimTransform>				_animTransforms;
	// 
	ComPtr<ID3D11Texture2D>				_texture;
	ComPtr<ID3D11ShaderResourceView>	_srv;


	// Keyframe에 대한 정보
	KeyframeDesc						_keyframeDesc;
	// Tween에 대한 정보
	TweenDesc							_tweenDesc;

private:
	shared_ptr<Shader>	_shader;
	uint8				_pass = 0;
	// Rendering할 Model
	shared_ptr<Model>	_model;
};

