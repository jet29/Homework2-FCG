#define _CRT_SECURE_NO_WARNINGS
#include "Model.h"


Model::Model() {



}


Model::~Model() {




}

unsigned int Model::GetVAO() {
	return VAO;
}

unsigned int Model::GetVBO() {
	return VBO;
}

GLuint Model::GetColorBuffer() {
	return colorBuffer;
}

GLuint Model::GetUvBuffer() {
	return uvBuffer;
}

GLuint Model::GetNormalBuffer() {
	return normalBuffer;
}

int Model::GetNumTriangles() {
	return numTriangles;
}

/*
bool Model::LoadObj(const char * path){

	fstream file;
	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertices;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	string line;

	file.open(path, std::ios::in);

	if (!file.is_open()) {
		cout << "no se pudo abrir el archivo :(" << endl;
		return false;
	}


	while (1) {

		getline(file, line);
		//cout << line << endl;

		std::istringstream iss(line);
		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());

		for (auto x : words) {
			//cout << "es un x: " << x << endl;
		}

		if (words.size() > 0) {

			if (words[0].compare("v") == 0) {
				//cout << "estoy asignando un vertice" << endl;
				glm::vec3 vertex = glm::vec3(atof(words[1].c_str()), atof(words[2].c_str()), atof(words[3].c_str()));
				temp_vertices.push_back(vertex);
			}
			else if (words[0].compare("vt") == 0) {

				glm::vec2 vertex = glm::vec2(atof(words[1].c_str()), atof(words[2].c_str()));
				temp_uvs.push_back(vertex);
			}
			else if (words[0].compare("vn") == 0) {

				glm::vec3 vertex = glm::vec3(atof(words[1].c_str()), atof(words[2].c_str()), atof(words[3].c_str()));
				temp_normals.push_back(vertex);
			}
			else if (words[0].compare("f") == 0) {

				std::string delimiter = "/";
				size_t pos = 0;
				string face;

				for (int i = 1; i < words.size(); i++) {

					pos = words[i].find(delimiter);

					face = words[i].substr(0, pos);
					words[i].erase(0, pos + 1);

					//cout << "face: " << face <<", word: "<<words[i]<<endl;

					vertexIndices.push_back(atoi(face.c_str()));

					pos = words[i].find(delimiter);

					face = words[i].substr(0, pos);
					words[i].erase(0, pos + 1);

					//cout << "face: " << face << ", word: " << words[i] << endl;

					uvIndices.push_back(atoi(face.c_str()));

					//cout << "face: " << words[i] << endl;

					normalIndices.push_back(atoi(words[i].c_str()));
				}
			}
		}

		words.clear();

		if (file.eof()) {
			break;
		}
	}

	file.close();

	cout << "Finalizando data" << endl;
	cout << vertexIndices.size() << endl;
	cout << uvIndices.size() << endl;
	cout << normalIndices.size() << endl;
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		unsigned int vertexIndex = vertexIndices[i];
		out_vertices.push_back( temp_vertices[vertexIndex - 1] );

		unsigned int uvIndex = uvIndices[i];
		if(uvIndex > 0) if(temp_uvs.size() > 0) out_uvs.push_back(temp_uvs[uvIndex - 1]);

		unsigned int normalIndex = normalIndices[i];
		out_normals.push_back(temp_normals[normalIndex - 1]);
	}
	cout << "data finalizada" << endl;
	if (out_uvs.size() <= 0) hasTexture = false;
	return true;
}
*/


bool Model::LoadObj(const char* path) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file;
	
	int err = fopen_s(&file, path, "r");
	if (err != 0) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
	
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		out_vertices.push_back( temp_vertices[vertexIndex - 1] );
		out_uvs.push_back(temp_uvs[uvIndex - 1]);
		out_normals.push_back(temp_normals[normalIndex - 1]);
	}

	numTriangles = out_vertices.size();

	return true;
}

bool Model::LoadPlane(/*glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4*/){

	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);

	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);


	float quadVertices[] = {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	cout << "buildeando plane" << endl;
	// configure plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

	
	numTriangles = 2;

	return true;
}

bool Model::LoadWithArray1(float * vertices, int size) {

	//just for vertices without textures and normals
	cout << "buildeando object with array 1" << endl;
	// configure plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	numTriangles = (size / 3) / 3;

	return true;
}




bool Model::LoadWithArray2(float * vertices, int size) {

	//The array has to be in the format vertex, uv, vertex, uv....
	cout << "buildeando object with array 2" << endl;

	// configure VAO

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	return true;
}

bool Model::LoadWithArray3(float * vertices, int size) {

	//The array has to be in the format vertex, uv, normal, vertex, uv, normal....
	cout << "buildeando object with array 3" << endl;

	// configure VAO

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
	// link vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	numTriangles = (size / 8) / 3;


	return true;
}





void Model::BuildGeometry(){

	cout << "Buildeando Geometria" << std::endl;

	// Creates on GPU the vertex array
	glGenVertexArrays(1, &VAO);
	// Binds the vertex array to set all the its properties
	glBindVertexArray(VAO);
	
	
	// Creates on GPU the vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);

	// Sets the vertex attributes
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	
	//this code is used when we have a color array
	/*
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);

	// Sets the vertex attributes
	// Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	*/

	// Creates on GPU the vertex buffer object
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);

	// Sets the vertex attributes
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);




	//if (hasTexture) {

		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);


		// Sets the vertex attributes
		// Texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

	//}
	
	glBindVertexArray(0);
}

glm::vec3 Model::getPosition() {
	return position;
}

void Model::setPosition(glm::vec3 pos) {
	position = pos;
}


void Model::setMaterial(MaterialType type) {
	material = type;
}

MaterialType Model::getMaterial() {
	return material;
}

void Model::setTextureID(unsigned int _textureID) {
	textureID = _textureID;
}

unsigned int Model::getTextureID() {
	return textureID;
}

void Model::setNormalMapTextureID(unsigned int _textureID) {
	normalMapTextureID = _textureID;
}

unsigned int Model::getNormalMapTextureID() {
	return normalMapTextureID;
}

void Model::setParallaxMapTextureID(unsigned int _textureID) {
	parallaxMapTextureID = _textureID;
}

unsigned int Model::getParallaxMapTextureID() {
	return parallaxMapTextureID;
}