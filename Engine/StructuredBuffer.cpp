#include "pch.h"
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(void* inputData, uint32 inputStride, uint32 inputCount, uint32 outputStride, uint32 outputCount)
	: _inputData(inputData), _inputStride(inputStride), _inputCount(inputCount), _outputStride(outputStride), _outputCount(outputCount)
{
	// 연산된(출력된) 구조체의 크기와 개수가 설정되어 있지 않으면 input구조체와 같은 설정을 함
	if (outputStride == 0 || outputCount == 0)
	{
		_outputStride = inputStride;
		_outputCount = inputCount;
	}

	CreateBuffer();
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::CreateBuffer()
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void StructuredBuffer::CreateInput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	
	desc.ByteWidth = GetInputByteWidth();			// 구조체의 총합 크기
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Shader Resource
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // Structured Buffer
	desc.StructureByteStride = _inputStride;		// Struct 크기
	desc.Usage = D3D11_USAGE_DYNAMIC;				// GPU 읽기, CPU 쓰기
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = _inputData;	// inputData 복사

	if (_inputData != nullptr)
		CHECK(DEVICE->CreateBuffer(&desc, &subResource, _input.GetAddressOf()));
	else	// 예외 처리, 빈 Buffer 생성
		CHECK(DEVICE->CreateBuffer(&desc, nullptr, _input.GetAddressOf()));
}

void StructuredBuffer::CreateSRV()
{
	// InputBuffer에 대한 정보 복사 저장
	D3D11_BUFFER_DESC desc;
	_input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;		// 형식을 알수 없음
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; // Resource가 RawBuffer임
	srvDesc.BufferEx.NumElements = _inputCount; // 입력 구조체 개수

	// _input에 대한 SRV를 _srv에 저장
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

void StructuredBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetOutputByteWidth();			// 구조체의 총합 크기
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;	// Unoredered Access에 바인딩
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// Resource를 StructuredBuffer로 사용하도록 함
	desc.StructureByteStride = _outputStride;

	// 빈 Buffer를 _output에 저장
	CHECK(DEVICE->CreateBuffer(&desc, nullptr, _output.GetAddressOf()));
}

void StructuredBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;		// 형식을 알수 없음
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER; // Resource를 Buffer로 설정
	uavDesc.Buffer.NumElements = _outputCount;	// 출력 구조체 개수

	// _output를 묘사하는 UAV를 생성해서 _uav에 저장함
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

void StructuredBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;	// GPU에서 CPU에게 데이터를 전송할때 사용함
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU가 읽을 수 있도록 허용함
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	// 빈 버퍼 상태로 생성해서 _result에 저장함
	CHECK(DEVICE->CreateBuffer(&desc, NULL, _result.GetAddressOf()));
}

void StructuredBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(_input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		// 설정된 데이터 크기만큼 전달된 data를 복사해서 _input Buffer에 복사 저장함.
		memcpy(subResource.pData, data, GetInputByteWidth());
	}
	DC->Unmap(_input.Get(), 0);
}

void StructuredBuffer::CopyFromOutput(void* data)
{
	// _output Buffer에 데이터를 _result 버퍼에 저장함
	DC->CopyResource(_result.Get(), _output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(_result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		// 설정된 데이터 크기만큼 _result Buffer에 있는 데이터를 복사해서 data에 저장함
		memcpy(data, subResource.pData, GetOutputByteWidth());
	}
	DC->Unmap(_result.Get(), 0);
}
