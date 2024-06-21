#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "pointLight.h"

class SpotLight : public Light {
  public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float innerCone = 5.0f, float outerCone = 35.0f, glm::vec3 ambient = glm::vec3(1.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.05f, float quadratic = 0.032f);

    virtual void addToShader(Shader& shader, uint index = 0);
    void addToShader(Shader& shader, std::string uniformName);

  private:
    float _innerCone, _outerCone;
    float _constant, _linear, _quadratic;
};

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float innerCone, float outerCone, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
  : Light(position, direction, ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic), _innerCone(innerCone), _outerCone(outerCone) {
}

void SpotLight::addToShader(Shader& shader, uint index) {
  std::string strindex = std::to_string(index);
  shader.use();
  shader.setVec3("spotLights[" + strindex + "].position",   _position);
  shader.setVec3("spotLights[" + strindex + "].direction",  _direction);
  shader.setVec3("spotLights[" + strindex + "].ambient",    _ambient);
  shader.setVec3("spotLights[" + strindex + "].diffuse",    _diffuse);
  shader.setVec3("spotLights[" + strindex + "].specular",   _specular);
  shader.setFloat("spotLights[" + strindex + "].constant",  _constant);
  shader.setFloat("spotLights[" + strindex + "].linear",    _linear);
  shader.setFloat("spotLights[" + strindex + "].quadratic", _quadratic);
  shader.setFloat("spotLights[" + strindex + "].innerCone", cos(glm::radians(_innerCone)));
  shader.setFloat("spotLights[" + strindex + "].outerCone", cos(glm::radians(_outerCone)));
}

void SpotLight::addToShader(Shader& shader, std::string uniformName) {
  shader.use();
  shader.setVec3(uniformName + ".position",   _position);
  shader.setVec3(uniformName + ".direction",  _direction);
  shader.setVec3(uniformName + ".ambient",    _ambient);
  shader.setVec3(uniformName + ".diffuse",    _diffuse);
  shader.setVec3(uniformName + ".specular",   _specular);
  shader.setFloat(uniformName + ".constant",  _constant);
  shader.setFloat(uniformName + ".linear",    _linear);
  shader.setFloat(uniformName + ".quadratic", _quadratic);
  shader.setFloat(uniformName + ".innerCone", cos(glm::radians(_innerCone)));
  shader.setFloat(uniformName + ".outerCone", cos(glm::radians(_outerCone)));

}

#endif /* SPOTLIGHT_H */

