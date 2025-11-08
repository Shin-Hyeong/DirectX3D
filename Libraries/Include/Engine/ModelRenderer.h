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

	virtual void Update() override;

	// Rendering할 Model 호출
	// @param model Rendering할 model
	void SetModel(shared_ptr<Model> model);

	// .fx에서 사용할 pass 지정
	void SetPass(uint8 pass) { _pass = pass; }

private:
	shared_ptr<Shader>		_shader;
	uint8					_pass = 0;	// .fx에서 사용할 pass 번호
	shared_ptr<Model>		_model;
};

