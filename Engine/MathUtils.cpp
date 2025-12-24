#include "pch.h"
#include "MathUtils.h"


// ********************
// Point3D(3차원 좌표)
// ********************
bool MathUtils::PointInSphere(const Point3D& point, const Sphere3D& sphere)
{
	// sphere에서 point로 향하는 방향 벡터
	float magSq = (point - sphere.position).LengthSquared();
	// magSq가 거리의 제곱이니까 반경도 제곱으로 만듦
	float radSq = (sphere.radius * sphere.radius);
	
	// sphrer에서 Point까지의 제곱, Sphere의 반경 제곱의 비교
	return magSq <= radSq;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Sphere3D& sphere)
{
	// sphere -> point 단위벡터
	Vec3 sphereToPointDir = (point - sphere.position);
	sphereToPointDir.Normalize();

	// Sphere의 표면 위에 있는 point와 가장 가까운 지점
	return sphere.position + sphereToPointDir * sphere.radius;
}

bool MathUtils::PointInAABB(const Point3D& point, const AABB3D& aabb)
{
	// AABB의 가장 수치가 낮은 좌표
	Point3D min = AABB3D::GetMin(aabb);
	// AABB의 가장 수치가 높은 좌표
	Point3D max = AABB3D::GetMax(aabb);

	// Point3D의 x,y,z좌표중 하나가 이상이 min보다 더 낮음 = 밖에 있음.
	if (point.x < min.x || point.y < min.y || point.z < min.z)
		return false;

	// Point3D의 x,y,z좌표중 하나가 이상이 max보다 더 큼 = 밖에 있음.
	if (point.x > max.x || point.y > max.y || point.z > max.z)
		return false;

	return true;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const AABB3D& aabb)
{
	Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

	// x, y, z가 minPt보다 낮으면 minPt값으로 조정
	result.x = max(result.x, minPt.x);
	result.y = max(result.y, minPt.y);
	result.z = max(result.z, minPt.z);

	// x, y, z가 maxPt보다 크면 maxPt값으로 조정
	result.x = min(result.x, maxPt.x);
	result.y = min(result.y, maxPt.y);
	result.z = min(result.z, maxPt.z);

	return result;
}

bool MathUtils::PointInOBB(const Point3D& point, const OBB3D& obb)
{
	// obb.position -> point 방향 벡터(길이)
	Vec3 dir = point - obb.position;

	// Obb의 Local 축
	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());	// obb의 Local x축
	axis.push_back(obb.orientation.Up());		// obb의 Local y축
	axis.push_back(obb.orientation.Backward()); // obb의 Local z축

	// obb의 크기
	vector<float> size;
	size.push_back(obb.size.x);	
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int32 i = 0; i < 3; i++)
	{
		// obb.position -> point 방향 벡터를 axis[i]한테 내적을 통해 투영함
		// - obb.position에서 obb.position -> point 방향 벡터에서 axis[i]한테 수선의 발을 내린 위치까지의 거리
		float distance = dir.Dot(axis[i]);

		// Local 축에 투영한 길이가 size[i] 보다 큼 = OBB밖에 있음
		if (distance > size[i])
			return false;
		// Local 축에 투영한 길이가 -size[i] 보다 작음 = OBB밖에 있음
		if (distance < -size[i])
			return false;
	}

	return true;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const OBB3D& obb)
{
	Point3D result = obb.position;
	// obb.position -> point 방향 벡터(길이)
	Vec3 dir = point - obb.position;

	// Obb의 Local 축
	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());	// obb의 Local x축
	axis.push_back(obb.orientation.Up());		// obb의 Local y축
	axis.push_back(obb.orientation.Backward()); // obb의 Local z축

	// obb의 크기
	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int i = 0; i < 3; i++)
	{
		// obb.position -> point 방향 벡터를 axis[i]한테 내적을 통해 투영함
		// - obb.position에서 obb.position -> point 방향 벡터에서 axis[i]한테 수선의 발을 내린 위치까지의 거리
		float distance = dir.Dot(axis[i]);

		// Local 축에 투영한 길이가 size[i] 보다 큼 = OBB밖에 있음
		// - distance = size[i]하고 dir방향으로 표면거리를 구함
		if (distance > size[i])
			distance = size[i];
		// Local 축에 투영한 길이가 -size[i] 보다 작음 = OBB밖에 있음
		// - distance = size[i]하고 dir 방향으로 표면거리르 구함
		if (distance < -size[i])
			distance = -size[i];

		// obb.position(혹은 축을 따라 이동된 위치)에서 axis[i]축 방향으로 distance만큼의 위치
		result = result + (axis[i] * distance);
	}

	return result;
}

