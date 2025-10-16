#include "pch.h"
#include "RenderManager.h"
#include "Camera.h"

void RenderManager::Init(shared_ptr<Shader> shader)
{
	_shader = shader;

	// View, Projection을 전달하는 ConstantBuffer 생성
	_globalBuffer = make_shared<ConstantBuffer<GlobalDesc>>();
	_globalBuffer->Create();
	// Shader에서 GlobalBuffer 가져와서 Cashing
	_globalEffectBuffer = _shader->GetConstantBuffer("GlobalBuffer");

	// World을 전달하는 ConstantBuffer 생성
	_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
	_transformBuffer->Create();
	// Shader에서 TransformBuffer 생성
	_transformEffectBuffer = _shader->GetConstantBuffer("TransformBuffer");
}

void RenderManager::Update()
{
	// 프레임마다 카메라의 View, Projection 행렬을 갱신함
	// ConstantBuffer에 데이터 삽입후 DeviceContext에 전달함
	PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
}

void RenderManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	_globalDesc.View = view;
	_globalDesc.Projection = projection;
	_globalDesc.VP = view * projection;
	// ConstantBuffer에 데이터 삽입
	_globalBuffer->CopyData(_globalDesc);
	// DeviceContext에 데이터 전달
	_globalEffectBuffer->SetConstantBuffer(_globalBuffer->GetComPtr().Get());
}

void RenderManager::PushTransformData(const TransformDesc& desc)
{
	_transformDesc = desc;
	// ConstantBuffer에 데이터 삽입
	_transformBuffer->CopyData(_transformDesc);
	// DeviceContext에 데이터 전달
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}
