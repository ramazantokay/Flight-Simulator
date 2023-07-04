#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp> // GL Math library header
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;
using std::stringstream;
using std::fstream;
using std::cout;
using std::endl;

struct Vertex
{
	Vertex() : x(0), y(0), z(0) {}
	Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }

	Vertex operator+(Vertex& v)
	{
		return Vertex(x + v.x, y + v.y, z + v.z);
	}
	Vertex operator*(float& f)
	{
		return Vertex(x * f, y * f, z * f);
	}
	Vertex operator*(Vertex v)
	{
		return Vertex(x * v.x, y * v.y, z * v.z);
	}

	glm::vec3 vertexToGlmVec3()
	{
		return glm::vec3(x, y, z);
	}
	GLfloat x, y, z;
};

struct Texture
{
	Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
	GLfloat u, v;
};

struct Normal
{
	Normal() : x(0), y(0), z(0) {}
	//Normal(Normal n) : x(n.x), y(n.y), z(n.z){}
	Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
	Normal(glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) { }

	glm::vec3 normalToGlmVec3()
	{
		return glm::vec3(x, y, z);
	}


	Normal& operator+=(Normal& n)
	{
		x += n.x;
		y += n.y;
		z += n.z;
		return *this;
	}

	GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
	GLuint vIndex[3], tIndex[3], nIndex[3];
};


class Mesh
{
public:
	Mesh();
	bool loadObj(const string& fileName);
	bool loadVertices(const float* verts);
public:
	vector<Vertex> gVertices;
	vector<Texture> gTextures;
	vector<Normal> gNormals;
	vector<Face> gFaces;
};