bool MathUtils::PointOnPlane(const Point3D& point, const Plane3D& plane)
{
	// 원점 -> Point 방향 벡터를 plane.normal에 내적을 통해 투영함
	// - 원점에서 Plane까지 거리(최단)
	float dot = point.Dot(plane.normal);
	
	// plane.distance = 원점에서 Plane까지의 최단 거리
	// - 원점 -> Point 방향 벡터를 plane.normal에 내적을 통해 투영한거리와 원점에서 Plane까지의 최단 거리 비교
	return ::abs(dot - plane.distance) <= 0.0001f;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Plane3D& plane)
{
	// 원점 -> Point 방향 벡터를 plane.normal에 내적을 통해 투영함
	float dot = point.Dot(plane.normal);
	// 투영된 거리 - 원점에서 Plane까지의 거리 = Normal에 투영된 Point의 Plane에서 Point까지의 거리
	float distance = dot - plane.distance;
	// Point에서 Normal에 투영된 Point와 Plane까지의 거리를 뺌 = Point에서 Plane까지 수선의 발을 내린 지점
	return point - (plane.normal * distance);
}

bool MathUtils::PointOnLine(const Point3D& point, const Line3D& line)
{
	// Line 중 Point와 가장 가까운 좌표 구하기
	Point3D closest = ClosestLine(point, line);

	// cloeset와 point의 거리 구하기
	float distanceSq = (closest - point).LengthSquared();

	// cloeset와 point의 거리가 0에 수렴하면 Line에 있는것
	return abs(distanceSq) <= 0.0001f;
}

Point3D MathUtils::ClosestLine(const Point3D& point, const Line3D& line)
{
	// line의 Vector(start -> end)
	Vec3 IVec = line.end - line.start;

	// IVec.Dot(IVec) = IVec * IVec * cos 0 = IVec * IVec * 1 = IVec^2
	// (point - line.start).Dot(IVec) = (line.start -> Point) * IVec * cos 세타 
	// - = (line.start -> point) * cos 세타 = (line.start -> point)를 IVec에 투영한 선분
	// - (line.start -> point)를 IVec에 투영한 선분 * |IVec|
	// 
	// ((line.start -> point)를 IVec에 투영한 선분 * |IVec|) / (IVec * IVec)
	// = (line.start -> point)를 IVec에 투영한 선분 / IVec
	// = IVec가 1일때 투영된 선분의 길이에 대한 비율
	// ex) 투영된 선분의 값이 8, IVec가 16일때 0.5 = Point에서 Line으로 수선의 발을 내린 지점이 Line안에 있음
	// ex) 투영된 선분의 값이 -2 IVec가 8일때 -0.25 = Point에서 Line으로 수선의 발을 내린 지점이 Origin보다 뒤에 있음
	float t = (point - line.start).Dot(IVec) / IVec.Dot(IVec);

	// 수선의 발을 내린 지점이 line 밖에 있으면 조정해줌
	t = fmaxf(t, 0.0f);	// t가 0보다 작으면 0으로 설정. Clamp to 0
	t = fminf(t, 1.0f); // t가 1보다 크면 1으로 설정. Clamp to 1

	return line.start + IVec * t;
}

