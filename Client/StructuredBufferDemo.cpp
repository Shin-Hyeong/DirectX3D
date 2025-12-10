#include "pch.h"
#include "StructuredBufferDemo.h"
#include "StructuredBuffer.h"


void StructuredBufferDemo::Init()
{
	_shader = make_shared<Shader>(L"27. StructuredBufferDemo.fx");

	// 연산할 데이터
	vector<Matrix> inputs(500, Matrix::Identity);

	// StructuredBuffer 생성
	auto buffer = make_shared<StructuredBuffer>(inputs.data(), sizeof(Matrix), 500, sizeof(Matrix), 500);

	// 셰이더의 Input과 SRV연결
	_shader->GetSRV("Input")->SetResource(buffer->GetSRV().Get());
	// 셰이더의 Output과 UAV연결
	_shader->GetUAV("Output")->SetUnorderedAccessView(buffer->GetUAV().Get());

	// Dispatch(technique, pass, x, y, z)
	_shader->Dispatch(0, 0, 1, 1, 1);

	vector<Matrix> outputs(500);
	// 연산된 데이터를 CPU 메모리로 가져옴
	buffer->CopyFromOutput(outputs.data());

}

void StructuredBufferDemo::Update()
{

}

void StructuredBufferDemo::Render()
{

}
