#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "mesh.h"

class Model {
  public:
    Model(char* path) {
      loadModel(path);
    }

    void draw(Shader &shader);
  private:
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

void Model::draw(Shader &shader) {
  for (uint i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader);
  }
}

void Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (uint i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (uint i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<uint> indices;
  std::vector<Texture> textures;

  for (uint i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.position = glm::vec3(
        mesh->mVertices[i].x,
        mesh->mVertices[i].y,
        mesh->mVertices[i].z
        );
    vertex.normal = glm::vec3(
        mesh->mNormals[i].x,
        mesh->mNormals[i].y,
        mesh->mNormals[i].z
        );

    if (mesh->mTextureCoords[0]) {
        vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
        vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for (uint i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (uint j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

  // stbi_set_flip_vertically_on_load(true);
  auto loadImage = [this](std::string file){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    std::cout << directory << "/" << file << std::endl;
    unsigned char* data = stbi_load((file).c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        if      (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    } else {
        std::cout << "Failed to load image '" << file << "'" << std::endl;
    }

    stbi_image_free(data);
    return texture;
  };

  std::vector<Texture> textures;
  for (uint i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool textureAlreadyLoaded = false;
    for (uint j = 0; j < loadedTextures.size(); j++) {
      if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0){
        textures.push_back(loadedTextures[j]);
        textureAlreadyLoaded = true;
      }
    }
    if (!textureAlreadyLoaded) {
      Texture texture;
      texture.id = loadImage(str.C_Str());
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      loadedTextures.push_back(texture);
    }
  }
  return textures;
}

#endif /* MODEL_H */