bool MathUtils::PointOnLine(const Point3D& point, const Ray3D& ray)
{
	// point가 origin과 동일
	if (point == ray.origin)
		return true;

	// (ray.origin -> point)의 방향 단위 벡터
	Vec3 norm = point - ray.origin;
	norm.Normalize();

	// Ray의 진행 방향 Normalize
	Vec3 dir = ray.direction;
	dir.Normalize();

	// (ray.origin -> point)의 방향 단위 벡터와 direction의 내적
	// |norm| * |dir| * cos 세타 = cos 세타
	// cos 0 = 1 -> 동일한 방향 = 같은 위치에 있음
	float diff = norm.Dot(dir);
	return diff == 1.f;
}

Point3D MathUtils::ClosestLine(const Point3D& point, const Ray3D& ray)
{
	// (ray.origin -> point)을 ray.direction에 투영함
	float t = (point - ray.origin).Dot(ray.direction);
	// direction 반대 방향으로 수선의 발을 내리면 음수임
	// 음수이면 0으로 clamp
	t = ::fmaxf(t, 0.f);

	return Point3D(ray.origin + ray.direction * t);
}


// *****************************
// Intersection(충돌 & 교차될때)
// *****************************
bool MathUtils::SphereToSphere(const Sphere3D& s1, const Sphere3D& s2)
{
	// 두 Sphere가 접촉했을 때의 거리(두 sphere의 반경)
	float sum = s1.radius + s2.radius;
	// 두 Sphere의 중심점의 거리의 제곱
	float sqDistance = (s1.position - s2.position).LengthSquared();

	// 두 sphere의 중심저의 거리가 더 작으면 두 Sphere는 충돌했거나 겹쳐져 있음
	return sqDistance <= sum * sum;
}

bool MathUtils::SphereToAABB(const Sphere3D& sphere, const AABB3D& aabb)
{
	// Sphere의 중심점과 AABB 표면에서 가장 가까운 지점
	Point3D closestPoint = ClosestPoint(sphere.position, aabb);

	// Sphere의 중심점에서 closestPoint의 거리의 제곱
	float distSq = (sphere.position - closestPoint).LengthSquared();
	// Sphere의 반경 제곱
	float radiusSq = sphere.radius * sphere.radius;

	// 중심점과의 AABB의 ClosestPoint거리가 Sphere의 반경보다 작으면 True
	return distSq < radiusSq;
}

bool MathUtils::SphereToOBB(const Sphere3D& sphere, const OBB3D& obb)
{
	// Sphere의 중심점과 OBB 표면에서 가장 가까운 지점
	Point3D closestPoint = ClosestPoint(sphere.position, obb);

	// Sphere의 중심점에서 closestPoint의 거리의 제곱
	float distSq = (sphere.position - closestPoint).LengthSquared();
	// Sphere의 반경 제곱
	float radiusSq = sphere.radius * sphere.radius;

	// 중심점과의 OBB의 ClosestPoint거리가 Sphere의 반경보다 작으면 True
	return distSq < radiusSq;
}

bool MathUtils::SphereToPlane(const Sphere3D& sphere, const Plane3D& plane)
{
	// Sphere의 중심점과 Plane 표면에서 가장 가까운 지점
	Point3D closestPoint = ClosestPoint(sphere.position, plane);

	// Sphere의 중심점에서 closestPoint의 거리의 제곱
	float distSq = (sphere.position - closestPoint).LengthSquared();
	// Sphere의 반경 제곱
	float radiusSq = sphere.radius * sphere.radius;

	// 중심점과의 Plane의 ClosestPoint거리가 Sphere의 반경보다 작으면 True
	return distSq < radiusSq;
}

bool MathUtils::AABBToAABB(const AABB3D& aabb1, const AABB3D& aabb2)
{
	Point3D aMin = AABB3D::GetMin(aabb1);
	Point3D aMax = AABB3D::GetMax(aabb1);
	Point3D bMin = AABB3D::GetMin(aabb2);
	Point3D bMax = AABB3D::GetMax(aabb2);

	// x축만 보았을 때 
	// a최소값이 b최대값보다 작으면서 a최대값이 b최소값보다 큼  = x값은 겹침
	// a최소값이 b최대값보다 큼. = x값이 전체적으로 a가 더 큼 = 안겹침
	// a최대값이 b최소값보다 작음 = x값이 전체적으로 b가 더 큼 = 안겹침
	bool AxisX = aMin.x <= bMax.x && aMax.x >= bMin.x;
	bool AxisY = aMin.y <= bMax.y && aMax.y >= bMin.y;
	bool AxisZ = aMin.z <= bMax.z && aMax.z >= bMin.z;

	return AxisX && AxisY && AxisZ;
}

