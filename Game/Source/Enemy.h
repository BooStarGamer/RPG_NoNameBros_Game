#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"

#include "DynArray.h"
#include "Point.h"

#include "SDL/include/SDL.h"

#define INIT_SMALL_WOLF_POSY 444

#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141

struct SDL_Rect;
struct Animation;

enum EnemyClass
{
    SMALL_WOLF,
    BIG_WOLF,
    MANTIS,
    BIRD
};

class Enemy : public Entity
{
public:

    Enemy();

    virtual ~Enemy();

    void SetUp(EnemyClass xenemyClass, SDL_Rect collider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity);

    void Jump();

    void SmallWolfAttack(unsigned short int typeOfAttack);

    void BirdAttack(unsigned short int typeOfAttack);

private: //Attack Time
    short int attack = 0;
    short int jumpTime = 0;

    short int smallWolfTimeAttack1 = 0;
    short int smallWolfTimeAttack2 = 0;

    short int birdTimeAttack1 = 0;
    short int birdTimeAttack2 = 0;

public:
    SDL_Rect colliderCombat;

private:
    friend class Combat;

    int health;
    int maxHealth;
    int defense;
    int strength;
    int velocity;
    int lvl;
    int exp;

private:

    DynArray<iPoint>* path;

    EnemyClass enemyClass;

public:
    
    Animation cLittleWolfAwakeAnim;
    Animation cLittleWolfIdleAnim;
    Animation cLittleWolfRunAnim;
};

#endif // __ENEMY_H__
