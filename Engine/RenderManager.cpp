#include "pch.h"
#include "RenderManager.h"
#include "Camera.h"

void RenderManager::Init(shared_ptr<Shader> shader)
{
	_shader = shader;

	// View, Projection�� �����ϴ� ConstantBuffer ����
	_globalBuffer = make_shared<ConstantBuffer<GlobalDesc>>();
	_globalBuffer->Create();
	// Shader���� GlobalBuffer �����ͼ� Cashing
	_globalEffectBuffer = _shader->GetConstantBuffer("GlobalBuffer");

	// World�� �����ϴ� ConstantBuffer ����
	_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
	_transformBuffer->Create();
	// Shader���� TransformBuffer ����
	_transformEffectBuffer = _shader->GetConstantBuffer("TransformBuffer");
}

void RenderManager::Update()
{
	// �����Ӹ��� ī�޶��� View, Projection ����� ������
	// ConstantBuffer�� ������ ������ DeviceContext�� ������
	PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
}

void RenderManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	_globalDesc.View = view;
	_globalDesc.Projection = projection;
	_globalDesc.VP = view * projection;
	// ConstantBuffer�� ������ ����
	_globalBuffer->CopyData(_globalDesc);
	// DeviceContext�� ������ ����
	_globalEffectBuffer->SetConstantBuffer(_globalBuffer->GetComPtr().Get());
}

void RenderManager::PushTransformData(const TransformDesc& desc)
{
	_transformDesc = desc;
	// ConstantBuffer�� ������ ����
	_transformBuffer->CopyData(_transformDesc);
	// DeviceContext�� ������ ����
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}
