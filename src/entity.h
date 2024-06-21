#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
  public:
    Entity(glm::vec3 position);

    void setPosition(glm::vec3 position);

  private:
    glm::vec3 _position;
}

Entity::Entity(glm::vec3 position) {
  _position = position;
}

#endif /* ENTITY_H */
