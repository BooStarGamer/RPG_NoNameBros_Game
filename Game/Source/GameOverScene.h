#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"
#include "GuiManager.h"
#include "GuiButton.h"

#include "Collider.h"

	

class Player;
class Scene;
class Collision;
class Animation;
struct SDL_Texture;
struct SDL_Rect;
class GuiString;



class GameOverScene
{
public:

    GameOverScene();

    void Start();

    void Restart();

    void Update();

    void Draw(int y);

private:
   

public:
    GuiButton* backToMenuButton = nullptr;

    GuiButton* continueButton = nullptr;
   
    GuiString* backToMenuText = nullptr;

    GuiString* continueText = nullptr;

    SDL_Texture* background = nullptr;

};

#endif // __LEVELUP_H__


