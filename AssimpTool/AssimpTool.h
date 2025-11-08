#pragma once
#include "IExecute.h" // 실행 단위

class AssimpTool : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:

};

