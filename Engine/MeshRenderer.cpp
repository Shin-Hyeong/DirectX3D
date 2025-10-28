#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	// Texture
//	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
//
//	// 월드 좌표
//	auto world = GetTransform()->GetWorldMatrix();
//	// World 좌표를 ConstantBuffer에 삽입 후 DeviceContext에 전달
//	RENDER->PushTransformData(TransformDesc{world});
//	// View, Projection은 물체마다 개별 계산이 아닌 Global적으로 통합으로 계산하는 것이 좋음
//	// - RenderManager의 Update()에서 계산하도록 설정
//	
//	
//	// Light (임시)
//	Vec3 lightDir = {0.f, 0.f, 1.f};
//	_shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	// Texture
//	_shader->GetSRV("DiffuseMap")->SetResource(_texture->GetComPtr().Get());
//
//	// 월드 좌표
//	auto world = GetTransform()->GetWorldMatrix();
//	// World 좌표를 ConstantBuffer에 삽입 후 DeviceContext에 전달
//	RENDER->PushTransformData(TransformDesc{ world });
//	// View, Projection은 물체마다 개별 계산이 아닌 Global적으로 통합으로 계산하는 것이 좋음
//	// - RenderManager의 Update()에서 계산하도록 설정
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

void MeshRenderer::Update()
{
	if (_mesh == nullptr || _material == nullptr) return;

	auto shader = _material->GetShader();
	if (shader == nullptr) return;

	// MaterialDesc를 Shader에 전달하고
	// DiffuseMap, NormalMap, SpecularMap이 있다면 SRV를 통해 전달함
	_material->Update();

	// 월드 좌표
	auto world = GetTransform()->GetWorldMatrix();
	// World 좌표를 ConstantBuffer에 삽입 후 DeviceContext에 전달
	RENDER->PushTransformData(TransformDesc{ world });
	// View, Projection은 물체마다 개별 계산이 아닌 Global적으로 통합으로 계산하는 것이 좋음
	// - RenderManager의 Update()에서 계산하도록 설정
	
	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
	
	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	
	shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}
