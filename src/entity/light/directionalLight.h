#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "light.h"

class DirectionalLight : public Light {
  public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient = glm::vec3(1.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f));

    virtual void addToShader(Shader& shader, uint index = 0);
};

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
  : Light(glm::vec3(0.0f), direction, ambient, diffuse, specular) {
}

void DirectionalLight::addToShader(Shader& shader, uint index) {
  shader.use();
  shader.setVec3("dirLight.direction", _direction);
  shader.setVec3("dirLight.ambient",   _ambient);
  shader.setVec3("dirLight.diffuse",   _diffuse);
  shader.setVec3("dirLight.specular",  _specular);
}

#endif /* DIRECTIONALLIGHT_H */
