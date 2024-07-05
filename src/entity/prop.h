#ifndef PROP_H
#define PROP_H

#include "shader.h"
#include "entity.h"

class Prop : public Entity {
  public:
    Prop(glm::vec3 position, glm::vec3 direction, std::string modelFilepath);

    void draw(Shader shader);

  private:
    Model _model;
};

Prop::Prop(glm::vec3 position, glm::vec3 direction, std::string modelFilepath)
  : Entity(position, direction), _model(Model(modelFilepath)) {
}

void Prop::draw(Shader shader) {
  shader.use();
  glm::mat4 model = glm::mat4(1.0f);
  //model = glm::scale(model, _scale);
  model = glm::translate(model, _position);
  model = glm::rotate(model, glm::radians(360.0f * _rotation.x), glm::vec3(1, 0, 0));
  model = glm::rotate(model, glm::radians(360.0f * _rotation.y), glm::vec3(0, 1, 0));
  model = glm::rotate(model, glm::radians(360.0f * _rotation.z), glm::vec3(0, 0, 1));
  shader.setMat4("model",      model);
  _model.draw(shader);
}

#endif /* PROP_H */
