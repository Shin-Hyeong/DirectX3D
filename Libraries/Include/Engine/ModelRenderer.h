#pragma once
#include "Component.h"

class Model;
class Shader;
class Material;

class ModelRenderer : public Component
{
	using Super = Component;
public:
	ModelRenderer(shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	// virtual void Update() override;

	// Rendering할 Model 호출
	// @param model Rendering할 model
	void SetModel(shared_ptr<Model> model);

	// .fx에서 사용할 pass 지정
	void SetPass(uint8 pass) { _pass = pass; }

	// 
	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	// Objects가 사용하는 shader와 model 주소를 Map으로 묶어서 반환함
	InstanceID GetInstanceID();

private:
	shared_ptr<Shader>		_shader;
	uint8					_pass = 0;	// .fx에서 사용할 pass 번호
	shared_ptr<Model>		_model;
};

