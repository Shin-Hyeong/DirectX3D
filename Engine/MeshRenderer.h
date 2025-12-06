#pragma once
#include "Component.h"

class Mesh;
class Shader;
class Material;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	// virtual void Update() override;

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetPass(uint8 pass) { _pass = pass; }

	// _mesh와 _materila의 주소값을 pair로 반환함
	InstanceID GetInstanceID();

	// Instancing된 Mesh들을 렌더링함
	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);


private:
	shared_ptr<Mesh>		_mesh;
	shared_ptr<Material>	_material;
	uint8					_pass = 0;
};

