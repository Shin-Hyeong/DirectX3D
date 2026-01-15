#include "pch.h"
#include "BillBoard.h"
#include "Material.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

BillBoard::BillBoard()
	: Super(ComponentType::BillBoard)
{
	// Quad(정점이 4개)이기 때문에 * 4를 함
	uint32 vertexCount = MAX_BILLBOARD_COUNT * 4;
	// Quad(정점이 4개)이기 때문에 * 6를 함
	// 한 Quad에 {1, 2, 3}, {2, 3, 1}
	uint32 indexCount = MAX_BILLBOARD_COUNT * 6;

	_vertices.resize(vertexCount);
	// vertexBuffer 생성
	_vertexBuffer = make_shared<VertexBuffer>();
	// cpuWrite를 True로 하여 나중에 갱신할 수 있도록 함
	_vertexBuffer->Create(_vertices, 0, true);

	_indices.resize(indexCount);

	// i번째 Object마다 Index 정리
	for (int32 i = 0; i < MAX_BILLBOARD_COUNT; i++)
	{
		// {0, 1, 2}
		_indices[i * 6 + 0] = i * 4 + 0; 
		_indices[i * 6 + 1] = i * 4 + 1;
		_indices[i * 6 + 2] = i * 4 + 2;
		// {2, 1, 3}
		_indices[i * 6 + 3] = i * 4 + 2;
		_indices[i * 6 + 4] = i * 4 + 1;
		_indices[i * 6 + 5] = i * 4 + 3;
	}

	
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_indices);
}

BillBoard::~BillBoard()
{

}

void BillBoard::Update()
{
	// 이전에 그린 개수(_prevCount)와 Update된 개수(_drawCount)가 다르면 갱신함
	// - 정보가 바뀌었을 때만 진행함
	if (_drawCount != _prevCount)
	{
		_prevCount = _drawCount;

		// Vertex 정보를 vertexBuffer에 전달함
		D3D11_MAPPED_SUBRESOURCE subResource;
		DC->Map(_vertexBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			::memcpy(subResource.pData, _vertices.data(), sizeof(VertexBillBoard) * _vertices.size());
		}
		DC->Unmap(_vertexBuffer->GetComPtr().Get(), 0);
	}

	auto shader = _material->GetShader();

	// Transform
	auto world = GetTransform()->GetWorldMatrix(); // 월드 변환 행렬
	shader->PushTransformData(TransformDesc{ world });

	// Global
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	_material->Update();

	// IA, RenderingPipeline에 연결
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	// 깩체를 늘려 Instancing하는 것이 아닌 BillBoard Component을 가진 Mesh의 정점을 늘린 형태임
	shader->DrawIndexed(0, _pass, _drawCount * 6);
}

void BillBoard::Add(Vec3 position, Vec2 scale)
{
	// 4개의 정점을 한곳에 모아둠
	// - shader에서 위치 배정을 시키도록 함
	// - Geometry Shader 단계에서 정점을 늘릴수도 있음
	_vertices[_drawCount * 4 + 0].position = position;
	_vertices[_drawCount * 4 + 1].position = position;
	_vertices[_drawCount * 4 + 2].position = position;
	_vertices[_drawCount * 4 + 3].position = position;

	_vertices[_drawCount * 4 + 0].uv = Vec2(0, 1);
	_vertices[_drawCount * 4 + 1].uv = Vec2(0, 0);
	_vertices[_drawCount * 4 + 2].uv = Vec2(1, 1);
	_vertices[_drawCount * 4 + 3].uv = Vec2(1, 0);

	// Shader에서 보정 작업을 진행함
	_vertices[_drawCount * 4 + 0].scale = scale;
	_vertices[_drawCount * 4 + 1].scale = scale;
	_vertices[_drawCount * 4 + 2].scale = scale;
	_vertices[_drawCount * 4 + 3].scale = scale;

	_drawCount++;
}