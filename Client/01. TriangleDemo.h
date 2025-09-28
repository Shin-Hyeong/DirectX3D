#pragma once
#include "IExecute.h"

class TriangleDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// ���̴�
	shared_ptr<Shader> _shader;
	// ���� ����
	vector<VertexData> _vertices;
	// GPU�� �����ϱ� ���� Buffer
	shared_ptr<VertexBuffer> _buffer;
};

