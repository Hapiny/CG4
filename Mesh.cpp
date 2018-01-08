#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &v, const std::vector<Texture> &t, const std::vector<uint> &i)
	: _vertices(v), _textures(t), _indices(i) 
{
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &vao); GL_CHECK_ERRORS;
	glGenBuffers(1, &vbo); GL_CHECK_ERRORS;
	glGenBuffers(1, &ebo); GL_CHECK_ERRORS;

	glBindVertexArray(vao); GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, vbo); GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); GL_CHECK_ERRORS;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint), &_indices[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;

	glEnableVertexAttribArray(0); GL_CHECK_ERRORS;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); GL_CHECK_ERRORS;

    glEnableVertexAttribArray(1); GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normal)); GL_CHECK_ERRORS;

    glEnableVertexAttribArray(2); GL_CHECK_ERRORS;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _texture)); GL_CHECK_ERRORS;

    glBindVertexArray(0);
}

void Mesh::Draw(const ShaderProgram & shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        GL_CHECK_ERRORS;
         // активируем текстурный блок, до привязки
        // получаем номер текстуры
        std::string number;
        std::string name = _textures[i]._type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++); 
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        glUniform1i(glGetUniformLocation(shader.GetProgram(), (name + number).c_str()), i);
        GL_CHECK_ERRORS;
        glBindTexture(GL_TEXTURE_2D, _textures[i]._id);
        GL_CHECK_ERRORS;
    }

    // отрисовывем полигональную сетку
    glBindVertexArray(vao);
    GL_CHECK_ERRORS;
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    GL_CHECK_ERRORS;
    glBindVertexArray(0);
    GL_CHECK_ERRORS;
 
    glActiveTexture(GL_TEXTURE0);
    GL_CHECK_ERRORS;
}  
