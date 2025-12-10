#include "pch.h"
#include "RawBufferDemo.h"
#include "RawBuffer.h"


void RawBufferDemo::Init()
{
	_shader = make_shared<Shader>(L"24. RawBufferDemo.fx");

	// 하나의 Thread 그룹내에서 사용할 Thread 수
	uint32 count = 10 * 8 * 3;
	
	shared_ptr<RawBuffer> rawBuffer = make_shared<RawBuffer>(nullptr, 0, sizeof(Output) * count);

	// shader의 Output에 rawBuffer의 uav를 연결함
	_shader->GetUAV("Output")->SetUnorderedAccessView(rawBuffer->GetUAV().Get());
	// ComputeShader를 실행 ( Rendering의 DrawCall과 비슷)
	// Dispatch(technique, pass, x, y, z)		x, y, z = thread Group 개수 설정
	_shader->Dispatch(0, 0, 1, 1, 1);

	// shader에 있는 RWByteAddressBuffer의 데이터를 outputs에 복사함
	// GPU 메모리 -> CPU 메모리
	vector<Output> outputs(count);
	rawBuffer->CopyFromOutput(outputs.data());

	// 결과물을 csv 파일로 추출
	FILE* file;
	::fopen_s(&file, "../RawBuffer.csv", "w");

	::fprintf
	(
		file,
		"GroupID(X),GroupID(Y),GroupID(Z),GroupThreadID(X),GroupThreadID(Y),GroupThreadID(Z),DispatchThreadID(X),DispatchThreadID(Y),DispatchThreadID(Z),GroupIndex\n"
	);

	for (uint32 i = 0; i < count; i++)
	{
		const Output& temp = outputs[i];

		::fprintf
		(
			file,
			"%d,%d,%d,	%d,%d,%d,	%d,%d,%d,	%d\n",
			temp.groupID[0], temp.groupID[1], temp.groupID[2],
			temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
			temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
			temp.groupIndex
		);
	}

	::fclose(file);
}

void RawBufferDemo::Update()
{

}

void RawBufferDemo::Render()
{

}
