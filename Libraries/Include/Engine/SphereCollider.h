#pragma once
#include "BaseCollider.h"
class SphereCollider : public BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

	virtual void Update() override;

	// Ray에 대해서 Collider가 충돌한다면 True를 반환하고 Ray.Position과의 거리를 반환함
	// @params ray : RayCasting에 대한 정보
	// @params distance : Ray에 대해서 충돌이 있으면 거리를 얻을 수 있음
	virtual bool Intersects(Ray& ray, OUT float& distance) override;

	// Collider끼리 충돌에 대한 판정 검사
	// @params other : 본인과 다른 Collider
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	void SetRadius(float radius) { _radius = radius; }
	BoundingSphere& GetBoundingSphere() { return _boundingSphere; }

private:
	// 중심점과 반경의 데이터를 가지고 있으며, Ray Casting이나 충돌에 대한 rough culling을 지원함
	BoundingSphere _boundingSphere;
	// Sphere의 반경
	float _radius = 1;
};

