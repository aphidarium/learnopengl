#ifndef SPRITE_H
#define SPRITE_H

#include "mesh.h"
#ifndef STB_IMAGE_H
#define STB_IMAGE_H
#include "stb_image.h"
#endif

class Sprite {
  public:
    Sprite(std::string texturePath, glm::vec3 color);

    void draw(Shader& shader);

  private:
    Mesh _mesh;

    glm::vec3 _color;
};

Sprite::Sprite(std::string texturePath, glm::vec3 color) {
    Vertex v0;
    v0.position  = glm::vec3(-1.0f, -1.0f, 0.0f);
    v0.texCoords = glm::vec2(0.0f, 0.0f);

    Vertex v1;
    v1.position  = glm::vec3( 1.0f, -1.0f, 0.0f);
    v1.texCoords = glm::vec2( 1.0f,  0.0f);

    Vertex v2;
    v2.position  = glm::vec3( 1.0f,  1.0f, 0.0f);
    v2.texCoords = glm::vec2( 1.0f,  1.0f);

    Vertex v3;
    v3.position  = glm::vec3(-1.0f,  1.0f, 0.0f);
    v3.texCoords = glm::vec2( 0.0f,  1.0f);

    std::vector<Vertex> quad;
    quad.insert(quad.end(), { v0, v1, v2, v3 });

    for (uint i = 0; i < quad.size(); i++)
      quad[i].position *= 0.25;

    std::vector<uint> indices;
    indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });

    Texture texture;
    texture.id = loadTexture(texturePath);
    texture.type = "texture_diffuse";
    texture.path = texturePath;

    std::vector<Texture> textures;
    textures.push_back(texture);

    _mesh = Mesh(quad, indices, textures);
    _color = color;
}

void Sprite::draw(Shader& shader) {
    shader.setVec3("color", _color);
    _mesh.draw(shader);
}

#endif /* SPRITE_H */
