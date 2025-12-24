#pragma once
#include "Primitive3D.h"

struct MathUtils
{
	// ********************
	// Point3D(3차원 좌표)
	// ********************
	// Point3D가 기하학 도형에 포함되어이 있는지
	
	// Sphere to Point
	// Sphere안에 Point가 속해 있는지
	static bool PointInSphere(const Point3D& point, const Sphere3D& sphere);
	// Sphere 표면에서 Point와 가장 가까운 좌표는 어디인지
	static Point3D ClosestPoint(const Point3D& point, const Sphere3D& sphere);


	// AABB to Point
	// AABBBox 안에 Point가 속해 있는지
	static bool PointInAABB(const Point3D& point, const AABB3D& aabb);
	// AABBBox 표면에서 Point와 가장 가까운 좌표는 어디인지(안에 있으면 Point위치 반환)
	static Point3D ClosestPoint(const Point3D& point, const AABB3D& aabb);


	// OBB to Point
	// OBBBox 안에 Point가 속해 있는지
	static bool PointInOBB(const Point3D& point, const OBB3D& obb);
	// OBBBox 표면에서 Point와 가장 가까운 좌표는 어디인지(안에 있으면 Point위치 반환)
	static Point3D ClosestPoint(const Point3D& point, const OBB3D& obb);


	// Plane to Point
	// Plane 안에 Point가 속해 있는지
	static bool PointOnPlane(const Point3D& point, const Plane3D& plane);
	// Plane 표면에서 Point와 가장 가까운 좌표는 어디인지(안에 있으면 Point위치 반환)
	static Point3D ClosestPoint(const Point3D& point, const Plane3D& plane);

	
	// Line to Point
	// Line 안에 Point가 속해 있는지
	static bool PointOnLine(const Point3D& point, const Line3D& line);
	// Line 표면에서 Point와 가장 가까운 좌표는 어디인지(안에 있으면 Point위치 반환)
	static Point3D ClosestLine(const Point3D& point, const Line3D& line);


	// Ray to Point
	// Ray 안에 Point가 속해 있는지
	static bool PointOnLine(const Point3D& point, const Ray3D& ray);
	// Ray 표면에서 Point와 가장 가까운 좌표는 어디인지(안에 있으면 Point위치 반환)
	static Point3D ClosestLine(const Point3D& point, const Ray3D& ray);


	// *****************************
	// Intersection(충돌 & 교차될때)
	// *****************************
	
	// Sphere to Sphere
	static bool SphereToSphere(const Sphere3D& s1, const Sphere3D& s2);
	// Sphere to AABB
	static bool SphereToAABB(const Sphere3D& sphere, const AABB3D& aabb);
	// Sphere to OBB
	static bool SphereToOBB(const Sphere3D& sphere, const OBB3D& obb);
	// Sphere to Plane
	static bool SphereToPlane(const Sphere3D& sphere, const Plane3D& plane);

	// AABB to AABB
	static bool AABBToAABB(const AABB3D& aabb1, const  AABB3D& aabb2);

	// OBB와의 Intersection 검사 방법
	// Separating Axis Theorem(SAT)
	// 1. OBB와 비교할 도형을 임의의 축에 투영한다. -> 도형을 축에 투영하기 때문에 선분이 됨.
	// 2. 투영된 선분이 축을 기준으로 겹치는 부분이 있는지 확인 . 
	//  - x축으로 투영했다면 x값이 -1 ~ 0.5 / 0.2 ~ 1이면 겹침.
	// 3. 투영할 축은 OBB의 x,y,z축, 비교할 도형의 x,y,z축, 6개의 축끼리 외적을 구하여 9개의 축을 추가 구한다
	//  - 3 + 3+ 9 = 15을 비교한다.
	// 4. 모든 투영에 대해서 겹쳐야 겹친 판정으로 됨.


	// AABB도형을 axis축에 투영하여 도형의 차지하는 범위를 반환함
	// @params aabb : axis축에 투영할 AABB3D타입의 도형
	// @params axis : 투영될 축
	static Interval3D GetInterval(const AABB3D& aabb, const Vec3& axis);
	// OBB도형을 axis축에 투영하여 도형의 차지하는 범위를 반환함
	// @params obb : axis축에 투영할 AABB3D타입의 도형
	// @params axis : 투영될 축
	static Interval3D GetInterval(const OBB3D& obb, const Vec3& axis);
	// 두 도형이 axis축에 투영되어 겹치는 부분이 있는지 검사
	// @params aabb : axis 축에 투영될 AABB3D 도형
	// @params obb : axis 축에 투영될 OBB3D 도형
	// @params axis : 도형을 투영시킬 Axis축
	static bool OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis);
	static bool OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis);


	// AABB to OBB
	static bool AABBToOBB(const AABB3D& aabb, const OBB3D& obb);
	// AABB to Plane
	static bool AABBToPlane(const AABB3D& aabb, const Plane3D& plane);

	// OBB to OBB
	static bool OBBToOBB(const OBB3D& obb1, const OBB3D& obb2);

	// Plane to Plane
	static bool PlaneToPlane(const Plane3D& plane1, const Plane3D& plane2);

	// ********
	// RayCast
	// ********

	// Ray to Sphere
	static bool Raycast(const Sphere3D& sphere, const Ray3D& ray, OUT float& distance);
	// Ray to AABB
	static bool Raycast(const AABB3D& aabb, const Ray3D& ray, OUT float& distance);
	// Ray to OBB
	static bool Raycast(const OBB3D& obb, const Ray3D& ray, OUT float& distance);
	// Ray to Plane
	static bool Raycast(const Plane3D& plane, const Ray3D& ray, OUT float& distance);


	// **********
	// Triangle
	// *********
	
	// Point In Triangle
	// Point가 Triangle안에 있는 것을 판단함
	// - Raycast를 통해 충돌 판정에 사용함
	// - NaviMesh를 통해 해당 지점이 갈수 있는 지점(Triangle)안인지 확인함
	static bool PointInTriangle(const Point3D& p, const Triangle3D& t);

	static bool Raycast(const Triangle3D& triangle, const Ray3D& ray, float& distance);
	// 다른 벡터에 투영해서 다른 벡터에서 선분을 구하기
	// @params from : 투영시킬 벡터
	// @params to : 기준이 될 벡터
	static Vec3 ProjectVecOnVec(Vec3 from, Vec3 to);
	// 삼각형을 포함한 Plane3D를 생성
	// @params triangle : 해당 삼각형을 포함한 Plane을 생성함
	static Plane3D FromTriangle(const Triangle3D& triangle);

	// Barycentric좌표를 사용하여 Tirangle내에 Point3D가 특정 비율 위치 계산
	static Vec3 Barycentric(const Point3D& p, const Triangle3D& t);
};

struct Interval3D
{
	float min;
	float max;
};
