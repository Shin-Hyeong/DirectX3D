#pragma once
#include "IExecute.h"

class InstancingDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader>				_shader;
	shared_ptr<GameObject>			_camera;
	vector<shared_ptr<GameObject>>	_objs;


private: // INSTANCING
	shared_ptr<Mesh>			_mesh;		// 원본 mesh
	shared_ptr<Material>		_material;	// 원본 material
	
	vector<Matrix>				_worlds;	// 물체들의 Transform 정보
	shared_ptr<VertexBuffer>	_instanceBuffer;
};

