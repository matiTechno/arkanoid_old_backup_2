#ifndef BALL_HH
#define BALL_HH

#include <entity.hh>
class Paddle;

class Ball: public Entity
{
public:
    Ball(Sprite sprite);

    virtual void update(float dt) override;
    virtual void setSize(const glm::vec2& size) override;

    void free();
    void reset();
    float getRadius() const;
    bool isStuck() const;
    void setPaddle(const Paddle* paddle);
    bool isImmune() const;
    void setImmune(float time);

private:
    float radius;
    bool isStuck_;
    float isImmune_;
    const Paddle* paddle;
};

inline void Ball::setImmune(float time)
{
    isImmune_ = time;
    getSprite().color.w = 0.5f;
}

inline bool Ball::isImmune() const
{
    if(isImmune_ > 0.f)
        return true;
    return false;
}

inline void Ball::setPaddle(const Paddle* paddle)
{
    this->paddle = paddle;
}

inline void Ball::free()
{
    isStuck_ = false;
}

inline float Ball::getRadius() const
{
    return radius;
}
inline bool Ball::isStuck() const
{
    return isStuck_;
}

#endif // BALL_HH
