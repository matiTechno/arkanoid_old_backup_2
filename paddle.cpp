#include <paddle.hh>
#include <ball.hh>
#include <glm/glm.hpp>
#include <utility.hh>

Paddle::Paddle(Sprite sprite, Ball& ball):
    Entity(sprite),
    ball(ball)
{}

void Paddle::update(float dt)
{
    move(dt);
    if(ball.isStuck())
    {
        glm::vec2 dPos = getPosition() - getPrevPosition();
        glm::vec2 ballPos = ball.getPosition();
        // setPosition affects prevPosition
        ball.setPosition(ballPos + dPos);
        ball.setPrevPosition(ballPos);
    }
}

void Paddle::reflect(Dir pene)
{
    if(ball.getPosition().y + ball.getRadius() < getPosition().y + getSize().y/2.f)
    {
        // P - paddle center
        // B - ball center
        float P = getPosition().x + getSize().x/2.f;
        float B = ball.getPosition().x + ball.getRadius();
        float reflectCoeff = glm::clamp((B - P)/(getSize().x/2.f), -1.f, 1.f);

        glm::vec2 ballVel = ball.getVelocity();
        float ballLenVel = glm::length(ballVel);

        float tanPow2 = glm::pow(glm::tan(minReflectAngle), 2.f);
        ballVel.x = glm::sqrt(glm::pow(ballLenVel, 2.f)/(tanPow2 + 1.f))
                * reflectCoeff;

        ballVel.y = -glm::sqrt(glm::pow(ballLenVel, 2.f) - glm::pow(ballVel.x, 2.f));

        ball.setVelocity(ballVel);
    }
    else
        reflectVel(ball, pene);
}
