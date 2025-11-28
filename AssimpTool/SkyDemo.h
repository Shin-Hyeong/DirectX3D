#pragma once
#include "IExecute.h"

// Animation이 적용되지 않은 StaticMesh를 렌더링하는 환경

class SkyDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


private:
	shared_ptr<Shader>			_shader;

	shared_ptr<GameObject>		_obj;
	shared_ptr<GameObject>		_camera;

};

