#include "App.h"
#include "Scene.h"+
#include "Combat.h"

#include "Enemy.h"

#include "Pathfinding.h"

#include "Log.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));

    //Load enemies textures
    for (int i = 8; i >=0 ; i--)
    {
        cLittleWolfAwakeAnim.PushBack({ SPRITE_TILE_WIDTH * i,0 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
    }
    cLittleWolfAwakeAnim.speed = 0.120f;

    for (int i = 0; i < 5; i++)
    {
        cLittleWolfIdleAnim.PushBack({ SPRITE_TILE_WIDTH * i,0 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
    }
    cLittleWolfIdleAnim.speed = 0.120f;

    for (int i = 0; i < 5; i++)
    {
        cLittleWolfRunAnim.PushBack({ SPRITE_TILE_WIDTH * i,0 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
    }
    cLittleWolfRunAnim.speed = 0.120f;
}

Enemy::~Enemy()
{
}

void Enemy::SetUp(EnemyClass xenemyClass, SDL_Rect collider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity)
{
    enemyClass = xenemyClass;
    colliderCombat = collider;
    lvl = xlvl;
    exp = xexp;
    health = xhealth;
    maxHealth = xhealth;
    strength = xstrength;
    defense = xdefense;
    velocity = xvelocity;
}

void Enemy::Jump()
{
    if (jumpTime < 28)
    {
        colliderCombat.y -= 14;
        colliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        colliderCombat.y = INIT_SMALL_WOLF_POSY;
        jumpTime = 0;
    }
}

void Enemy::SmallWolfAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        colliderCombat.x -= 8;
        smallWolfTimeAttack1++;

        if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;
    }
    else if (typeOfAttack == 2)
    {
        if (smallWolfTimeAttack2 < 29)
        {
            Jump();
        }
        else if (smallWolfTimeAttack2 < 50)
        {
            int a = 0;
        }
        else if (smallWolfTimeAttack2 < 220)
        {
            if (app->scene->combatScene->steps == 0)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 135 && smallWolfTimeAttack2 < 165) Jump();
            }
            else if (app->scene->combatScene->steps == 1)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 113 && smallWolfTimeAttack2 < 143) Jump();
            }
            else if (app->scene->combatScene->steps == 2)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 90 && smallWolfTimeAttack2 < 120) Jump();
            }
            else if (app->scene->combatScene->steps == 3)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 68 && smallWolfTimeAttack2 < 98) Jump();
            }
        }
    }
}

void Enemy::BirdAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        if (app->scene->combatScene->steps == 0)
        {
            if (birdTimeAttack1 < 95)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 120) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 155) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 180) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 179) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 215) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 248)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 1)
        {
            if (birdTimeAttack1 < 75)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 100) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 135) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 160) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 160) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 195) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 228)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 2)
        {
            if (birdTimeAttack1 < 51)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 76) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 111) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 136) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 135) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 171) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 204)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 3)
        {
            if (birdTimeAttack1 < 31)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 56) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 91) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 116) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 115) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 151) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 184)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }

        birdTimeAttack1++;

        if (colliderCombat.x + colliderCombat.w < 0)
        {
            colliderCombat.x = 1280;
            colliderCombat.y = INIT_BIRD_POSY;
        }
    }
    else if (typeOfAttack == 2)
    {
        if (birdTimeAttack2 < 30)
        {
            colliderCombat.x += 3;
        }
        else if (birdTimeAttack2 < 50)
        {
            colliderCombat.x += 0;
        }
        else if (birdTimeAttack2 < 75)
        {
            colliderCombat.x -= 12;
            colliderCombat.y += 3;
        }
        else if (birdTimeAttack2 < 150)
        {
            colliderCombat.x -= 12;
            if (birdTimeAttack2 == 149)
            {
                app->scene->combatScene->playerResponseAble = true;
                app->scene->combatScene->playerHitAble = true;
            }
        }
        else if (birdTimeAttack2 < 168)
        {
            colliderCombat.y--;
            colliderCombat.x -= 12;
        }
        else if (birdTimeAttack2 < 280)
        {
            colliderCombat.x -= 12;
        }

        birdTimeAttack2++;

        if (colliderCombat.x + colliderCombat.w < 0)
        {
            colliderCombat.x = 1280;
            colliderCombat.y = INIT_BIRD_POSY;
        }
    }
}


