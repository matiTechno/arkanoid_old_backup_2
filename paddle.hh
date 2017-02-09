#ifndef PADDLE_HH
#define PADDLE_HH

#include <entity.hh>
class Ball;
enum class Dir;

class Paddle: public Entity
{
public:
    Paddle(Sprite sprite, Ball& ball);

    virtual void update(float dt) override;
    void reflect(Dir pene);
    float getVelMod() const;
    void setVelMod(float velMod);
    // in radians
    void setMinReflectAngle(float angle);

private:
    Ball& ball;
    float velMod;
    float minReflectAngle;
};

inline float Paddle::getVelMod() const
{
    return velMod;
}

inline void Paddle::setVelMod(float velMod)
{
    this->velMod = velMod;
}

inline void Paddle::setMinReflectAngle(float angle)
{
    minReflectAngle = angle;
}

#endif // PADDLE_HH