Interval3D MathUtils::GetInterval(const AABB3D& aabb, const Vec3& axis)
{
	// aabb의 가장 작은 좌표
	Vec3 i = AABB3D::GetMin(aabb);
	// aabb의 가장 큰 좌표
	Vec3 a = AABB3D::GetMax(aabb);

	// AABB의 정점 좌표
	Vec3 vertex[8] =
	{
		Vec3(i.x, a.y, a.z),
		Vec3(i.x, a.y, i.z),
		Vec3(i.x, i.y, a.z),
		Vec3(i.x, i.y, i.z),
		Vec3(a.x, a.y, a.z),
		Vec3(a.x, a.y, i.z),
		Vec3(a.x, i.y, a.z),
		Vec3(a.x, i.y, i.z),
	};

	// 최소, 최대 구하기
	Interval3D result;
	// vertex[0]을 axis에 투영
	result.min = result.max = axis.Dot(vertex[0]);

	for (int i = 1; i < 8; ++i)
	{
		// 각 정점을 axis축에 내적을 함
		float projection = axis.Dot(vertex[i]);
		// 내적된 값이 가장 작은/큰지 확인함
		result.min = min(result.min, projection);
		result.max = max(result.max, projection);
	}

	return result;
}

Interval3D MathUtils::GetInterval(const OBB3D& obb, const Vec3& axis)
{
	Vec3 vertex[8];

	Vec3 C = obb.position; // OBB의 중심점
	Vec3 E = obb.size; // OBB 크기(Extents)

	vector<Vec3> A; // OBB 축
	A.push_back(obb.orientation.Right());
	A.push_back(obb.orientation.Up());
	A.push_back(obb.orientation.Backward());

	// 각 정점의 좌표
	vertex[0] = C + (A[0] * E.x) + (A[1] * E.y) + (A[2] * E.z);
	vertex[1] = C - (A[0] * E.x) + (A[1] * E.y) + (A[2] * E.z);
	vertex[2] = C + (A[0] * E.x) - (A[1] * E.y) + (A[2] * E.z);
	vertex[3] = C + (A[0] * E.x) + (A[1] * E.y) - (A[2] * E.z);
	vertex[4] = C - (A[0] * E.x) - (A[1] * E.y) - (A[2] * E.z);
	vertex[5] = C + (A[0] * E.x) - (A[1] * E.y) - (A[2] * E.z);
	vertex[6] = C - (A[0] * E.x) + (A[1] * E.y) - (A[2] * E.z);
	vertex[7] = C - (A[0] * E.x) - (A[1] * E.y) + (A[2] * E.z);

	// 최소 / 최대 구하기
	Interval3D result;
	// 축에 대해서 0번정점을 투영
	result.min = result.max = axis.Dot(vertex[0]);

	for (int i = 1; i < 8; i++)
	{
		// i번 정점을 axis축에 대해 투영
		float projection = axis.Dot(vertex[i]);
		// 투영된 좌표가 가장 큰지/작은지 검사
		result.min = min(result.min, projection);
		result.max = max(result.max, projection);
	}

	return result;
}

bool MathUtils::OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis)
{
	Interval3D a = GetInterval(aabb, axis);
	Interval3D b = GetInterval(obb, axis);

	// aabb와 obb가 서로 겹쳐 있는지 확인
	return (b.min <= a.max) && (a.min <= b.max);
}

bool MathUtils::OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis)
{
	Interval3D a = GetInterval(obb1, axis);
	Interval3D b = GetInterval(obb2, axis);

	// aabb와 obb가 서로 겹쳐 있는지 확인
	return (b.min <= a.max) && (a.min <= b.max);
}

