#pragma once

#include <iostream>
#include <fstream>
#include <DirectXMath.h>
using namespace std;
using namespace DirectX;

class Logger
{
	ofstream myfile;

public:
	void inline log(const char* text)
	{
		if (!myfile.is_open())
		{
			myfile.open("log.txt");
			log("new Session");
		}
		myfile << text <<endl;
	}
	void inline log(const char* variableName, const XMVECTOR &v)
	{
		char buffer[100];
		sprintf_s(buffer, "variableName: %s Value: (%.8f, %.8f, %.8f, %.8f)", variableName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
		log(buffer);
	}
	void inline log(const char* variableName, const XMFLOAT2 &v)
	{
		char buffer[100];
		sprintf_s(buffer, "variableName: %s Value: (%.8f, %.8f)", variableName, v.x, v.y);
		log(buffer);
	}
	void inline log(const char* variableName, const XMFLOAT3 &v)
	{
		char buffer[100];
		sprintf_s(buffer, "variableName: %s Value: (%.8f, %.8f, %.8f)", variableName, v.x, v.y, v.z);
		log(buffer);
	}
	void inline log(const char* variableName, const float &f)
	{
		char buffer[100];
		sprintf_s(buffer, "variableName: %s Value: %.8f", variableName, f);
		log(buffer);
	}
	Logger();
	~Logger();
};

