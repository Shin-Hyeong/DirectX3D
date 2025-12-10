#include "pch.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ComPtr<ID3D11Texture2D> src)
{
	CreateInput(src);
	CreateBuffer();
}

TextureBuffer::~TextureBuffer()
{

}

void TextureBuffer::CreateBuffer()
{
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void TextureBuffer::CreateInput(ComPtr<ID3D11Texture2D> src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	// Texture의 정보 가져오기
	_width = srcDesc.Width;
	_height = srcDesc.Height;
	_arraySize = srcDesc.ArraySize;
	_format = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;	// 픽셀당 멀티 샘플의 수, 안티 에일리어싱 관련

	// _input에 빈 Texture2D 인터페이스를 생성
	CHECK(DEVICE->CreateTexture2D(&desc, NULL, _input.GetAddressOf()));

	// _input에 src를 복사해서 저장
	DC->CopyResource(_input.Get(), src.Get());
}

void TextureBuffer::CreateSRV()
{
	// 전달할 Texture2D의 DESC를 복사 저장
	D3D11_TEXTURE2D_DESC desc;
	_input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; // Texture2DArray
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	// _input의 Texture에 대한 SRV를 생성해서 _srv에 저장
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

void TextureBuffer::CreateOutput()
{
	// 연산된 Texture를 받기 위한 DESC 설정
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	// unordered Resource 바인딩 | Resource 바인딩
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	// _output에 빈 Texture2D 인터페이스를 생성
	CHECK(DEVICE->CreateTexture2D(&desc, nullptr, _output.GetAddressOf()));
}

void TextureBuffer::CreateUAV()
{
	// _output의 DESC 복사 저장
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;		// Texture2DArray
	uavDesc.Texture2DArray.ArraySize = _arraySize;

	// _output에 대한 UAV를 생성해서 _uav에 저장
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

void TextureBuffer::CreateResult()
{
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc);

	// UAV의 데이터를 읽기용으로 사용하기 위해 SRV를 사용함
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	// _output에 대한 SRV 생성해서 _outputSRV에 저장
	CHECK(DEVICE->CreateShaderResourceView(_output.Get(), &srvDesc, _outputSRV.GetAddressOf()));
}
