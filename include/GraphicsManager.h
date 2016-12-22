#pragma once
#include <string.h>
#include <list>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Const.h"

using namespace Const;

struct BufferResource {
	GLuint vbo;
	GLuint vao;
};

struct Attribute {
	GLint handle;
	std::string name;
	int length;
	int stride;
	int distanceToFirst;
};

struct ShaderResource {
	GLuint handle;
	std::list<Attribute> attributes;
};

struct ArrayTextureResource {
	GLuint handle;
	sf::Image image;
};


struct TextureResource {
	GLuint handle;
	sf::Image image;
	std::string name;
};

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	int newShaderResource(std::string vertShader, std::string fragShader, std::list<Attribute> attributes);
	void bindShader(int id);
	void deleteShader(int id);

	int getUniform(int shaderId, std::string name);

	int getArrayTexture(std::string name);
	int getTexture(std::string name);
	void bindArrayTexture();
	void bindTexture(int id);
	void deleteTexture(int id);

	int newBuffer(int shaderId);
	void updateBuffer(int id, float *mesh, int length);
	void renderBuffer(int id, int length);
	void deleteBuffer(int id);

private:
	std::map < int, BufferResource> bufferResources;
	std::map < int, ShaderResource> shaderResources;
	std::map < int, TextureResource> textureResources;
	std::map < std::string, ArrayTextureResource> arrayTextureResources;

	GLuint textureArray;
	const GLsizei textureWidth = 16;
	const GLsizei textureHeight = 16;
	const GLsizei textureCount = 256;
};

