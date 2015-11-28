#include "headers\Shader.h"

Shader::Shader(string vsPath, string fsPath)
{
	GLuint vertexShader = pgr::createShaderFromFile(GL_VERTEX_SHADER, vsPath);
	GLuint fragmentShader = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fsPath);

	GLint status;
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) cerr << "Vertex shader compile error.";
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) cerr << "Fragment shader compile error.";

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
}

Shader::~Shader()
{
}


GLuint Shader::getProgram()
{
	return shaderProgram;
}
