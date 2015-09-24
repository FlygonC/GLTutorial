#pragma once

#include <gl_core_4_4.h>
#include <string>
#include <fstream>
#include <iostream>

class ShaderLoader
{
public:
	static unsigned int loadShader(unsigned int type, const std::string path);
	static unsigned int createShaderProgram(const std::string vertexShaderPath, const std::string fragmentShaderPath);
	static void freeShaderProgram(unsigned int program);
};