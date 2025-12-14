#include "pch.h"
#include "OBBBoxCollider.h"
#include "AABBBoxCollider.h"
#include "SphereCollider.h"

OBBBoxCollider::OBBBoxCollider() : BaseCollider(ColliderType::OBB)
{

}

OBBBoxCollider::~OBBBoxCollider()
{

}

void OBBBoxCollider::Update()
{
	_boundingBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	// Vec3 Scale = GetGameObject()->GetTransform()->GetWorldScale();
	// _boundingBox.Extents = Scale;
}

bool OBBBoxCollider::Intersects(Ray& ray, OUT float& distance)
{
	return _boundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

bool OBBBoxCollider::Intersects(shared_ptr<BaseCollider>& other)
{
	// 다른 Collider의 CollliderType
	ColliderType type = other->GetColliderType();

	// _boundingBox의 Intersects()에 타입별로 대응하는 함수가 오버로드 되어 있음
	switch (type)
	{
	case ColliderType::Sphere:
		return _boundingBox.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());

	case ColliderType::AABB:
		return _boundingBox.Intersects(dynamic_pointer_cast<AABBBoxCollider>(other)->GetBoundingBox());

	case ColliderType::OBB:
		return _boundingBox.Intersects(dynamic_pointer_cast<OBBBoxCollider>(other)->GetBoundingBox());
	}

	return false;
}