bool MathUtils::AABBToOBB(const AABB3D& aabb, const OBB3D& obb)
{
	Vec3 test[15] =
	{
		// AABB와 OBB의 기본 축
		Vec3(1, 0, 0),				// AABB axis 1
		Vec3(0, 1, 0),				// AABB axis 2
		Vec3(0, 0, 1),				// AABB axis 3

		obb.orientation.Right(),	// OBB axis 1
		obb.orientation.Up(),		// OBB axis 2
		obb.orientation.Backward()	// OBB axis 3
	};

	// 추가적으로 9개의 축을 구함
	for (int i = 0; i < 3; i++)
	{
		// i축과 0번축의 외적(수직)
		test[6 + i * 3 + 0] = test[i].Cross(test[0]);
		// i축과 1번축의 외적(수직)
		test[6 + i * 3 + 1] = test[i].Cross(test[1]);
		// i축과 2번축의 외적(수직)
		test[6 + i * 3 + 2] = test[i].Cross(test[2]);
	}

	// 15개의 축에 투영된 범위가 겹쳐야 됨.
	for (int i = 0; i < 15; i++)
	{
		if (OverlapOnAxis(aabb, obb, test[i]) == false)
			return false;
	}

	return true;
}

bool MathUtils::AABBToPlane(const AABB3D& aabb, const Plane3D& plane)
{
	float pLen = aabb.size.x * fabsf(plane.normal.x) +
		aabb.size.y * fabsf(plane.normal.y) +
		aabb.size.z * fabsf(plane.normal.z);

	// aabb의 중심점을 normal벡터에 투영 = 원점 -> 중심점이 투영된 위치의 거리
	float dot = plane.normal.Dot(aabb.position);
	// 원점 -> 중심점이 투영된 위치의 거리 - 원점 -> Plane의 거리 = Plane와 AABB의 Position의 거리
	float dist = dot - plane.distance;

	return fabsf(dist) <= pLen;
}

bool MathUtils::OBBToOBB(const OBB3D& obb1, const OBB3D& obb2)
{
	Vec3 test[15] =
	{
		// OBB1와 OBB2의 기본 축
		obb1.orientation.Right(),	// OBB2 axis 1
		obb1.orientation.Up(),		// OBB2 axis 2
		obb1.orientation.Backward(),// OBB2 axis 3

		obb2.orientation.Right(),	// OBB2 axis 1
		obb2.orientation.Up(),		// OBB2 axis 2
		obb2.orientation.Backward()	// OBB2 axis 3
	};

	// 추가적으로 9개의 축을 구함
	for (int i = 0; i < 3; i++)
	{
		// i축과 0번축의 외적(수직)
		test[6 + i * 3 + 0] = test[i].Cross(test[0]);
		// i축과 1번축의 외적(수직)
		test[6 + i * 3 + 1] = test[i].Cross(test[1]);
		// i축과 2번축의 외적(수직)
		test[6 + i * 3 + 2] = test[i].Cross(test[2]);
	}

	// 15개의 축에 투영된 범위가 겹쳐야 됨.
	for (int i = 0; i < 15; i++)
	{
		if (OverlapOnAxis(obb1, obb2, test[i]) == false)
			return false;
	}

	return true;
}

bool MathUtils::PlaneToPlane(const Plane3D& plane1, const Plane3D& plane2)
{
	// Plane1의 Normal과 Plane2의 Normal의 외적 = 두 벡터가 평행이면 (0, 0, 0) 반환함
	Vec3 d = plane1.normal.Cross(plane2.normal);

	// d.Dot(d) = |d| * |d| = |d|^2 = 평행이면 0^2 = 0
	return d.Dot(d) != 0;
}

