#pragma once
#include "IExecute.h"

class TriangleDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	// 쉐이더
	shared_ptr<Shader> _shader;
	// 정점 정보
	vector<VertexData> _vertices;
	// GPU에 전달하기 위한 Buffer
	shared_ptr<VertexBuffer> _buffer;
};

