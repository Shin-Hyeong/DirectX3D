#include "pch.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"

SphereCollider::SphereCollider()
	: BaseCollider(ColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Update()
{
	// Collider 중심점 설정
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 Scale = GetGameObject()->GetTransform()->GetWorldScale();
	// Scale값에서 가장 큰 수치와 _radius(외부에서 조정가능한 수치)를 곱한 값을 반경으로 설정함
	_boundingSphere.Radius = _radius * max(max(Scale.x, Scale.y), Scale.z);
}

bool SphereCollider::Intersects(Ray& ray, OUT float& distance)
{
	// ray과 Collider에 대해 충돌 여부를 True/False반환하고 충돌이 일어났으면 distance를 출력함
	return _boundingSphere.Intersects(ray.position, ray.direction, OUT distance);
}

bool SphereCollider::Intersects(shared_ptr<BaseCollider>& other)
{
	// 다른 Collider의 CollliderType
	ColliderType type = other->GetColliderType();

	// _boundingSphere의 Intersects()에 타입별로 대응하는 함수가 오버로드 되어 있음
	switch (type)
	{
	case ColliderType::Sphere:
		return _boundingSphere.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
		
	case ColliderType::AABB:
		return _boundingSphere.Intersects(dynamic_pointer_cast<AABBBoxCollider>(other)->GetBoundingBox());

	case ColliderType::OBB:
		return _boundingSphere.Intersects(dynamic_pointer_cast<OBBBoxCollider>(other)->GetBoundingBox());
	}

	return false;
}

