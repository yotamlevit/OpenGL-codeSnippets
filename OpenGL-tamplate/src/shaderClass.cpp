#define _CRT_SECURE_NO_WARNINGS

#include"shaderClass.h"
#include"utils.h"
#include <iostream>

#define SHADER_PATH "./Resources/Shaders/"

char* appandShaderPath(const char* shader_filename) {
	char* shaderPath = new char[strlen(SHADER_PATH)+1];
	strcpy(shaderPath, SHADER_PATH);
	strcat(shaderPath, shader_filename);
	return(shaderPath);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(appandShaderPath(vertexFile));
	std::string fragmentCode = get_file_contents(appandShaderPath(fragmentFile));

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vetex shader Object and get refrence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the vertex shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//Compile the vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}