#include "Ray.h"

#define max(a,b) a < b ? b : a
#define min(a,b) a > b ? a : b

void Ray::pick(const int &sx, const int &sy, const XMMATRIX & projection, const XMMATRIX & view, const XMMATRIX & world, const int &mClientWidth, const int mClientHeight)
{
	XMFLOAT4X4 fProjection;
	XMStoreFloat4x4(&fProjection, projection);

	float vx = (+2.0f*sx / mClientWidth - 1.0f) / fProjection._11;
	float vy = (-2.0f*sy / mClientHeight + 1.0f) / fProjection._22;

	orig = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	dir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);
	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
	orig = XMVector3TransformCoord(orig, toLocal);
	dir = XMVector3TransformNormal(dir, toLocal);

	dir = XMVector3Normalize(dir);
	update();
}


bool Ray::intersect(const XMFLOAT3 AABBounds[2], float &distance) const
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	XMFLOAT3 origF, invdirF;
	XMStoreFloat3(&origF, orig);
	XMStoreFloat3(&invdirF, invdir);

	tmin = (AABBounds[sign[0]].x - origF.x) * invdirF.x;
	tmax = (AABBounds[1 - sign[0]].x - origF.x) * invdirF.x;
	tymin = (AABBounds[sign[1]].y - origF.y) * invdirF.y;
	tymax = (AABBounds[1 - sign[1]].y - origF.y) * invdirF.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (AABBounds[sign[2]].z - origF.z) * invdirF.z;
	tzmax = (AABBounds[1 - sign[2]].z - origF.z) * invdirF.z;

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