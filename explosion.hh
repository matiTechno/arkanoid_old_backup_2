#ifndef EXPLOSION_HH
#define EXPLOSION_HH

#include <entity.hh>
#include <animation.hh>

class Explosion: public Entity
{
public:
    Explosion(Sprite sprite, const Animation& anim);
    virtual void update(float dt) override;
    void set();
    bool isEnd() const;

private:
    Animation anim;
};

inline void Explosion::set()
{
    anim.set(*this);
}

inline bool Explosion::isEnd() const
{
    if(anim.getState() == State::end)
        return true;
    return false;
}

#endif // EXPLOSION_HH
