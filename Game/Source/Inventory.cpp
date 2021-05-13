#define _CRT_SECURE_NO_WARNINGS

#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "EntityManager.h"

#include "Inventory.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"
#include "Defs.h"

#include "Log.h"
#include <string>
#include <stdio.h>

Inventory::Inventory()
{

}

void Inventory::Start()
{
	

	interfaceTexture = app->tex->Load("Assets/Textures/UI/inventory.png");
	faceAnimationTexture = app->tex->Load("Assets/Textures/UI/face-animations.png");
	statsTexture = app->tex->Load("Assets/Textures/UI/stats.png");

	idleFaceAnim.PushBack({ 0, 0, 70, 68 });

	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 70, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.speed = 0.1f;
	blinkFaceAnim.loop = false;

	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.PushBack({ 0, 70, 70, 68 });
	angryFaceAnim.PushBack({ 70, 70, 70, 68 });
	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.speed = 0.1f;
	angryFaceAnim.loop = false;

	currPlayerFaceAnim = &idleFaceAnim;


	//texts
	SetText();


}

void Inventory::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(faceAnimationTexture);
	app->tex->UnLoad(statsTexture);

}

void Inventory::Update()
{
	quitQuestButton->Update(1.0f);

	exitInventoryButton->Update(1.0f);

	bigBeefButton->UpdateOnClick(1.0f);
	littlebeefButton->UpdateOnClick(1.0f);
	featherButton->UpdateOnClick(1.0f);
	mantisButton->UpdateOnClick(1.0f);
	coinButton->UpdateOnClick(1.0f);
	splitButton->UpdateOnClick(1.0f);
}

void Inventory::UseItems()
{
}

void Inventory::Draw()
{
	DrawInterface();

	DrawFace();

	

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->exp, BLUE);

	//DrawStats();

	UpdateButtons();

	DrawButtons();

	//UpdateText();

	DrawText();

	app->guiManager->DrawCursor();

}

void Inventory::DrawInterface()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawBar(iPoint pos, int current, int max, SDL_Color color)
{
	int size = 261;
	int thickness = 20;
	float percent = 1;
	
	float c = current;
	float m = max;

	if (current > 0 && max > 0)
	{
		percent = (c / m)*100;
		//LOG("current: %d,max: %d  =  inv: %f", current, max, percent);
	}

	if (color.r > 240) //red
	{
		app->render->DrawRectangle({ pos.x,pos.y,size,thickness }, DARK_RED, true, false);

		int lenght = size * percent/100;
		app->render->DrawRectangle({ pos.x,pos.y,lenght,thickness }, RED, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &healthBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	}
	else //blue
	{
		app->render->DrawRectangle({ pos.x,pos.y,size,thickness }, DARK_BLUE, true, false);

		int lenght = size * percent / 100;
		app->render->DrawRectangle({ pos.x,pos.y,lenght ,thickness }, BLUE, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &expBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	}
}

void Inventory::SetButtons()
{
	if (app->scene->player1->playerInventory->smallMeatCount == 0) littlebeefButton->state = GuiControlState::DISABLED;
	else littlebeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->largeMeatCount == 0) bigBeefButton->state = GuiControlState::DISABLED;
	else bigBeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->featherCount == 0) featherButton->state = GuiControlState::DISABLED;
	else featherButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->mantisRodCount == 0) mantisButton->state = GuiControlState::DISABLED;
	else mantisButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->moneyCount == 0) coinButton->state = GuiControlState::DISABLED;
	else coinButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->splitedEnemyCount == 0) splitButton->state = GuiControlState::DISABLED;
	else splitButton->state = GuiControlState::NORMAL;

}

void Inventory::UpdateButtons()
{
	if (app->scene->player1->playerInventory->smallMeatCount == 0) littlebeefButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->largeMeatCount == 0) bigBeefButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->featherCount == 0) featherButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->mantisRodCount == 0) mantisButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->moneyCount == 0) coinButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->splitedEnemyCount == 0) splitButton->state = GuiControlState::DISABLED;

	coinButton->state = GuiControlState::NORMAL;
}

void Inventory::SetInventoryValues()
{
}

void Inventory::DrawButtons()
{
	quitQuestButton->Draw(1, true, true, ButtonType::QUIT);

	exitInventoryButton->Draw(1, true, true, ButtonType::INVENTORY);

	littlebeefButton->Draw(1, true, true, ButtonType::LITTLE_BEEF_B);
	bigBeefButton->Draw(1, true, true, ButtonType::BIG_BEEF_B);
	featherButton->Draw(1, true, true, ButtonType::FEATHER_B);
	mantisButton->Draw(1, true, true, ButtonType::MANTIS_B);
	coinButton->Draw(1, true, true, ButtonType::COIN_B);
	splitButton->Draw(1, true, true, ButtonType::SPLIT_B);
}

void Inventory::UpdateFace()
{
	counter++;

	if (counter % 60 == 0)
	{
		seconds++;
		//LOG("seconds : %d", seconds);
	}

	if (seconds >= 3)
	{
		blinkFaceAnim.Reset();
		currPlayerFaceAnim = &blinkFaceAnim;
		seconds = 0;
	}
}

void Inventory::DrawFace()
{
	UpdateFace();

	currPlayerFaceAnim->Update(1.0f);

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y, 1.4f, false, &currPlayerFaceAnim->GetCurrentFrame(), 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);


}

