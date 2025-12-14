#include "pch.h"
#include "Viewport.h"

Viewport::Viewport()
{
	Set(800, 600);
}

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{

}

void Viewport::RSSetViewport()
{
	DC->RSSetViewports(1, &_viewport);
}

void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	_viewport.TopLeftX = x;
	_viewport.TopLeftY = y;
	_viewport.Width = width;
	_viewport.Height = height;
	_viewport.MinDepth = minDepth;
	_viewport.MaxDepth = maxDepth;
}

Vec3 Viewport::Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Matrix wvp = W * V * P;

	// pos * wvp를 곱한 좌표 = 투영 좌표계에 있는 좌표
	Vec3 p = Vec3::Transform(pos, wvp);

	// Viewport 설정에 맞게 2D 좌표를 구함
	p.x = (p.x + 1.0f) * (_viewport.Width / 2) + _viewport.TopLeftX;			// X
	p.y = (-p.y + 1.0f) * (_viewport.Height / 2) + _viewport.TopLeftY;			// Y
	p.z = p.z * (_viewport.MaxDepth - _viewport.MinDepth) + _viewport.MinDepth; // Z

	return p;
}

Vec3 Viewport::Unproject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Vec3 p = pos;

	// Viewport의 설정에 맞게 3D좌표를 구함
	p.x = 2.f * (p.x - _viewport.TopLeftX) / _viewport.Width - 1.f;					// X
	p.y = -2.f * (p.y - _viewport.TopLeftY) / _viewport.Height + 1.f;				// Y
	p.z = ((p.z - _viewport.MinDepth) / (_viewport.MaxDepth - _viewport.MinDepth));	// Z

	Matrix wvp = W * V * P;
	// WVP의 역행렬
	Matrix wvpInv = wvp.Invert();

	// pos * wvp의 역행렬의 좌표
	p = Vec3::Transform(p, wvpInv);
	return p;
}
