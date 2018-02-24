#include "Effect.h"




void Effect::apply()
{
	AMD3D->d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, pFrameData, 0, 0);
	AMD3D->d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
}

Effect::Effect()
{
}


Effect::~Effect()
{
}
