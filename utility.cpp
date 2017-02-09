#include <utility.hh>
#include <entity.hh>
#include <ball.hh>
#include <glm/glm.hpp>

Dir vecDir(const glm::vec2& vec)
{
    glm::vec2 dirs[] =
    {
        glm::vec2(0.f, -1.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(-1.f, 0.f)
    };

    int dir = 0;
    float max = 0.f;

    for(int i = 0; i < 4; ++i)
    {
        float dotProduct = glm::dot(glm::normalize(vec), dirs[i]);
        if(dotProduct > max)
        {
            max = dotProduct;
            dir = i;
        }
    }
    return static_cast<Dir>(dir);
}

Collision rectRectCollision(const Entity& rect1, const Entity& rect2)
{
    bool x = rect1.getPosition().x + rect1.getSize().x > rect2.getPosition().x
            &&
            rect2.getPosition().x + rect2.getSize().x > rect1.getPosition().x;

    bool y = rect1.getPosition().y + rect1.getSize().y > rect2.getPosition().y
            &&
            rect2.getPosition().y + rect2.getSize().y > rect1.getPosition().y;

    // R1 - rect1 center
    // R2 - rect2 center
    // P - closest point from rect2 to rect1

    glm::vec2 R1 = rect1.getPosition() + rect1.getSize()/2.f;
    glm::vec2 R2 = rect2.getPosition() + rect2.getSize()/2.f;
    glm::vec2 R2R1 = R1 - R2;
    glm::vec2 R2P = glm::clamp(R2R1, -rect2.getSize()/2.f, rect2.getSize()/2.f);
    glm::vec2 P = R2 + R2P;
    glm::vec2 R1P = P - R1;

    return Collision{x && y, vecDir(R1P), P};
}

Collision ballRectCollision(const Ball& ball, const Entity& rect)
{
    // B - ball center
    // R - rect center
    // P - closest point from rect to circle

    glm::vec2 B = ball.getPosition() + ball.getRadius();
    glm::vec2 R = rect.getPosition() + rect.getSize()/2.f;
    glm::vec2 RB = B - R;
    glm::vec2 RP = glm::clamp(RB, -rect.getSize()/2.f, rect.getSize()/2.f);
    glm::vec2 P = R + RP;
    glm::vec2 BP = P - B;

    return Collision{glm::length(BP) < ball.getRadius(), vecDir(BP), P};
}

void reflectVel(Ball& ball, Dir penDir)
{
    glm::vec2 vel = ball.getVelocity();
    switch(penDir)
    {
    case Dir::up:
        ball.setVelocity(glm::vec2(vel.x, glm::abs(vel.y)));
        break;
    case Dir::down:
        ball.setVelocity(glm::vec2(vel.x, -glm::abs(vel.y)));
        break;
    case Dir::right:
        ball.setVelocity(glm::vec2(-glm::abs(vel.x), vel.y));
        break;
    case Dir::left:
        ball.setVelocity(glm::vec2(glm::abs(vel.x), vel.y));
    }
}
