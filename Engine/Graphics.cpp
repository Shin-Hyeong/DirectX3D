#include "pch.h"
#include "Graphics.h"

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	SetViewport();
}

void Graphics::RenderBegin()
{
	// DSV을 추가 연결. 그릴때마다 최종적으로 Depth값을 DSV에 저장함
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	// 렌더링이 시작되기 전에 RTV 전체를 지정한 색상으로 덮어써서 이전 프레임의 잔상을 지우는 역할
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), (float*)(&GAME->GetGameDesc().clearColor));
	// 렌더링이 시작되기 전에 이전 Depth값을 가지고 있던 DSV 전체를 초기화(Depth : 1, Stencil : 0)하여 새로운 값을 받을 수 있도록 함
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}
void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GAME->GetGameDesc().width;
		desc.BufferDesc.Height = GAME->GetGameDesc().height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// BackBuffer에 있는 Texture을 묘사하는 View 생성
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Graphics::CreateDepthStencilView()
{
	// Depth Stencil Textrue 생성
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ZeroMemory(&desc, sizeof(desc));

		// 화면 크기의 Texture을 생성함 -> 화면에 그려진 물체의 Depth값을 저장할 것임
		desc.Width = static_cast<uint32>(GAME->GetGameDesc().width);
		desc.Height = static_cast<uint32>(GAME->GetGameDesc().height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		// Depth : 24bit, Stencil : 8bit
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // DepthStencil 용도로 사용할 예정
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _depthStencilTexture.GetAddressOf());
		CHECK(hr);
	}

	// Depth Stencil View 생성
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// Depth : 24bit, Stencil : 8bit
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		// _depthStencilTexture을 넣은 _depthStencilView을 생성
		HRESULT hr = DEVICE->CreateDepthStencilView(_depthStencilTexture.Get(), &desc, _depthStencilView.GetAddressOf());
		CHECK(hr);
	}
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(GAME->GetGameDesc().width);
	_viewport.Height = static_cast<float>(GAME->GetGameDesc().height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}