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
};

