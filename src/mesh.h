#ifndef MESH_H
#define MESH_H

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

    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures);

    void draw(Shader &shader);

  private:
    uint VAO, VBO, EBO;

    void setupMesh();
};

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

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
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

#endif /* MESH_H */
