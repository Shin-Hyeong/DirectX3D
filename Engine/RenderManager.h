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
};

struct TransformDesc
{
	Matrix World = Matrix::Identity;
};

class RenderManager
{
	DECLARE_SINGLE(RenderManager)

public:
	void Init(shared_ptr<Shader> shader);
	void Update();

	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);

private:
	shared_ptr<Shader>							_shader;

	// Global
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
};

