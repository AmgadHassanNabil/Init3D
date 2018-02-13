#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "WICTextureLoader.h"

using namespace std;
using namespace DirectX;

class Font
{
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	const inline ID3D11ShaderResourceView* GetTexture() { return m_Texture; }

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	FontType * m_Font;
	ID3D11ShaderResourceView* m_Texture;
};

