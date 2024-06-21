#ifndef MESH_H
#define MESH_H

#ifndef STB_IMAGE_H
#define STB_IMAGE_H
#include "stb_image.h"
#endif
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  uint id;
  std::string type;
  std::string path;
};

class Mesh {
  public:
    std::vector<Vertex>  vertices;
    std::vector<uint>    indices;
    std::vector<Texture> textures;

    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures);

    void draw(Shader &shader);

  private:
    uint VAO, VBO, EBO;

    void setupMesh();
};

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures) {
  this->vertices = vertices;
  this->indices  = indices;
  this->textures = textures;

  // setup mesh
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
  uint diffuseAmount  = 1;
  uint specularAmount = 1;

  for (uint i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuseAmount++);
    } else if (name == "texture_specular") {
      number = std::to_string(specularAmount++);
    }

    shader.setFloat(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);


  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

uint loadTexture(std::string file) {
  stbi_set_flip_vertically_on_load(true);
  uint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height, nrChannels;
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

#endif /* MESH_H */
