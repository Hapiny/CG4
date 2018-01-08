#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "LiteMath.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
using namespace LiteMath;

unsigned int TextureFromFile(const char *path, const string &directory);

class Model 
{
public:
    vector<Texture> _textures_loaded;
    vector<Mesh> _meshes;
    string _directory;
    
    Model(string const &);
    
    void Draw(const ShaderProgram &);
    
private:
    void loadModel(string const &);
    void processNode(aiNode *, const aiScene *);
    Mesh processMesh(aiMesh *, const aiScene *);
    vector<Texture> loadMaterialTextures(aiMaterial *, aiTextureType, string);
};
#endif