void Inventory::SetText()
{
	//player
	if (healthText == nullptr)
	{
		healthText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthText->bounds = { healthBarPos.x, healthBarPos.y+barOff.x, 200, 50 };
		healthText->SetTextFont(app->fontTTF->inventoryFont);
		
		char hp[5] = {};
		char mhp[5] = {};
		char slash[2] = {"/"};
		char t[6] = { "HP : " };
		sprintf(hp, "%d", app->scene->player1->health);
		sprintf(mhp, "%d", app->scene->player1->maxHealth);
		
		char* str;
		char* str1;
		str=strcat(hp, slash);
		str1 = strcat(str, mhp);
		str = strcat(t, str1);

		healthText->SetString(str, YELLOW);
	}

	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { expBarPos.x,expBarPos.y+barOff.x, 200, 50 };
		expText->SetTextFont(app->fontTTF->inventoryFont);
		
		char xp[5] = {};
		char mxp[5] = {};
		char slash[2] = { "/" };
		char t[6] = { "XP : " };
		sprintf(xp, "%d", app->scene->player1->exp);
		sprintf(mxp, "%d", app->scene->player1->exp);

		char* str;
		char* str1;
		str = strcat(xp, slash);
		str1 = strcat(str, mxp);
		str = strcat(t, str1);

		expText->SetString(str, YELLOW);
	}



	//ITEMS
	if (littleBeefText == nullptr)
	{
		littleBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		littleBeefText->bounds = { numberItemPos.x, numberItemPos.y, 100, 100 };
		littleBeefText->SetTextFont(app->fontTTF->inventoryFont);
		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->smallMeatCount);
		littleBeefText->SetString(str, BROWN);
	}
	if (bigBeefText == nullptr)
	{
		bigBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		bigBeefText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y, 100, 100 };
		bigBeefText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->largeMeatCount);
		bigBeefText->SetString(str, BROWN);
	}
	if (featherText == nullptr)
	{
		featherText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		featherText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		featherText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->featherCount);
		featherText->SetString(str, BROWN);
	}
	if (mantisText == nullptr)
	{
		mantisText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		mantisText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		mantisText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->mantisRodCount);
		mantisText->SetString(str, BROWN);
	}
	if (coinText == nullptr)
	{
		coinText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		coinText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		coinText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->moneyCount);
		coinText->SetString(str, BROWN);
	}
	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		splitText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->playerInventory->splitedEnemyCount);
		splitText->SetString(str, BROWN);
	}

	//Stats
	if (healthStatText == nullptr)
	{
		healthStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthStatText->bounds = { statsPos.x,statsPos.y,100,100 };
		healthStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->healthStat);
		healthStatText->SetString("12", BROWN);
	}
	if (strenghtStatText == nullptr)
	{
		strenghtStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strenghtStatText->bounds = { statsPos.x,statsPos.y + statsOff.y,100,100 };
		strenghtStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->strengthStat);
		strenghtStatText->SetString(str, BROWN);
	}
	if (defenseStatText == nullptr)
	{
		defenseStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		defenseStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 2,100,100 };
		defenseStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->defenseStat);
		defenseStatText->SetString(str, BROWN);
	}
	if (velocityStatText == nullptr)
	{
		velocityStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		velocityStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 3,100,100 };
		velocityStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->velocityStat);
		velocityStatText->SetString(str, BROWN);
	}
	if (luckStatText == nullptr)
	{
		luckStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		luckStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 4,100,100 };
		luckStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->luckStat);
		luckStatText->SetString(str, BROWN);
	}
	if (stabStatText == nullptr)
	{
		stabStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		stabStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,100,100 };
		stabStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->stabStat);
		stabStatText->SetString(str, BROWN);
	}

	//quest
	if (questText == nullptr)
	{
		questText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		questText->bounds = { 60,600, 100, 100 };
		questText->SetTextFont(app->fontTTF->inventoryFont);
		questText->SetString("Dale ostias a un lobo");
	}

}

void Inventory::UpdateText()
{

	char str[5] = {};
	sprintf(str, "%d", app->scene->player1->playerInventory->smallMeatCount);
	littleBeefText->SetString(str, BROWN);

	char str1[5] = {};
	sprintf(str1, "%d", app->scene->player1->playerInventory->largeMeatCount);
	bigBeefText->SetString(str1, BROWN);

	char str2[5] = {};
	sprintf(str2, "%d", app->scene->player1->playerInventory->featherCount);
	featherText->SetString(str2, BROWN);

	char str3[5] = {};
	sprintf(str3, "%d", app->scene->player1->playerInventory->mantisRodCount);
	mantisText->SetString(str3, BROWN);

	char str4[5] = {};
	sprintf(str4, "%d", app->scene->player1->playerInventory->moneyCount);
	coinText->SetString(str4, BROWN);

	char str5[5] = {};
	sprintf(str5, "%d", app->scene->player1->playerInventory->splitedEnemyCount);
	splitText->SetString(str5, BROWN);

}

void Inventory::DrawText()
{
	littleBeefText->Draw();
	bigBeefText->Draw();
	featherText->Draw();
	mantisText->Draw();
	coinText->Draw();
	splitText->Draw();

	healthStatText->Draw();
	strenghtStatText->Draw();
	defenseStatText->Draw();
	velocityStatText->Draw();
	stabStatText->Draw();
	luckStatText->Draw();

	questText->Draw();
}