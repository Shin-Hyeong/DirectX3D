#pragma once
#include "IExecute.h"
#include "Geometry.h"

class ConstBufferDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// ���̴�
	shared_ptr<Shader>						_shader;
	
	// ����
	shared_ptr<Geometry<VertexColorData>>	_geometry;

	// GPU�� �����ϱ� ���� Buffer
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;

	Vec3 _translation = Vec3(0.f, 0.f, 0.f);

	// Matrix
	Matrix _world = Matrix::Identity;			// ��ü���� �ٸ� Transform
	Matrix _view = Matrix::Identity;			// Camera
	Matrix _projection = Matrix::Identity;		// Camera
};

