#pragma once
#include "IExecute.h"
#include "Geometry.h"

class GameObject;

// ��ü�� ������ �ְ� ī�޶� �̵���

class NormalDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// ���̴�
	shared_ptr<Shader>								_shader;
	
	// Object
	shared_ptr<Geometry<VertexTextureNormalData>>	_geometry;
	// MeshRenderer, GPU�� �����ϱ� ���� Buffer
	shared_ptr<VertexBuffer>						_vertexBuffer;
	shared_ptr<IndexBuffer>							_indexBuffer;
	// Matrix
	Matrix _world = Matrix::Identity;			// ��ü���� �ٸ� Transform


	// Camera
	shared_ptr<GameObject>							_camera;

	// Textrue
	shared_ptr<Texture>								_texture;

	// Light
	// ���� ���� (�¿�, ����, �յ�)
	Vec3 _lightDir = Vec3(-1.f, -1.f, 1.f);
};

