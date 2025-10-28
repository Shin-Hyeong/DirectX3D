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

	// Light���� Desc�� �����ϴ� ConstantBuffer ����
	_lightBuffer = make_shared<ConstantBuffer<LightDesc>>();
	_lightBuffer->Create();
	_lightEffectBuffer = _shader->GetConstantBuffer("LightBuffer");

	// Material���� Desc�� ������ ConstantBuffer ����
	_materialBuffer = make_shared<ConstantBuffer<MaterialDesc>>();
	_materialBuffer->Create();
	_materialEffectBuffer = _shader->GetConstantBuffer("MaterialBuffer");
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
	_globalDesc.VInv = view.Invert();

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

void RenderManager::PushLightData(const LightDesc& desc)
{
	_lightDesc = desc;
	_lightBuffer->CopyData(_lightDesc);
	_lightEffectBuffer->SetConstantBuffer(_lightBuffer->GetComPtr().Get());
}

void RenderManager::PushMaterialData(const MaterialDesc& desc)
{
	_materialDesc = desc;
	_materialBuffer->CopyData(_materialDesc);
	_materialEffectBuffer->SetConstantBuffer(_materialBuffer->GetComPtr().Get());
}
