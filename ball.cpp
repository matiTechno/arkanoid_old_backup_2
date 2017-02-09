#include <ball.hh>
#include <paddle.hh>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Ball::Ball(Sprite sprite):
    Entity(sprite),
    isImmune_(0.f)
{}

void Ball::update(float dt)
{
    if(!isStuck())
        move(dt);

    if(isImmune_ > 0.f)
    {
        isImmune_ -= dt;
        if(isImmune_ <= 0.f)
        {
            getSprite().color.w = 1.f;
        }
    }
}

void Ball::setSize(const glm::vec2& size)
{
    Entity::setSize(size);
    this->radius = size.x/2.f;
}

void Ball::reset()
{
    glm::vec2 newPos(paddle->getPosition().x + paddle->getSize().x/2.f - radius,
                     paddle->getPosition().y - getSize().y);

    setPosition(newPos);
    isStuck_ = true;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, 1);
    int randomInt = uni(rng);

    glm::vec2 vel = getVelocity();
    float lenVel = glm::length(vel);

    float tanPow2 = glm::pow(glm::tan(glm::pi<float>()/4.f), 2.f);
    vel.x = glm::sqrt(glm::pow(lenVel, 2.f)/(tanPow2 + 1.f));

    if(randomInt == 1)
        vel.x = -vel.x;

    vel.y = -glm::sqrt(glm::pow(lenVel, 2.f) - glm::pow(vel.x, 2.f));

    setVelocity(vel);
}
