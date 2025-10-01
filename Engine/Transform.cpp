#include "pch.h"
#include "Transform.h"

Transform::Transform() : Super(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Awake()
{

}

void Transform::Update()
{

}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// Quaternion → Angles
Vec3 ToEulerAngles(Quaternion q) 
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

// Local Scale, LocalRotation, Local Position이 변경될때마다 호출됨.
void Transform::UpdateTransform()
{
	// 스케일 - 자전 - 이동 - 공전 - 부모 순으로 곱해야 원하는 행렬 좌표를 얻게됨
	Matrix matScale = Matrix::CreateScale(_localScale);				// Scale
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);	// Rotation
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);		// Translation

	// Local SRT행렬
	_matLocal = matScale * matRotation * matTranslation;

	// 부모가 있으면
	if (HasParent())
	{
		// 부모의 월드좌표 구하기
		// 부모도 누구의 자식이면 UpdateTransform()을 반복해 월드좌표를 얻을 수 있음
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	// 부모가 없으면
	// 자신의 Local Matrix가 World Matrix임
	else
	{
		_matWorld = _matLocal;
	}

	// 월드 좌표 Cache
	Quaternion quat;
	// 월드 변환 행렬의 SRT를 따로따로 분리해서 저장
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	// Children
	for (const shared_ptr<Transform>& child : _children)
	{
		// 자식도 업데이트
		child->UpdateTransform();
	}
}


void Transform::SetWorldScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		// 부모의 scale에 영향을 받음
		// 부모가 4배 커지면 자식들도 4배 커짐.
		// 하지만 본인이 world기준으로 2배 사이즈를 유지할려고
		// 4(부모) * x(본인) = 2(world)
		// x(본인) = 2(world) / 4(부모), x = 0.5가 됨
		Vec3 parentScale = _parent->GetWorldScale();
		Vec3 scale = worldScale;

		scale.x /= parentScale.x; // world * parent
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;

		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetWorldRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		// world좌표계 -> 부모 좌표계로 가는 행렬
		Matrix inverseMatrix = _parent->GetWorldMatrix().Invert();

		// 전달 받은 world Rotation을  world좌표계를 부모 좌표계로 변환함
		// 위치 이동이 아닌 방향만 바꿀것이기 때문에 TransformNormal()를 사용
		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
	{
		SetLocalRotation(worldRotation);
	}
}

void Transform::SetWorldPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		// world좌표계 -> 부모 좌표계로 가는 행렬
		Matrix worldToParentLocalmatrix = _parent->GetWorldMatrix().Invert();

		// 전달 받은 world Position을  world좌표계를 부모 좌표계로 변환함
		Vec3 position;
		position.Transform(worldPosition,worldToParentLocalmatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
