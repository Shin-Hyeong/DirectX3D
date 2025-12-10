#pragma once

class StructuredBuffer
{
public:
	// @params inputData : 연산할 구조체 데이터
	// @params inputStride : 연산할 구조체의 크기
	// @params inputCount : 연산할 구조체의 개수
	// @params outputStride : 연산된(출력된) 구조체의 크기
	// @params outputCount : 연산된(출력된) 구조체의 개수
	StructuredBuffer(void* inputData, uint32 inputStride, uint32 inputCount, uint32 outputStride = 0, uint32 outputCount = 0);
	~StructuredBuffer();

public:
	void CreateBuffer();

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	// 입력된 구조체의 총합 크기 구하기(구조체 크기 * 구조체 개수)
	uint32 GetInputByteWidth() { return _inputStride * _inputCount; }
	// 연산된(출력된) 구조체의 총합 크기 구하기(구조체 크기 * 구조체 개수)
	uint32 GetOutputByteWidth() { return _outputStride * _outputCount; }

	// Cpu 메모리에서 _input Buffer에 복사 전달함
	// @params data : inputBuffer에 데이터를 복사 전달할 데이터
	void CopyToInput(void* data);
	// result Buffer에 있는 데이터를 Cpu메모리에 저장함
	// @params data : result에서 데이터를 받아서 저장할 변수.
	void CopyFromOutput(void* data);


public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	ComPtr<ID3D11Buffer> _input;			// 연산할 Struct
	ComPtr<ID3D11ShaderResourceView> _srv;	// Input
	ComPtr<ID3D11Buffer> _output;			// 연산된 Struct
	ComPtr<ID3D11UnorderedAccessView> _uav; // Output
	ComPtr<ID3D11Buffer> _result;			// 최종 결과 Struct

private:
	void* _inputData;

	uint32 _inputStride = 0;	// Input 구조체의 크기
	uint32 _inputCount = 0;		// Input 구조체의 개수
	uint32 _outputStride = 0;	// Output 구조체의 크기
	uint32 _outputCount = 0;	// Output 구조체의 개수
};

