#include "headers\Shader.h"

Shader::Shader(string vsPath, string fsPath)
{
	GLuint vertexShader = pgr::createShaderFromFile(GL_VERTEX_SHADER, vsPath);
	if (vertexShader == 0) cerr << "Error creating vertex shader." << endl;

	GLuint fragmentShader = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fsPath);
	if (fragmentShader == 0) cerr << "Error creating fragment shader." << endl;

	const GLuint shaderArray[] = { vertexShader, fragmentShader, 0 };
	shaderProgram = pgr::createProgram(shaderArray);
	if (shaderProgram == 0) cerr << "Error creating shader program." << endl;
}

Shader::~Shader()
{
}
