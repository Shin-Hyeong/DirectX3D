#include "pch.h"
#include "SnowBillboard.h"
#include "Material.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MathUtils.h"

SnowBillboard::SnowBillboard(Vec3 extent, int32 drawCount)
	: Super(ComponentType::SnowBillboard)
{
	_desc.extent = extent;
	_desc.drawDistance = _desc.extent.z * 2.f;
	_drawCount = drawCount;
	
	const int32 vertexCount = _drawCount * 4;
	_vertices.resize(vertexCount);

	// 임의의 위치에 Quad위치 지정하기
	for (int32 i = 0; i < _drawCount * 4; i += 4)
	{
		Vec2 scale = MathUtils::RandomVec2(0.1f, 0.5f);

		// 랜덤 position
		Vec3 position;
		position.x = MathUtils::Random(-_desc.extent.x, _desc.extent.x);
		position.y = MathUtils::Random(-_desc.extent.y, _desc.extent.y);
		position.z = MathUtils::Random(-_desc.extent.z, _desc.extent.z);

		Vec2 random = MathUtils::RandomVec2(0.0f, 1.0f);

		// 4개의 정점을 동일한 위치에 지정하고 Shader에서 재배치함
		_vertices[i + 0].position = position;
		_vertices[i + 1].position = position;
		_vertices[i + 2].position = position;
		_vertices[i + 3].position = position;

		// uv
		_vertices[i + 0].uv = Vec2(0, 1);
		_vertices[i + 1].uv = Vec2(0, 0);
		_vertices[i + 2].uv = Vec2(1, 1);
		_vertices[i + 3].uv = Vec2(1, 0);

		// scale
		_vertices[i + 0].scale = scale;
		_vertices[i + 1].scale = scale;
		_vertices[i + 2].scale = scale;
		_vertices[i + 3].scale = scale;

		// random
		_vertices[i + 0].random = random;
		_vertices[i + 1].random = random;
		_vertices[i + 2].random = random;
		_vertices[i + 3].random = random;
	}

	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_vertices, 0);

	const int32 indexCount = _drawCount * 6;
	_indices.resize(indexCount);

	for (int32 i = 0; i < _drawCount; i++)
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

SnowBillboard::~SnowBillboard()
{

}

void SnowBillboard::Update()
{
	// 카메라를 원점으로 사용하다록 함
	_desc.origin = CUR_SCENE->GetMainCamera()->GetTransform()->GetWorldPosition();
	// DeltaTime을 Snow 경과시간에 넣음
	_desc.time = _elpasedTime;
	_elpasedTime += DT;

	auto shader = _material->GetShader();

	// Transform
	auto world = GetTransform()->GetWorldMatrix(); // 월드 변환 행렬
	shader->PushTransformData(TransformDesc{ world });

	// Global
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Constant Buffer를 Shader에 전달
	shader->PushSnowData(_desc);

	// Light
	_material->Update();

	// IA, RenderingPipeline에 연결
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	// 깩체를 늘려 Instancing하는 것이 아닌 BillBoard Component을 가진 Mesh의 정점을 늘린 형태임
	shader->DrawIndexed(0, _pass, _drawCount * 6);
}

