#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
  public:
    Entity(glm::vec3 position, glm::vec3 direction);

    void setPosition(glm::vec3 position);
    void setPosition(float x, float y, float z);
    void setDirection(glm::vec3 direction);
    void setRotationX(float degrees);
    void setRotationY(float degrees);
    void setRotationZ(float degrees);

    glm::vec3 getPosition();
    glm::vec3 getDirection();
    glm::vec3 getRotation();

  protected:
    glm::vec3 _position, _direction, _rotation;
};

Entity::Entity(glm::vec3 position, glm::vec3 direction)
: _position(position), _direction(direction), _rotation(glm::vec3(0.0f)) {
}

void Entity::setPosition(glm::vec3 position) {
  _position = position;
}

void Entity::setPosition(float x, float y, float z) {
  _position.x = x; 
  _position.y = y; 
  _position.z = z; 
}

void Entity::setDirection(glm::vec3 direction) {
  _direction = glm::normalize(direction);
}

glm::vec3 Entity::getPosition() {
  return _position;
}

glm::vec3 Entity::getDirection() {
  return _direction;
}

void Entity::setRotationX(float degrees) {
  _rotation.x = degrees;
}

void Entity::setRotationY(float degrees) {
  _rotation.y = degrees;
}

void Entity::setRotationZ(float degrees) {
  _rotation.z = degrees;
}

glm::vec3 Entity::getRotation() {
  return _rotation;
}

#endif /* ENTITY_H */
