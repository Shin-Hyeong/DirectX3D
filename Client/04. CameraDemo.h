#pragma once
#include "IExecute.h"
#include "Geometry.h"

class GameObject;

// 물체는 가만히 있고 카메라가 이동함

class CameraDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// 쉐이더
	shared_ptr<Shader>						_shader;
	
	// Object
	shared_ptr<Geometry<VertexColorData>>	_geometry;
	// MeshRenderrer, GPU에 전달하기 위한 Buffer
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;
	// Matrix
	Matrix _world = Matrix::Identity;			// 물체마다 다른 Transform


	// Camera
	shared_ptr<GameObject>					_camera;
};

