#pragma once
#include "BaseCollider.h"
class OBBBoxCollider : public BaseCollider
{
public:
	OBBBoxCollider();
	virtual ~OBBBoxCollider();

	virtual void Update() override;

	// ray에 대한 충돌 판정 검사
	virtual bool Intersects(Ray& ray, OUT float& distance) override;

	// Collider끼리 충돌에 대한 판정 검사
	// @params other : 본인과 다른 Collider
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	BoundingOrientedBox& GetBoundingBox() { return _boundingBox; }

private:
	BoundingOrientedBox _boundingBox;
};

