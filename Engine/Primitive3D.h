#pragma once

// ********************
// Point3D(3차원 좌표)
// ********************
using Point3D = Vec3;

// ********************
// Line3D (3차원 선분)
// ********************
struct Line3D
{
	// 시작점
	Point3D start = Point3D(0.f);
	// 끝점
	Point3D end = Point3D(0.f);

	// Line의 길이(Start에서 End까지)
	float Length() { return Vec3::Distance(start, end); }
	// Line의 길이의 제곱(Length에 제곱근(루트)를 진행하지 않아 성능이 좋음)
	float LengthSq() { return Vec3::DistanceSquared(start, end); }
};


// ********************
// Ray3D (3차원 벡터)
// ********************
struct Ray3D
{
	// 발원지점
	Point3D origin = Point3D(0.f);
	// 발사 방향
	Vec3 direction = Vec3(0.f);

	// 방향을 단위벡터로 만듦
	void NormalizeDirection() { direction.Normalize(); }

	// from에서 to로 향하는 Ray를 반환함
	static Ray3D FromPoints(const Point3D& from, const Point3D& to) { return Ray3D{ from, to - from }; }
};

// ********************
// Sphere3D (구)
// ********************
struct Sphere3D
{
	// 구의 중심점
	Point3D position;
	// 반경
	float radius;
};

// ********************
// AABB (World 축과 평행한 축을 가진 Box)
// ********************
struct AABB3D
{
	// Box의 중심점
	Point3D position = Point3D(0.f);
	// 중심점으로 부터 x, y, z에 크기
	Vec3 size = Vec3(1.f, 1.f, 1.f);

	// AABB의 꼭짓점중 좌표수치가 가장 낮은 좌표 구하기
	static Vec3 GetMin(const AABB3D& aabb)
	{
		// Box의 서로 반대에 있는 꼭짓점 좌표 구함
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;

		// 두 꼭짓점 중 x, y, z가 각각 가장 작은 값을 가진 좌표를 구함
		return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	// AABB의 꼭짓점중 좌표수치가 가장 큰좌표 구하기
	static Vec3 GetMax(const AABB3D& aabb)
	{
		// Box의 서로 반대에 있는 꼭짓점 좌표 구함
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;

		// 두 꼭짓점 중 x, y, z가 각각 가장 큰 값을 가진 좌표를 구함
		return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	// min과 max값을 가지고 AABB3D를 만들어 반환함
	static AABB3D FromMinMax(const Vec3& min, const Vec3& max)
	{
		// (min + max) / 2 = position
		// (max - min) / 2 = size
		return AABB3D((min + max) / 2, (max - min) / 2);
	}
};

// ********************
// OBB (물체의 로컬 축과 평행한 축을 가진 Box)
// ********************
struct OBB3D
{
	Point3D position = Point3D(0.f);
	Vec3 size = Vec3(1.f, 1.f, 1.f);
	// AABB상태에서 orientation만큼 회전시킨 상태 = OBB
	// World 좌표계에서 Local좌표계로 변환하는 행렬
	Matrix orientation;
};

// ********************
// Plane3D(평면)
// ********************
// 평면 구하기
// 1. 평면에 속해 있는 삼각형(정점 3개)
// 2. Normal 벡터와 평면에 있는 점하나(Normal + 정점 1개)
// 3. Normal 벡터와 원점에서 평면까지의 거리(Normal + 원점으로부터의 거리)
struct Plane3D
{
	// 평면에서의 Normal 벡터
	Vec3 normal;
	// 원점으로 부터 평면까지의 거리
	float distance;
};

// ********************
// Triangle3D
// ********************
struct Triangle3D
{
	// 3개의 방식중 하나를 사용 가능함
	union
	{
		struct
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};
		Point3D points[3];
		float values[9];
	};
};