bool MathUtils::Raycast(const Sphere3D& sphere, const Ray3D& ray, OUT float& distance)
{
	// ray.origin -> sphere.position의 방향 벡터
	Vec3 e = sphere.position - ray.origin;

	// Sphere의 반경 제곱
	float rSq = sphere.radius * sphere.radius;
	// ray의 Origin에서 sphere의 Position까지의 거리 제곱
	float eSq = e.LengthSquared();

	//  ray.origin -> sphere.position의 방향 벡터를 ray진행 방향 벡터에 투영함.
	float a = e.Dot(ray.direction);

	// ray의 Origin에서 sphere의 Position까지의 거리 제곱 -  ray 진행 방향 벡터에 투영된 ray.origin -> sphere.position의 제곱
	// 피타고라스의 a^2 + b^2 = c^2을 이용한 공식
	//  - eSq = 빗변의 제곱
	//  - a^2 = 밑변의 제곱
	//  - a^2 + b^2 = eSq  -> b^2 = eSq - a^2
	// sphere.position에서 Ray.Direction을 향해 수선의 발을 내린 선분의 길이의 제곱
	float bSq = eSq - (a * a);

	// 피타고라스의 a^2 + b^2 = c^2을 이용한 공식
	// rSq(빗변) - bSq(높이) = f^2(밑변)
	// sqrt() : 제곱근 구하기 
	// f = Ray.direction방향으로 Sphere에 닿는 지점 -> sphere.position에서 Ray.Direction을 향해 수선의 발을 내린 지점까지의 선분
	// Ray가 Sphere에 닿아야 양수임
	float f = sqrt(rSq - bSq);

	// eSq - (a * a) = bSq
	// rSq - bSq = f
	// f가 음수이다 -> Ray에 Sphere가 닿지 않음
	if (rSq - (eSq - (a * a)) < 0.f)
		return false;

	// eSq < rSq = Ray의 시작점이 Sphere 안에 있음
	if (eSq < rSq)
	{
		distance = a + f;
		return true;
	}

	// a - f = Ray.origin -> Ray.direction방향으로 Sphere닿는 지점의 거리
	distance = a - f;
	return true;
}

bool MathUtils::Raycast(const AABB3D& aabb, const Ray3D& ray, OUT float& distance)
{
	// Cyrus-Beck clipping 알고리즘
	// AABB를 구성하는 6개의 평면에 대해 클리핑 진행
	// Point3D point = ray.origin + ray.direction * t(거리);

	Vec3 min = AABB3D::GetMin(aabb);
	Vec3 max = AABB3D::GetMax(aabb);

	// (ray.origin -> min/max) / ray.direcion 
	// = ray.origin에서 ray.direction 방향으로 진행하여 aabb의 min/max값 도착할때 ray.origin에서 도착지점까지의 거리
	// ex) (min.x - ray.origin.x) / ray.direction.x -> (3 - 0) / 0.5 -> 6
	//		- ray.origin에서 min.x까지 ray.direction.x방향으로 갈려면 6만큼 가야함
	float t1 = (min.x - ray.origin.x) / ray.direction.x;
	float t2 = (max.x - ray.origin.x) / ray.direction.x;

	float t3 = (min.y - ray.origin.y) / ray.direction.y;
	float t4 = (max.y - ray.origin.y) / ray.direction.y;
													  
	float t5 = (min.z - ray.origin.z) / ray.direction.y;
	float t6 = (max.z - ray.origin.z) / ray.direction.y;

	// 각 축(x, y, z)중  Ray가 AABB의 Slab에 들어가는 값 중 가장 큰 값
	//  - 각 축 슬랩에 대한 진입 t 값들 중 가장 큰 값
	float tmin = fmaxf(
		fmaxf(
			fminf(t1, t2), // x축 Slab 진입하는 값 = tminX
			fminf(t3, t4) // y축 Slab 진입하는 값 = tminY
		),
		fminf(t5, t6) // z축 Slab 진입하는 값 = tminZ
	);

	// 각 축(x, y, z)중  Ray가 AABB의 Slab에 나가는 값 중 가장 작은 값
	//  - 각 축 슬랩에 대한 이탈 t 값들 중 가장 작은 값
	float tmax = fminf(
		fminf(
			fmaxf(t1, t2), // x축 Slab 이탈하는 값 = tmaxX
			fmaxf(t3, t4) // y축 Slab 이탈하는 값 = tmaxY
		),
		fmaxf(t5, t6) // z축 Slab 이탈하는 값 = tmaxZ
	);

	if (tmax < 0)
		return false;

	// 마지막 진입 > 처음 이탈 = Ray가 AABB에 닿지 않음
	if (tmin > tmax)
		return false;

	// Ray가 AABB안에 시작함
	if (tmin < 0.0f)
	{
		distance = tmax;
		return true;
	}

	// 처음 닿는 지점이 Ray -> AABB의 거리가 됨
	distance = tmin;
	return true;
}

