#pragma once
#include "ResourceBase.h"


// 물체의 Material에 사용되는 SRV를 Shader에 넘길 수 있음.

class Material : public ResourceBase
{
	using Super = ResourceBase;
public:
	Material();
	virtual ~Material();

	void Update();

	shared_ptr<Shader> GetShader() { return _shader; }

	MaterialDesc& GetMaterialDesc() { return _desc; }
	shared_ptr<Texture> GetDiffuseMap() { return _diffuseMap; }
	shared_ptr<Texture> GetNormalMap() { return _normalMap; }
	shared_ptr<Texture> GetSpecularMap() { return _specularMap; }

	void SetShader(shared_ptr<Shader> shader);
	void SetDiffuseMap(shared_ptr<Texture> diffuseMap) { _diffuseMap = diffuseMap; }
	void SetNormalMap(shared_ptr<Texture> normalMap) { _normalMap = normalMap; }
	void SetSpecularMap(shared_ptr<Texture> specularMap) { _specularMap = specularMap; }

	// Material를 내용을 그대로 사용하지만 복사해서 사용함
	shared_ptr<Material> Clone();

private:
	friend class MeshRenderer;

	MaterialDesc			_desc;

	// Resource
	shared_ptr<Shader>		_shader;
	shared_ptr<Texture>		_diffuseMap;
	shared_ptr<Texture>		_normalMap;
	shared_ptr<Texture>		_specularMap;

	// Shader와 연결하여 Resource를 전달함
	ComPtr<ID3DX11EffectShaderResourceVariable>		_diffuseEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable>		_normalEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable>		_specularEffectBuffer;
};

