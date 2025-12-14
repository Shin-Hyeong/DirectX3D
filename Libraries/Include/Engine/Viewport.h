#pragma once
class Viewport
{
public:
	Viewport();
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	// 현재 설정된 Viewport 값을 가지고 RS단계에 설정함
	void RSSetViewport();
	// 생성자에서 설정값 값을 수정하기 위한 함수
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidht() { return _viewport.Width; }
	float GetHeight() { return _viewport.Height; }

	// 3D 좌표에서 2D 좌표로 변환하기
	// @params pos  : Local 좌표계의 3D 좌표(World 좌표이면 W 값을 Identity로 설정)
	// @params W : World 변환 행렬
	// @params V : View 변환 행렬
	// @params P : Prjoction 변환 행렬
	Vec3 Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);

	// 2D 좌표에서 3D 좌표로 변환하기
	Vec3 Unproject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);

private:
	D3D11_VIEWPORT _viewport = { 0 };
};

