#pragma once
#include "IExecute.h"

class GroupDemo : public IExecute
{
	// CS()에 데이터를 전달하기 위한 구조체
	struct Input
	{
		float value;
	};


	// CS()에서 반환하는 데이터를 받아주기 위한 구조체
	struct Output
	{
		uint32 groupID[3];				// ThreadGroup
		uint32 groupThreadID[3];		// ThreadGroup내 Thread
		uint32 dispatchThreadID[3];		// 전체 ThreadGroup내 번호
		uint32 groupIndex;				// ThreadGroup내 1차원 좌표
		float value;	// input으로 전달한 데이터가 연산되어 반환 받을 곳
	};

public:

	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader>				_shader;
	
};

