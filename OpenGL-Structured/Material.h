#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>

class Material
{
public:
	Material();
	void initShaders(std::string vs_shader, std::string frag_shader);
	//void initShaders(std::string vs_shader, std::string geom_shader, std::string frag_shader);
	~Material();

	GLuint glprogramIndex;
	GLint modelingMatrixLoc;
	GLint viewingMatrixLoc;
	GLint projectionMatrixLoc;
	GLint eyePosLoc;
	GLint iResloc;
	GLint iTimeloc;
private:
	
};

