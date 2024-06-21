#ifndef 3DOBJECT_H
#define 3DOBJECT_H

#include "entity.h"

class 3DObject: public Entity() {
  public:
    3DObject(glm::vec3 position);

    void draw();

  private:
    Mesh _mesh;
}


#endif /* 3DOBJECT_H */
