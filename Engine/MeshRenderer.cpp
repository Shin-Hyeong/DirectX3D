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
//	// ���� ��ǥ
//	auto world = GetTransform()->GetWorldMatrix();
//	// World ��ǥ�� ConstantBuffer�� ���� �� DeviceContext�� ����
//	RENDER->PushTransformData(TransformDesc{world});
//	// View, Projection�� ��ü���� ���� ����� �ƴ� Global������ �������� ����ϴ� ���� ����
//	// - RenderManager�� Update()���� ����ϵ��� ����
//	
//	
//	// Light (�ӽ�)
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
//	// ���� ��ǥ
//	auto world = GetTransform()->GetWorldMatrix();
//	// World ��ǥ�� ConstantBuffer�� ���� �� DeviceContext�� ����
//	RENDER->PushTransformData(TransformDesc{ world });
//	// View, Projection�� ��ü���� ���� ����� �ƴ� Global������ �������� ����ϴ� ���� ����
//	// - RenderManager�� Update()���� ����ϵ��� ����
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

	// MaterialDesc�� Shader�� �����ϰ�
	// DiffuseMap, NormalMap, SpecularMap�� �ִٸ� SRV�� ���� ������
	_material->Update();

	// ���� ��ǥ
	auto world = GetTransform()->GetWorldMatrix();
	// World ��ǥ�� ConstantBuffer�� ���� �� DeviceContext�� ����
	RENDER->PushTransformData(TransformDesc{ world });
	// View, Projection�� ��ü���� ���� ����� �ƴ� Global������ �������� ����ϴ� ���� ����
	// - RenderManager�� Update()���� ����ϵ��� ����
	
	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
	
	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	
	shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}
