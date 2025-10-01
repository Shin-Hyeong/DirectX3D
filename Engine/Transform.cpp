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
// Quaternion �� Angles
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

// Local Scale, LocalRotation, Local Position�� ����ɶ����� ȣ���.
void Transform::UpdateTransform()
{
	// ������ - ���� - �̵� - ���� - �θ� ������ ���ؾ� ���ϴ� ��� ��ǥ�� ��Ե�
	Matrix matScale = Matrix::CreateScale(_localScale);				// Scale
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);	// Rotation
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);		// Translation

	// Local SRT���
	_matLocal = matScale * matRotation * matTranslation;

	// �θ� ������
	if (HasParent())
	{
		// �θ��� ������ǥ ���ϱ�
		// �θ� ������ �ڽ��̸� UpdateTransform()�� �ݺ��� ������ǥ�� ���� �� ����
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	// �θ� ������
	// �ڽ��� Local Matrix�� World Matrix��
	else
	{
		_matWorld = _matLocal;
	}

	// ���� ��ǥ Cache
	Quaternion quat;
	// ���� ��ȯ ����� SRT�� ���ε��� �и��ؼ� ����
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	// Children
	for (const shared_ptr<Transform>& child : _children)
	{
		// �ڽĵ� ������Ʈ
		child->UpdateTransform();
	}
}


void Transform::SetWorldScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		// �θ��� scale�� ������ ����
		// �θ� 4�� Ŀ���� �ڽĵ鵵 4�� Ŀ��.
		// ������ ������ world�������� 2�� ����� �����ҷ���
		// 4(�θ�) * x(����) = 2(world)
		// x(����) = 2(world) / 4(�θ�), x = 0.5�� ��
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
		// world��ǥ�� -> �θ� ��ǥ��� ���� ���
		Matrix inverseMatrix = _parent->GetWorldMatrix().Invert();

		// ���� ���� world Rotation��  world��ǥ�踦 �θ� ��ǥ��� ��ȯ��
		// ��ġ �̵��� �ƴ� ���⸸ �ٲܰ��̱� ������ TransformNormal()�� ���
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
		// world��ǥ�� -> �θ� ��ǥ��� ���� ���
		Matrix worldToParentLocalmatrix = _parent->GetWorldMatrix().Invert();

		// ���� ���� world Position��  world��ǥ�踦 �θ� ��ǥ��� ��ȯ��
		Vec3 position;
		position.Transform(worldPosition,worldToParentLocalmatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
