#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Input.h"
#include "Render.h"
#include "Animation.h"

#include "Point.h"

#include "SDL/include/SDL.h"

#define INIT_COMBAT_POSX 225
#define INIT_COMBAT_POSY 400
#define INIT2_COMBAT_POSX 275
#define INIT2_COMBAT_POSY 350
#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141


class Animation;

class Player : public Entity
{
public:

    Player();

    Player(SDL_Rect, EntityType);

    bool Update(float dt);

    bool Draw();

    void SetTexture(SDL_Texture *tex);

    void Jump();

    void FeatherJump();

    void Crouch();

    SDL_Rect GetCombatBounds()
    {
        return colliderCombat;
    }

    SDL_Rect GetWorldBounds()
    {
        return colliderWorld;
    }

    SDL_Rect GetCollisionBounds()
    {
        return collisionRect;
    }

    int GetCollCombatWidth() const
    {
        return colliderCombat.w;
    }

    int GetCollCombatHeight() const
    {
        return colliderCombat.h;
    }

    int GetCollWorldWidth() const
    {
        return colliderWorld.w;
    }

    int GetCollWorldHeight() const
    {
        return colliderWorld.h;
    }

public:
    Animation cIdleAnim;
    Animation cPos0AttackAnim;
    Animation cPos1AttackAnim;
    Animation cPos2AttackAnim;
    Animation cPos3AttackAnim;
    Animation cStepAnim;
    Animation cJumpAnim;
    Animation cDieAnim;
    Animation cCrouchAnim;

    bool jump = false;
    bool crouch = false;
    int jumpTime = 0;
    int jumpFeatherTime = 0;
    int crouchTime = 0;

public:
    friend class Combat;

    int health = 35;
    int maxHealth = 35;
    int defense = 5;
    int strength = 30;
    int velocity = 0;
    int luck = 0;
    int stab;
    
    int lvl;
    int exp;
    int money;

public:
    SDL_Texture* texture;
    int playerSpeed;

    iPoint positionLive;
    int positionCombat;

    SDL_Rect colliderWorld;
    SDL_Rect collisionRect;
    SDL_Rect colliderCombat;
public:
    bool godMode = false;
};

#endif // __PLAYER_H__
