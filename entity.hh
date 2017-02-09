#ifndef ENTITY_HH
#define ENTITY_HH

#include <glm/vec2.hpp>
#include <sprite.hh>

// by default <glm> values are set to 0
class Entity
{
public:
    Entity(Sprite sprite);
    virtual ~Entity() = default;

    // dt in seconds
    virtual void update(float dt) = 0;
    virtual void setSize(const glm::vec2& size);

    void updateSprite(float alpha);
    const glm::vec2& getPosition() const;
    const glm::vec2& getPrevPosition() const;
    const glm::vec2& getSize() const;
    const glm::vec2& getVelocity() const;
    const Sprite& getSpriteC() const;
    void setPosition(const glm::vec2& position);
    void setPrevPosition(const glm::vec2& prevPosition);
    void setVelocity(const glm::vec2& velocity);
    Sprite& getSprite();

protected:
    virtual void move(float dt);

private:
    Sprite sprite;
    glm::vec2 position;
    glm::vec2 prevPosition;
    glm::vec2 size;
    glm::vec2 velocity;
};

inline Entity::Entity(Sprite sprite):
    sprite(sprite)
{}

inline void Entity::setSize(const glm::vec2& size)
{
    this->size = size;
}

inline void Entity::updateSprite(float alpha)
{
    glm::vec2 newPos = alpha * position + (1.f - alpha) * prevPosition;
    sprite.position = newPos;
    sprite.size = size;
}

inline Sprite& Entity::getSprite()
{
    return sprite;
}

inline void Entity::move(float dt)
{
    glm::vec2 temp = position;
    position += velocity * dt;
    prevPosition = temp;
}

inline const glm::vec2& Entity::getPosition() const
{
    return position;
}
inline const glm::vec2& Entity::getPrevPosition() const
{
    return prevPosition;
}
inline const glm::vec2& Entity::getSize() const
{
    return size;
}
inline const glm::vec2& Entity::getVelocity() const
{
    return velocity;
}
inline const Sprite& Entity::getSpriteC() const
{
    return sprite;
}
inline void Entity::setPrevPosition(const glm::vec2& prevPosition)
{
    this->prevPosition = prevPosition;
}
inline void Entity::setPosition(const glm::vec2& position)
{
    this->position = position;
    prevPosition = position;
}
inline void Entity::setVelocity(const glm::vec2& velocity)
{
    this->velocity = velocity;
}

#endif // ENTITY_HH
