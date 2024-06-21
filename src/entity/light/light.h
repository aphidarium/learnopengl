#ifndef LIGHT_H
#define LIGHT_H

#include "../../shader.h"
#include "../entity.h"

class Light : public Entity {
  public:
    Light(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient = glm::vec3(1.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f));

    virtual void addToShader(Shader& shader, uint index = 0) = 0;

  protected:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
};

Light::Light(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
  : Entity(position, direction), _ambient(ambient), _diffuse(diffuse), _specular(specular) {
}

#endif /* LIGHT_H */
