#pragma once
#include <DirectXMath.h>

using namespace DirectX;


class Ray
{
public:
	Ray() {}
	Ray(const XMFLOAT3 &orig, const XMFLOAT3 &dir) : orig(orig), dir(dir)
	{
		invdir.x = 1 / dir.x;
		invdir.y = 1 / dir.y;
		invdir.z = 1 / dir.z;

		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);
	}
	XMFLOAT3 orig, dir;
	XMFLOAT3 invdir;
	int sign[3];

	void pick(const int &sx, const int &sy, const XMMATRIX & projection, const XMMATRIX & view, const XMMATRIX & world, const int &mClientWidth, const int mClientHeight);
	bool intersect(const XMFLOAT3 AABBounds[2], float &distance) const;
};

