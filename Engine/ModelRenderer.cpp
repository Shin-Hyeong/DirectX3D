#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h"

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
void ModelRenderer::Update()
{
	// Model이 없으면
	if (_model == nullptr)
		return;


	// Bones
	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		// Model에 저장된 bone
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	// Model에 저장된 Bone의 로컬 변환 행렬을 모아서 CBuffer에 전달
	RENDER->PushBoneData(boneDesc);


	// Transform
	// GameObject의 World 좌표 가져오기
	auto world = GetTransform()->GetWorldMatrix();
	// ConstantBuffer에 해당 위치로 Model 이동
	RENDER->PushTransformData(TransformDesc{ world });
	
	// Mesh 마다 갱신
	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		// Mesh에 적용된 Material 갱신
		if (mesh->material)
			mesh->material->Update();

		// 해당 Mesh가 몇번 Bone과 연결되어 있는지
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();

		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
	}
}

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}
