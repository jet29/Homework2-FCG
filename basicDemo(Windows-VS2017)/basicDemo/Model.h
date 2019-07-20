#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

using namespace std;

enum MaterialType {
	blinnPhong,
	orenNayar,
	cookTorrance,
	normalMapping
};


class Model{

private:

	vector < glm::vec3 > out_vertices;
	vector < glm::vec2 > out_uvs;
	vector < glm::vec3 > out_normals;
	glm::vec3 position;
	MaterialType material;
	unsigned int textureID;
	unsigned int normalMapTextureID;
	unsigned int parallaxMapTextureID;
	bool hasTexture;
	int numTriangles;

	// Index (GPU) of the geometry buffer
	unsigned int VBO;
	// Index (GPU) vertex array object
	unsigned int VAO;

	GLuint colorBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;

public:

	Model();

	~Model();


	bool LoadObj(const char * path);

	bool LoadPlane(/*glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4*/);

	bool LoadWithArray1(float * vertices, int size);
	bool LoadWithArray2(float * vertices, int size);
	bool LoadWithArray3(float * vertices, int size);

	void BuildGeometry();

	unsigned int GetVAO();

	unsigned int GetVBO();

	GLuint GetColorBuffer();

	GLuint GetUvBuffer();

	GLuint GetNormalBuffer();

	int GetNumTriangles();

	glm::vec3 getPosition();

	void setPosition(glm::vec3 pos);

	void setMaterial(MaterialType type);

	MaterialType getMaterial();

	void setTextureID(unsigned int _textureID);
	unsigned int getTextureID();

	void setNormalMapTextureID(unsigned int _textureID);
	unsigned int getNormalMapTextureID();

	void setParallaxMapTextureID(unsigned int _textureID);
	unsigned int getParallaxMapTextureID();

};