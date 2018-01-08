#ifndef MESH_H
#define MESH_H

///////////////////////
// Mesh class based on samples from learnopengl.com
// Класс полигональной сетки
#include "common.h"
#include "LiteMath.h"
#include "ShaderProgram.h"
#include <vector>

using namespace LiteMath;

struct Vertex 
{
	float3 _position;	//позиция вершины
	float3 _normal; 	//вектор вершины
	float2 _texture;	//вектор текстурных координат
};

struct Texture
{
	uint _id;	//номер текстуры
	std::string _type; //тип структуры(диффузная или бликовая)
	std::string _path;
};

class Mesh
{
public:
	std::vector<Vertex> _vertices;		//вектор всех вершин сетки
	std::vector<Texture> _textures;		//вектор всех текстурных координат
	std::vector<unsigned int> _indices;	//вектор индексов вершин
	Mesh(const std::vector<Vertex> &, const std::vector<Texture> &, const std::vector<uint> &);

	void Draw(const ShaderProgram &);
private:
	GLuint vao, vbo, ebo;
	void setupMesh();
};

#endif