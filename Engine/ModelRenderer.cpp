#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h"
#include "InstancingBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Light.h"
#include "Camera.h"

ModelRenderer::ModelRenderer(shared_ptr<Shader> shader) 
	: Super(ComponentType::ModelRenderer), _shader(shader)
{

}

ModelRenderer::~ModelRenderer()
{

}

// Bone이 없는 단일 Model
//void ModelRenderer::Update()
//{
//	// Model이 없으면
//	if (_model == nullptr)
//		return;
//	
//	// GameObject의 World 좌표 가져오기
//	auto world = GetTransform()->GetWorldMatrix();
//	// ConstantBuffer에 해당 위치로 Model 이동
//	RENDER->PushTransformData(TransformDesc{ world });
//	
//	// Mesh 마다 갱신
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		// Mesh에 적용된 Material 갱신
//		if (mesh->material)
//			mesh->material->Update();
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}


// Bone을 가지고 있는 Model
// void ModelRenderer::Update()
// {
// 	// Model이 없으면
// 	if (_model == nullptr)
// 		return;
// 
// 
// 	// Bones
// 	BoneDesc boneDesc;
// 
// 	const uint32 boneCount = _model->GetBoneCount();
// 	for (uint32 i = 0; i < boneCount; i++)
// 	{
// 		// Model에 저장된 bone
// 		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
// 		boneDesc.transforms[i] = bone->transform;
// 	}
// 	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
// 	RENDER->PushBoneData(boneDesc);
// 
// 
// 	// Transform
// 	// GameObject의 World 좌표 가져오기
// 	auto world = GetTransform()->GetWorldMatrix();
// 	// ConstantBuffer에 해당 위치로 Model 이동
// 	RENDER->PushTransformData(TransformDesc{ world });
// 	
// 	// Mesh 마다 갱신
// 	const auto& meshes = _model->GetMeshes();
// 	for (auto& mesh : meshes)
// 	{
// 		// Mesh에 적용된 Material 갱신
// 		if (mesh->material)
// 			mesh->material->Update();
// 
// 		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
// 		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
// 
// 		uint32 stride = mesh->vertexBuffer->GetStride();
// 		uint32 offset = mesh->vertexBuffer->GetOffset();
// 
// 		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
// 		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
// 
// 		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
// 	}
// }

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	// Model이 없으면
	if (_model == nullptr)
		return;

	{
		// GlobalData
		_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		// Light
		auto lightObj = SCENE->GetCurrentScene()->GetLight();
		if (lightObj)
			_shader->PushLightData(lightObj->GetLight()->GetLightDesc());
	}

	// Bones의 계층구조
	// - 계층 구조를 동일하게하면 모든 Static Mesh가 동일한 모습을 가지게 됨.
	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		// Model에 저장된 bone
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
	_shader->PushBoneData(boneDesc);


	// // Transform -> InstancingBuffer에서 진행함
	// // GameObject의 World변환 행렬 가져오기
	// auto world = GetTransform()->GetWorldMatrix();
	// // ConstantBuffer에 해당 위치로 Model 이동
	// RENDER->PushTransformData(TransformDesc{ world });


	// 3D Model의 모듈화된 부품끼리 Instancing하여 DrawCall를 함
	// - 한번에 바퀴 그린 후, 차체, 창문, 등 부품별로 Instancing
	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		// Mesh에 적용된 Material 갱신
		if (mesh->material)
			mesh->material->Update();

		// 해당 Mesh가 몇번 Bone과 연결되어 있는지 shader의 BoneIndex 변수에 전달
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);


		// IA
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		// Instancing된 Model들의 World 변환 행렬 전달
		buffer->PushData();

		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}

InstanceID ModelRenderer::GetInstanceID()
{
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}
