cbuffer cbPerObject
{
	float4x4 WVP;
	float billWidth;
	float billHeight;
	float2 pad1;
	float3 camUp;
	float timeSinceInception;
	float4 camPos;
}

struct VS
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
	float size					: SIZE;
};

[maxvertexcount(4)]
void main(point VS input[1], inout TriangleStream<VS> OutputStream)
{
	float halfWidth = billWidth / 2.0f;
	float halfHeight = billHeight / 2.0f;

	float3 planeNormal = normalize(input[0].inceptionPosition - camPos.xyz);
	float3 rightVector = normalize(cross(camUp, planeNormal));
	float3 upVector = cross(planeNormal, rightVector);

	rightVector = rightVector * halfWidth * input[0].size;
	upVector = upVector * halfHeight * input[0].size;

	float3 vert[4];

	vert[0] = input[0].inceptionPosition - rightVector - upVector;
	vert[1] = input[0].inceptionPosition + rightVector - upVector;
	vert[2] = input[0].inceptionPosition - rightVector + upVector;
	vert[3] = input[0].inceptionPosition + rightVector + upVector;

	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	VS outputVert;
	
	outputVert.random = input[0].random;
	for (int i = 0; i < 4; i++)
	{
		outputVert.position = mul(float4(vert[i], 1.0f), WVP) ;
		outputVert.inceptionPosition = float4(vert[i], 0.0f);
		outputVert.texCoord = texCoord[i];
		outputVert.size = input[0].size;

		OutputStream.Append(outputVert);
	}
}