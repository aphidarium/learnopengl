#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"

class PointLight : public Light {
  public:
    PointLight(glm::vec3 position, glm::vec3 ambient = glm::vec3(1.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.05f, float quadratic = 0.032f);

    virtual void addToShader(Shader& shader, uint index = 0);

  private:
    float _constant, _linear, _quadratic;
};

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
  : Light(position, glm::vec3(0), ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic) {
}

void PointLight::addToShader(Shader& shader, uint index) {
  std::string strindex = std::to_string(index);
  shader.use();
  shader.setVec3("pointLights[" + strindex + "].position",   _position);
  shader.setVec3("pointLights[" + strindex + "].ambient",    _ambient);
  shader.setVec3("pointLights[" + strindex + "].diffuse",    _diffuse);
  shader.setVec3("pointLights[" + strindex + "].specular",   _specular);
  shader.setFloat("pointLights[" + strindex + "].constant",  _constant);
  shader.setFloat("pointLights[" + strindex + "].linear",    _linear);
  shader.setFloat("pointLights[" + strindex + "].quadratic", _quadratic);
}

#endif /* POINTLIGHT_H */
