#include "pch.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer(void* inputData, uint32 inputByte, uint32 outputByte)
	: _inputData(inputData), _inputByte(inputByte), _outputByte(outputByte)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{

}

void RawBuffer::CreateBuffer()
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void RawBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(_input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		::memcpy(subResource.pData, data, _inputByte);
	}
	DC->Unmap(_input.Get(), 0);
}

void RawBuffer::CopyFromOutput(void* data)
{
	// Raw Buffer(_output)에 있는 데이터를 일반 Buffer(_result)에 복사
	// _output 데이터 -> result에 복사
	DC->CopyResource(_result.Get(), _output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(_result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		// _result에 있는 suBresource를 _outputByte만큼 data에 복사 전달함.
		// _result -> data
		::memcpy(data, subResource.pData, _outputByte);
	}
	DC->Unmap(_result.Get(), 0);
}

void RawBuffer::CreateInput()
{
	if (_inputByte == 0)
		return;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = _inputByte;	// 버퍼 크기
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// 셰이더 단계에 바인딩
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; // Raw Buffer로 설정
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU-WRITE, GPU-READ
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = _inputData; // GPU에 전달할 데이터

	// _input에 _inputData를 넣고 Raw Buffer 저장
	if (_inputData != nullptr)
		CHECK(DEVICE->CreateBuffer(&desc, &subResource, _input.GetAddressOf()));
	else	// 예외처리. 빈 Buffer를 생성함.
		CHECK(DEVICE->CreateBuffer(&desc, nullptr, _input.GetAddressOf()));
}

void RawBuffer::CreateSRV()
{
	if (_inputByte == 0)
		return;

	D3D11_BUFFER_DESC desc;
	_input->GetDesc(&desc);		// _input의 BufferDesc를 가져옴

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	// DXGI_FORMAT_R32_TYPELESS : 32bit(4byte)의 타입이 없는 싱글 컴포넌트
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS; // 쉐이더에서 알아서 하도록 함
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;	// SRV_FLAG_RAW
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	// 전체 데이터 개수 = Format이 4byte이기 때문에 4를 나눔
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;		

	// _input에 대한 SRV를 생성해서 _srv에 저장함
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

void RawBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = _outputByte;	// 버퍼의 크기
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;	// unordered 리소스 바인딩
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; // Raw Buffer로 설정

	// 값이 비어는 RawBuffer 생성해서 _output에 저장
	// GPU에 메모리만 할당함
	CHECK(DEVICE->CreateBuffer(&desc, NULL, _output.GetAddressOf()));
}

void RawBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	// DXGI_FORMAT_R32_TYPELESS : 32bit(4byte)의 타입이 없는 싱글 컴포넌트
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;	// 쉐이더에서 알아서 하도록 함
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER; // SRV_FLAG_RAW
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;	// 리소스에는 구조화되지 않은 원시 데이터가 포함
	// 전체 데이터 개수 = Format이 4byte이기 때문에 4를 나눔
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;

	// _output에 대한 UAV를 생성해서 _uav에 저장함
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

void RawBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	// Cpu 메모리에 복사하기위 설정
	desc.Usage = D3D11_USAGE_STAGING;	// GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = D3D11_USAGE_DEFAULT; // UAV가 연결되려면, USAGE는 DEFALT여야함.
	desc.MiscFlags = 0; // 특수 Buffer 기능 비활성화 = 기본 Buffer

	// 값이 비어있는 Buffer를 생성해서 _result에 저장
	CHECK(DEVICE->CreateBuffer(&desc, nullptr, _result.GetAddressOf()));
}
