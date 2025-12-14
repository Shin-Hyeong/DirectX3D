#include "pch.h"
#include "Terrain.h"
#include "MeshRenderer.h"
#include "Camera.h"

Terrain::Terrain()
	: Super(ComponentType::Terrain)
{

}

Terrain::~Terrain()
{
}

void Terrain::Create(int32 sizeX, int32 sizeZ, shared_ptr<Material> material)
{
	_sizeX = sizeX;
	_sizeZ = sizeZ;

	auto object = _gameObject.lock();
	object->GetOrAddTransform();

	if (object->GetMeshRenderer() == nullptr)
		object->AddComponent(make_shared<MeshRenderer>());

	_mesh = make_shared<Mesh>();
	_mesh->CreateGrid(sizeX, sizeZ);

	object->GetMeshRenderer()->SetMesh(_mesh);
	object->GetMeshRenderer()->SetPass(0);
	object->GetMeshRenderer()->SetMaterial(material);
}

bool Terrain::Pick(int32 screenX, int32 screenY, Vec3& pickPos, float& distance)
{
	// GameObject가 World 좌표계로 변환하는 행렬
	Matrix W = GetTransform()->GetWorldMatrix();
	// World -> View변환 행렬
	Matrix V = Camera::S_MatView;
	// View -> Projection 변환 행렬
	Matrix P = Camera::S_MatProjection;

	// Viewport(Project(3D->2D), Unproject(2D->3D))
	Viewport& vp =GRAPHICS->GetViewport();

	// Depth 0
	Vec3 n = vp.Unproject(Vec3(screenX, screenY, 0), W, V, P);
	// Depth 1
	Vec3 f = vp.Unproject(Vec3(screenX, screenY, 1), W, V, P);

	// Ray의 시작 위치
	Vec3 start = n;
	// Ray의 방향
	Vec3 direction = f - n;
	direction.Normalize();

	Ray ray = Ray(start, direction);

	// Terrain(Grid)의 정점 정보
	const auto& vertices =  _mesh->GetGeometry()->GetVertices();

	for (int32 z = 0; z < _sizeZ; z++)
	{
		for (int32 x = 0; x < _sizeX; x++)
		{
			// Grid에서 Vertex의 Index
			// [2][3]
			// [0][1]
			uint32 index[4];
			index[0] = (_sizeX + 1) * z + x;
			index[1] = (_sizeX + 1) * z + (x + 1);
			index[2] = (_sizeX + 1) * (z + 1) + x;
			index[3] = (_sizeX + 1) * (z + 1) + (x + 1);

			// Vertex의 Position
			Vec3 p[4];
			for (int32 i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			// [2]
			//  |  \
			// [0] ㅡ [1]
			// 삼각형에 대해서 충돌 판정 검사
			if (ray.Intersects(p[0], p[1], p[2], OUT distance))
			{
				// Terrain(Grid)가 감지된 위치
				pickPos = ray.position + ray.direction * distance;
				return true;
			}

			// [2] ㅡ [3]
			//      \  |
			//        [1]
			// 삼각형에 대해서 충돌 판정 검사
			if (ray.Intersects(p[2], p[3], p[1], OUT distance))
			{
				// Terrain(Grid)가 감지된 위치
				pickPos = ray.position + ray.direction * distance;
				return true;
			}
		}
	}

	return false;
}
