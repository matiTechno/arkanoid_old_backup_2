#include <explosion.hh>

Explosion::Explosion(Sprite sprite, const Animation& anim):
    Entity(sprite),
    anim(anim)
{}

void Explosion::update(float dt)
{
    anim.update(dt, *this);
}
