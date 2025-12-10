#include "pch.h"
#include "GroupDemo.h"
#include "RawBuffer.h"


void GroupDemo::Init()
{
	_shader = make_shared<Shader>(L"25. GroupDemo.fx");

	// 하나의 ThreadGroup 내의 Thread 갯수
	uint32 threadCount = 10 * 8 * 3;
	// Thread Group 갯수
	uint32 groupCount = 2 * 1 * 1;
	// 최종 Thread 개수
	uint32 count = threadCount * groupCount;


	// 임의의 Input 값 설정
	vector<Input> inputs(count);
	for (int32 i = 0; i < count; i++)
		inputs[i].value = rand() % 10000;
	

	// inputs의 데이터를 설정한 크기(count)만큼 RawBuffer에 넘김.
	// Output * count 크기만큼 반환함.
	shared_ptr<RawBuffer> rawBuffer = make_shared<RawBuffer>(inputs.data(), sizeof(inputs) * count, sizeof(Output) * count);

	// shader의 Input에 rawBuffer의 srv를연결함
	_shader->GetSRV("Input")->SetResource(rawBuffer->GetSRV().Get());
	// shader의 Output에 rawBuffer의 uav를 연결함
	_shader->GetUAV("Output")->SetUnorderedAccessView(rawBuffer->GetUAV().Get());

	// ComputeShader를 실행 및 ThreadGroup설정 ( Rendering의 DrawCall과 비슷)
	// Dispatch(technique, pass, x, y, z)		x, y, z = thread Group 개수 설정
	_shader->Dispatch(0, 0, 2, 1, 1);

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
		"GroupID(X),GroupID(Y),GroupID(Z),GroupThreadID(X),GroupThreadID(Y),GroupThreadID(Z),DispatchThreadID(X),DispatchThreadID(Y),DispatchThreadID(Z),GroupIndex, Before, After\n"
	);

	for (uint32 i = 0; i < count; i++)
	{
		const Output& temp = outputs[i];

		::fprintf
		(
			file,
			"%d,%d,%d,	%d,%d,%d,	%d,%d,%d,	%d,  %f,%f\n",
			temp.groupID[0], temp.groupID[1], temp.groupID[2],
			temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
			temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
			temp.groupIndex, inputs[i].value, temp.value
		);
	}

	::fclose(file);
}

void GroupDemo::Update()
{

}

void GroupDemo::Render()
{

}