bool MathUtils::Raycast(const OBB3D& obb, const Ray3D& ray, OUT float& distance)
{
	float tMin = 0.0f;          // Ray가 OBB에 진입하는 가장 먼 거리
	float tMax = FLT_MAX;       // Ray가 OBB에서 나가는 가장 가까운 거리

	// ray.origin->obb.position 벡터
	Vec3 p = obb.position - ray.origin;

	// OBB의 각 로컬 축(X, Y, Z)에 대해 슬랩 테스트 진행
	// i=0: Right(X), i=1: Up(Y), i=2: Forward(Z)
	for (int i = 0; i < 3; ++i)
	{
		Vec3 axis;
		float halfSize;

		if (i == 0) { axis = obb.orientation.Right();   halfSize = obb.size.x; }
		else if (i == 1) { axis = obb.orientation.Up();      halfSize = obb.size.y; }
		else { axis = obb.orientation.Backward();  halfSize = obb.size.z; } 

		// p를 axis축에 투영함
		// 로컬 축 방향으로의 거리 차이
		float e = axis.Dot(p);             
		// 로컬 축 방향으로의 Ray 속도(성분), t = 1일 때 이동하는 거리
		float f = axis.Dot(ray.direction); 

		// Ray가 해당 축(면)에 평행하지 않은 경우
		//  - cos 세타 = 0 -> 서로 평행함
		if (fabs(f) > 0.0001f)
		{
			float t1 = (e + halfSize) / f;		// axis축 앞쪽 면까지의 거리
			float t2 = (e - halfSize) / f;		// axis축 뒤쪽 면까지의 거리

			if (t1 > t2) std::swap(t1, t2);		// t1을 진입점, t2를 이탈점으로 맞춤

			if (t1 > tMin) tMin = t1;			// 진입점 중 가장 큰 값
			if (t2 < tMax) tMax = t2;			// 이탈 점중 가장 작은 값

			// 진입점이 이탈점보다 멀음
			if (tMin > tMax) return false;
		}
		else
		{
			// Ray가 축과 평행한데 박스 범위 밖에서 시작하면 충돌 불가
			if (-e - halfSize > 0 || -e + halfSize < 0) return false;
		}
	}

	// OBB가 Ray 뒤에 있음
	if (tMax < 0.f)
		return false;

	// Ray가 박스 내부에서 시작됨
	if (tMin < 0.0f) 
	{
		distance = tMax;
		return true;
	}

	// 처음 닿는 거리
	distance = tMin;
	return true;
}

bool MathUtils::Raycast(const Plane3D& plane, const Ray3D& ray, OUT float& distance)
{
	// direction이 Plane을 향하는지 확인
	// nd이 음수 = Normal과 Direction이 역방향임 -> Direction이 Plane을 향함
	// |nd|가 클수록 Plane과 수직으로 만남.
	float nd = ray.direction.Dot(plane.normal);	
	// origin이 Plane 앞/뒤에 있는지 확인가능
	// 값이 클수록 Plane과 멀리 떨어져 있음. origin이 Normal에 수선의 발을 내린 선분
	float pn = ray.origin.Dot(plane.normal);		

	// 양수 = Direction이 Plane을 향하지 않음
	if (nd >= 0.f)
		return false;

	// t = ray에서 Plane까지의 거리
	float t = (plane.distance - pn) / nd;

	if (t >= 0.f)
	{
		distance = t;
		return true;
	}

	return false;
}

