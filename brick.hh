#ifndef BRICK_HH
#define BRICK_HH

#include <entity.hh>

class Brick: public Entity
{
public:
    Brick(Sprite sprite);

    virtual void update(float dt) override {(void)dt;}

    bool isSolid_;
    int hp;
};

#endif // BRICK_HH
