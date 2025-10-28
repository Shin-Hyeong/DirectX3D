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
};

