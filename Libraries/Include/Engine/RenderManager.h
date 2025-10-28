// MeshRenderer���� Mesh�� ���� Shader�� �����ϴ� ���� �޸��Ͽ�
// �̸� ������
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

	// ���
	GlobalDesc									_globalDesc;
	// V, P, VP�� �����ϴ� ConstantBuffer
	shared_ptr<ConstantBuffer<GlobalDesc>>		_globalBuffer;
	// Shader���� �ش� Buffer�� ����ϵ��� ��. 
	// shader->GetConstantBuffer()�� ���� ���� ���� Cashing �� �뵵
	ComPtr<ID3DX11EffectConstantBuffer>			_globalEffectBuffer;

	// Transform
	TransformDesc								_transformDesc;
	// W�� �����ϴ� ConstantBuffer
	shared_ptr<ConstantBuffer<TransformDesc>>	_transformBuffer;
	// Shader���� �ش� Buffer�� ����ϵ��� ��. 
	// shader->GetConstantBuffer()�� ���� ���� ���� Cashing �� �뵵
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

