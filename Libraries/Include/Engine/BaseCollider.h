#pragma once
#include "Component.h"

enum class ColliderType
{
	Sphere,
	// Box Collider이며 축(x, y, z)들이 World 좌표계의 축(x, y, z)들과 평행(같은 방향을 가지면)
	// 판정이 단순하지만, 오브젝트가 회전하면 판정의 정확도가 떨어짐 
	AABB,
	// Box Collider이며 축(x, y, z)들이 World의 축을 따르지 않고 오브젝트이 로컬 축 방향을 따름.
	// 판정이 복잡한 대신, 오브젝트의 이동과 상관없이 판정의 정확도가 높음
	OBB
};

class BaseCollider : public Component
{
	using Super = Component;
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	// Ray에 대해서 Collider가 충돌한다면 True를 반환하고 Ray.Position과의 거리를 반환함
	// @params ray : RayCasting에 대한 정보
	// @params distance : Ray에 대해서 충돌이 있으면 거리를 얻을 수 있음
	virtual bool Intersects(Ray& ray, OUT float& distance) = 0;

	// Collider끼리 충돌에 대한 판정 검사
	virtual bool Intersects(shared_ptr<BaseCollider>& other) = 0;

	ColliderType GetColliderType() { return _colliderType; }

protected:
	ColliderType _colliderType;
};