bool MathUtils::PointInTriangle(const Point3D& p, const Triangle3D& t)
{
	Vec3 a = t.a - p; 	// Triangle의 정점 a -> p 벡터
	Vec3 b = t.b - p; 	// Triangle의 정점 b -> p 벡터
	Vec3 c = t.c - p; 	// Triangle의 정점 c -> p 벡터

	// 정점과 Triangle 안의 Point 연결된 선분을 동일한 방향으로 회전하면서 외적
	//   [c]
	// [a]  [b]
	// Triangle 안에 있으면 동일한 외적의 결과를 반환함.
	// Triangle 밖에 있으면 다른 결과가 반환됨.
	Vec3 normalPBC = b.Cross(c); // b와 c의 외적
	Vec3 normalPCA = c.Cross(a); // b와 a의 외적
	Vec3 normalPAB = a.Cross(b); // a와 b의 외적

	// Triangle 안에 있으면 두 외적의 사이각은 0이기 떄문에 1이 나옴
	if (normalPBC.Dot(normalPCA) < 0.0f)
		return false;

	// Triangle 안에 있으면 두 외적의 사이각은 0이기 떄문에 1이 나옴
	else if (normalPBC.Dot(normalPAB) < 0.f)
		return false;

	return true;
}

bool MathUtils::Raycast(const Triangle3D& triangle, const Ray3D& ray, float& distance)
{
	// 삼각형을 포함한 평면 생성
	Plane3D plane = FromTriangle(triangle);

	float t = 0;
	// Plane에 대해서 RayCast시도
	if (Raycast(plane, ray, OUT t) == false)
		return false;

	// Raycast에 감지된 Plane의 지점
	Point3D result = ray.origin + ray.direction * t;

	Vec3 barycentric = Barycentric(result, triangle);

	// 
	if (barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
		barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
		barycentric.z >= 0.0f && barycentric.z <= 1.0f)
	{
		distance = t;
		return true;
	}
}

Vec3 MathUtils::ProjectVecOnVec(Vec3 from, Vec3 to)
{
	to.Normalize();
	float dist = from.Dot(to);

	return to * dist;
}

Plane3D MathUtils::FromTriangle(const Triangle3D& triangle)
{
	Plane3D result;

	// 선분 BA와 선분 CA의 외적 = 삼각과 수직인 법선벡터
	result.normal = (triangle.b - triangle.a).Cross(triangle.c - triangle.a);
	result.normal.Normalize();

	// Normal에 정점을 내적 = Plane을 만드는 방법2
	// 1. 평면에 속해 있는 삼각형(정점 3개)
	// 2. Normal 벡터와 평면에 있는 점하나(Normal + 정점 1개) <----
	// 3. Normal 벡터와 원점에서 평면까지의 거리(Normal + 원점으로부터의 거리)
	result.distance = result.normal.Dot(triangle.a);

	return result;
}

Vec3 MathUtils::Barycentric(const Point3D& p, const Triangle3D& t)
{
	const Vec3& a = t.a;
	const Vec3& b = t.b;
	const Vec3& c = t.c;

	// 1. 직접 계산으로 가중치 (v_weight, w_weight) 구하기
	Vec3 v0 = b - a;
	Vec3 v1 = c - a;
	Vec3 v2 = p - a;

	float d00 = v0.Dot(v0);
	float d01 = v0.Dot(v1);
	float d11 = v1.Dot(v1);
	float d20 = v2.Dot(v0);
	float d21 = v2.Dot(v1);

	float denom = d00 * d11 - d01 * d01;
	if (fabs(denom) < 1e-6f) return Vec3::Zero; // 퇴화된 삼각형

	float v_weight = (d11 * d20 - d01 * d21) / denom;  // f (b에 대한 가중치)
	float w_weight = (d00 * d21 - d01 * d20) / denom;  // g (c에 대한 가중치)

	// 2. SimpleMath::Barycentric 직접 호출해서 위치 벡터 반환!
	return Vec3::Barycentric(a, b, c, v_weight, w_weight);
}

