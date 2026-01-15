#pragma once

struct VertexBillBoard
{
	Vec3 position;
	Vec2 uv;
	Vec2 scale;
};

#define MAX_BILLBOARD_COUNT 500

class BillBoard : public Component
{
	using Super = Component;
public:
	BillBoard();
	virtual ~BillBoard();

	void Update();

	// BillBoard할 위치 추가
	// - 추가한 위치에 해당 Component를 가진 GameObject와 동일한 Mesh를 추가함
	// @params position : 추가할 위치
	// @params size : 추가할 위치에 있을 Mesh의 Size
	void Add(Vec3 position, Vec2 scale);

	void SetMaterial(shared_ptr<class Material> material) { _material = material; }
	void SetPass(uint8 pass) { _pass = pass; }

private:
	vector<VertexBillBoard>		_vertices;
	vector<uint32>				_indices;
	shared_ptr<VertexBuffer>	_vertexBuffer;
	shared_ptr<IndexBuffer>		_indexBuffer;

	// 그려야하는 물체 개수
	int32 _drawCount = 0;
	// 이전에 그린 개수
	int32 _prevCount = 0;

	shared_ptr<Material> _material;
	uint8 _pass = 0;
};

