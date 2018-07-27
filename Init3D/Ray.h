#pragma once
#include <DirectXMath.h>

using namespace DirectX;


class Ray
{
	XMVECTOR one = XMVectorSet(1, 1, 1, 1);
public:
	Ray() {}
	Ray(const XMVECTOR &orig, const XMVECTOR &dir) : orig(orig), dir(dir)
	{
		update();
	}
	XMVECTOR orig, dir;
	XMVECTOR invdir;
	int sign[3];

	void pick(const int &sx, const int &sy, const XMMATRIX & projection, const XMMATRIX & view, const XMMATRIX & world, const int &mClientWidth, const int mClientHeight);
	bool intersect(const XMFLOAT3 AABBounds[2], float &distance) const;

	inline void update()
	{
		invdir = XMVectorDivide(one, dir);

		sign[0] = (XMVectorGetX(invdir) < 0);
		sign[1] = (XMVectorGetY(invdir) < 0);
		sign[2] = (XMVectorGetZ(invdir) < 0);
	}
};

