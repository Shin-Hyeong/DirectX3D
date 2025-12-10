#pragma once

// GPU에 연산할 데이터를 받아서 ComputeShader에 전달.
// 연산된 데이터를 다시 받는 역할

// GPU에 데이터 전달하기 순서 : _inputData -> _input -> _srv -> GPU
// GPU에 데이터 받아오기 순서 :  GPU -> _uav -> _output -> _result -> data

class RawBuffer
{
public:
	// Cpu에 있는 데이터를 Raw Buffer 생성 및 Buffer에 저장
	// @params inputData : GPU에 연산시킬 데이터
	// @params inputByte : 연산시킬 데이터 크기
	// @params outputbyte : 연산된 데이터 크기
	RawBuffer(void* inputData, uint32 inputByte, uint32 outputByte);
	~RawBuffer();

public:
	// 필요한 Buffer, View 생성
	void CreateBuffer();

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
	void CreateInput();		// 연산할 데이터를 넣은 Raw Buffer 생성
	void CreateSRV();		// _input를 묘사하기 위한 SRV 생성
	void CreateOutput();	
	void CreateUAV();		// 연산된 데이터를 받기 위한 UAV 생성
	void CreateResult();

private:
	// INPUT
	ComPtr<ID3D11Buffer>				_input;		// 데이터를 입력할 때 사용하는 buffer
	// ID3D11ShaderResourceView : GPU 셰이더가 해당 리소스를 읽기 전용으로 접근함
	ComPtr<ID3D11ShaderResourceView>	_srv;		// 입력할 때 사용하는 View

	// OUTPUT
	ComPtr<ID3D11Buffer>				_output;	// 연산된 데이터를 받아올 때 사용하는 buffer
	// ID3D11UnorderedAccessView : GPU 셰이더가 해당 리소스를 읽기 / 쓰기 모두 가능함.
	ComPtr<ID3D11UnorderedAccessView>	_uav;		// 데이터를 받을 때 사용하는 View
	ComPtr<ID3D11Buffer>				_result;	// 최종 결과를 저장

private:
	void*		_inputData;			// 입력 받을 데이터
	uint32		_inputByte = 0;		// 입력 받을 데이터 크기
	uint32		_outputByte = 0;	// 반환 받는 데이터 크기
};

