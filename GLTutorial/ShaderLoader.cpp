#include "ShaderLoader.h"

unsigned int ShaderLoader::loadShader(unsigned int type, const std::string path)
{
	std::string source;
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile)
	{
		std::cout << "Error: Could not read file!" << std::endl;
		return -1;
	}
	std::string line;
	while (inFile)
	{
		std::getline(inFile, line);
		if (inFile)
		{
			source += line + '\n';
		}
		else
		{
			source += '\0';
		}
	}

	const char* finalSource = source.c_str();
	//std::cout << finalSource;
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&finalSource, 0);
	glCompileShader(shader);

	return shader;
}
unsigned int ShaderLoader::createShaderProgram(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{
	//make shader variables
	int success = GL_FALSE;

	unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderPath);
	unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	//link
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	//make sure it worked
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//cleanup
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return program;
}
void ShaderLoader::freeShaderProgram(unsigned int program)
{
	glDeleteProgram(program);
}