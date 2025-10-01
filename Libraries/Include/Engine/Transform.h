#pragma once
#include "Component.h"

class Transform : public Component
{
	using Super = Component;
public:
	Transform();
	~Transform();

	virtual void Awake() override;
	virtual void Update() override;

	void UpdateTransform();

	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }


	// World
	Vec3 GetWorldScale() { return _scale; }
	void SetWorldScale(const Vec3& Scale);
	Vec3 GetWorldRotation() { return _rotation; }
	void SetWorldRotation(const Vec3& Rotation);
	Vec3 GetWorldPosition() { return _position; }
	void SetWorldPosition(const Vec3& Position);

	Matrix GetWorldMatrix() { return _matWorld; }
	Vec3 GetRight() { return _matWorld.Right(); } // _matWorld�� 1��
	Vec3 GetUp() { return _matWorld.Up(); } // _matWorld�� 2��
	Vec3 GetForward() { return _matWorld.Backward(); } // _matWorld�� 3��, ������ ��ǥ��

	// ���� ����
	bool HasParent() { return _parent != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	vector<shared_ptr<Transform>> GetChildrend() { return _children; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	// �θ� GameObject�� ���������� �ڽ��� SRT
	Vec3 _localScale = { 1.f, 1.f, 1.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localPosition = { 0.f, 0.f, 0.f };


	// Cache
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	Vec3 _right;
	Vec3 _up;
	Vec3 _forward;

private:
	// �ڽ��� �θ� GameObject Transform
	shared_ptr<Transform> _parent = nullptr;
	// �ڽ��� �ڽ� GameObject Transform
	vector<shared_ptr<Transform>> _children;

	
};