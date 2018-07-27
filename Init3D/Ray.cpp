#include "Ray.h"

#define max(a,b) a < b ? b : a
#define min(a,b) a > b ? a : b

//void Ray::pick(const int &sx, const int &sy, const XMMATRIX & projection, const XMMATRIX & view, const XMMATRIX & world, const int &mClientWidth, const int mClientHeight)
//{
//	XMFLOAT4X4 fProjection;
//	XMStoreFloat4x4(&fProjection, projection);
//	// Compute picking ray in view space.
//	float vx = (+2.0f*sx / mClientWidth - 1.0f) / fProjection._11;
//	float vy = (-2.0f*sy / mClientHeight + 1.0f) / fProjection._22;
//	// Ray definition in view space.
//	orig = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//	dir = XMVectorSet(vx, vy, 1.0f, 0.0f);
//
//	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
//	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);
//	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
//	orig = XMVector3TransformCoord(orig, toLocal);
//	dir = XMVector3TransformNormal(dir, toLocal);
//	// Make the ray direction unit length for the intersection tests.
//	dir = XMVector3Normalize(rayDir);
//}


bool Ray::intersect(const XMFLOAT3 AABBounds[2], float &distance) const
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (AABBounds[sign[0]].x - orig.x) * invdir.x;
	tmax = (AABBounds[1 - sign[0]].x - orig.x) * invdir.x;
	tymin = (AABBounds[sign[1]].y - orig.y) * invdir.y;
	tymax = (AABBounds[1 - sign[1]].y - orig.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (AABBounds[sign[2]].z - orig.z) * invdir.z;
	tzmax = (AABBounds[1 - sign[2]].z - orig.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	distance = tmin;

	if (distance < 0) {
		distance = tmax;
		if (distance < 0) return false;
	}

	return true;
}