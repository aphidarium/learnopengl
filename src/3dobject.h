#ifndef 3DOBJECT_H
#define 3DOBJECT_H

#include "entity.h"

class 3DObject: public Entity() {
  public:
    3DObject(glm::vec3 position, std::string modelFilepath);

    void draw();

  private:
    Model _model;
}

3DObject::3DObject(glm::vec3 position, std::string modelFilepath);
  : _position(position), _model(Model(modelFilepath)) {
}

#endif /* 3DOBJECT_H */
