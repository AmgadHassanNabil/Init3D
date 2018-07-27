struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoords : TEXCOORD)
{
	VS_OUTPUT output;

	output.Pos = inPos;
	output.texCoords = inTexCoords;

	return output;
}
