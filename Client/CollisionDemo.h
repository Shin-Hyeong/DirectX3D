#pragma once
#include "IExecute.h"

class CollisionDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader>				_shader;
	
private:

};


// Mesh를 이동시키기 위한 Script
#include "MonoBehaviour.h"
class MoveScript : public MonoBehaviour
{
public:
	virtual void Update() override;
};
