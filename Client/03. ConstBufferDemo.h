#pragma once
#include "IExecute.h"
#include "Geometry.h"

class ConstBufferDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// 쉐이더
	shared_ptr<Shader>						_shader;
	
	// 정점
	shared_ptr<Geometry<VertexColorData>>	_geometry;

	// GPU에 전달하기 위한 Buffer
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;

	Vec3 _translation = Vec3(0.f, 0.f, 0.f);

	// Matrix
	Matrix _world = Matrix::Identity;			// 물체마다 다른 Transform
	Matrix _view = Matrix::Identity;			// Camera
	Matrix _projection = Matrix::Identity;		// Camera
};

