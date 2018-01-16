#pragma once

class Model
{
	static bool isInitialized;

	Model();
	Model(const Model&);
	Model& operator=(const Model&);
	~Model();

	void importFBX(const char*);
public:
	Model(const char*);
	void draw();	
};

