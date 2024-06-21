#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
  public:
    Entity(glm::vec3 position, glm::vec3 direction);

    void setPosition(glm::vec3 position);
    void setDirection(glm::vec3 direction);

  protected:
    glm::vec3 _position, _direction;
};

Entity::Entity(glm::vec3 position, glm::vec3 direction)
: _position(position), _direction(direction) {
}

void Entity::setPosition(glm::vec3 position) {
  _position = position;
}

void Entity::setDirection(glm::vec3 direction) {
  _direction = glm::normalize(direction);
}

#endif /* ENTITY_H */
