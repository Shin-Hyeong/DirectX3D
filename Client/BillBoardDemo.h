#pragma once
#include "IExecute.h"


class BillBoardDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader>				_shader;
	
private:

};

// BillBoard ¿€µø Script
#include "MonoBehaviour.h"
class BillBoardTest : public MonoBehaviour
{
public:
	virtual void Update();
};