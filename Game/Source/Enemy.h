#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

struct SDL_Rect;

class Enemy : public Entity
{
public:

    Enemy();

    virtual ~Enemy();

    void SetUp(SDL_Rect collider);

public:
    SDL_Rect colliderCombat;

private:
    friend class Combat;

    int health;
    int defense;
    int strength;
    int velocity;

private:

    DynArray<iPoint>* path;

};

#endif // __ENEMY_H__
