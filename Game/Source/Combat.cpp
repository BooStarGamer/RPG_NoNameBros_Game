#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiButton.h"

#include "Combat.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"

#include "Log.h"

Combat::Combat()
{
	currPlayerAnim = nullptr;
}

void Combat::Start()
{
	//ENEMY SET       ENEMY CLASS            ---------------------RECT-------------------     LVL EXP  HP STR DEF VEL
	//enemy->SetUp(EnemyClass::SMALL_WOLF, { INIT_SMALLWOLF_POSX, INIT_SMALLWOLF_POSY, 70, 55 }, 2, 200, 30, 30, 10, 20);
	//enemy->SetUp(EnemyClass::BIRD, { INIT_BIRD_POSX, INIT_BIRD_POSY, 40, 75 }, 2, 200, 30, 30, 10, 20);

	//Player HardCoded
	app->scene->player1->health = 35;
	app->scene->player1->strength = 30;
	app->scene->player1->defense = 5;
	app->scene->player1->luck = 0;
	app->scene->player1->velocity = 0;
	app->scene->player1->lvl = 10;
	app->scene->player1->colliderCombat.x = INIT_COMBAT_POSX;
	app->scene->player1->colliderCombat.y = INIT_COMBAT_POSY;

	//Idle Animation Set
	currPlayerAnim = &app->scene->player1->cIdleAnim;

	//Item Inventory amount
	ItemSetup(1, 1, 1, 1, 1);

	//Bool preparation for combat
	BoolStart();

	//Firts turn decision
	FirstTurnLogic();

	//LuckArray fill
	int pLuck = app->scene->player1->luck;
	if (pLuck > 0)
	{
		for (int i = 0; i < 100; i++)
		{
			int trufals = i - pLuck;

			if (trufals < 0) luckArray[i] = true;
			else if (trufals >= 0) luckArray[i] = false;
		}
	}

	LOG("PH: %d", app->scene->player1->health);
	LOG("EH: %d", enemy->health);
}

void Combat::Restart()
{
	combatState = NULL_STATE;
	enemy = nullptr;
	app->tex->UnLoad(character1Spritesheet);
}

void Combat::Update()
{
	app->scene->attackButton->Update(0.0f);
	app->scene->moveButton->Update(0.0f);
	app->scene->itemButton->Update(0.0f);
	app->scene->scapeButton->Update(0.0f);

	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->Update(0.0f);

	currPlayerAnim->Update(1.0f);

	/*LOG("PY: %d", app->scene->player1->colliderCombat.y + app->scene->player1->colliderCombat.h);
	LOG("EY: %d", enemy->colliderCombat.y + enemy->colliderCombat.h);*/

	CombatLogic();
}

void Combat::Draw()
{
	app->scene->attackButton->Draw();
	app->scene->moveButton->Draw();
	app->scene->itemButton->Draw();
	app->scene->scapeButton->Draw();

	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->Draw();

	app->render->DrawRectangle(app->scene->player1->colliderCombat, { 100, 3, 56, 100 });

	app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x-52, app->scene->player1->colliderCombat.y-52, &currPlayerAnim->GetCurrentFrame());

	app->render->DrawRectangle(enemy->colliderCombat, { 255, 0, 0 , 255 });

	if (drawInventory) app->render->DrawRectangle(inventorySimulation, { 0, 255, 100, 50 });
}

void Combat::FirstTurnLogic()
{
	if (app->scene->player1->velocity <= enemy->velocity) combatState = ENEMY_TURN;
	else
	{
		playerChoice = true;
		combatState = PLAYER_TURN;
	}
}

void Combat::CombatLogic()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeWait < 60)
		{
			enemyTimeWait++; // Make enemy wait so it does not atack directly
			if (enemyTimeWait == 59) EnemyAttackProbability();
		}
		else
		{
			EnemyAttack(enemy->enemyClass);

			PlayerResponse();

			if (enemy->enemyClass == SMALL_WOLF || enemy->enemyClass == BIG_WOLF)
			{
				if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 30) playerResponseAble = false;
			}
			else if (enemy->enemyClass == BIRD)
			{
				if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 60 - (steps * 20)) playerResponseAble = false;
			}
		}
	}
	else if (combatState == PLAYER_TURN)
	{
		if (playerChoice)
		{
			PlayerChoiceLogic();
		}

		if (playerAttack)
		{
			PlayerAttack();
		}

		if (playerStep)
		{
			PlayerMove();
		}

		if (playerItem)
		{
			PlayerItemChoose();
		}

		if (playerSplit)
		{
			PlayerSplit();
		}
	}
	else if (combatState == WIN)
	{
		ItemDrop(enemy->enemyClass);
		playerScape = true; //Provisional, will lead to win animation and level upgrade
	}
	else if (combatState == LOSE)
	{
		if (CompareFrames(currPlayerAnim->GetCurrentFrame(), app->scene->player1->cDieAnim.GetLastFrame()))
		{
			playerScape = true; //Provisional, will lead to lose animation and respawn
		}
	}
	else if (combatState == SPLIT)
	{
		playerScape = true; //Provisional, will lead to win animation
	}
}

void Combat::BoolStart()
{
	playerScape = false;
	playerAttack = false;
	playerItem = false;
	playerStep = false;
	playerSplit = false;
	playerResponseAble = true;
	playerHitAble = true;
	playerChoice = true;
	steps = 0;

	itemChoice = true;
	healPlayerSmall = false;
	healPlayerLarge = false;
	featherPlayerTurn = false;
	protectPlayerTurn = false;
	enemyThrow = false;

	wearFeather = false;
	wearMantisLeg = false;
}

//---------------------------------------------------

void Combat::PlayerChoiceLogic()
{
	if (app->scene->attackPressed)
	{
		playerAttack = true;
		playerChoice = false;
		return;
	}
	else if (app->scene->movePressed && steps < 3)
	{
		playerStep = true;
		playerChoice = false;

		currPlayerAnim = &app->scene->player1->cStepAnim;
		currPlayerAnim->Reset();
		return;
	}
	else if (app->scene->itemPressed)
	{
		playerItem = true;
		playerChoice = false;
		drawInventory = true;
		return;
	}
	else if (app->scene->scapePressed)
	{
		playerChoice = false;
		short int probabilityRange = enemy->lvl - app->scene->player1->lvl;
		EscapeProbability(probabilityRange);
		return;
	}
	else if (app->scene->splitPressed)
	{
		playerChoice = false;
		playerSplit = true;
		return;
	}
}

int Combat::PlayerDamageLogic()
{
	int damage = 0;
	int pDamage = app->scene->player1->strength - enemy->defense;
	int pLuck = app->scene->player1->luck;

	if (steps == 0) damage += floor(15 * pDamage / 100);
	else if (steps == 1) damage += floor(35 * pDamage / 100);
	else if (steps == 2) damage += floor(65 * pDamage / 100);
	else if (steps == 3) damage += pDamage;

	if (damage < 1) //Normal enemy 0 damage, Boss 1 damage (for speedrunners) | To implement
	{
		damage = 0;
		return damage;
	}

	//Set luck
	if (pLuck == 0) return damage;
	else if (pLuck > 0)
	{
		if (pLuck > 25) pLuck = 25;
		int a = rand() % 100;
		if (luckArray[a]) return damage + floor(20 * (pDamage - enemy->defense) / 100);
		else if (!luckArray[a]) return damage;
	}

	return 0;
}

int Combat::EnemyDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defense;

	if (damage < 1) damage = 1;

	return damage;
}

void Combat::EnemyAttack(EnemyClass enemyc)
{
	if (enemyc == EnemyClass::SMALL_WOLF)
	{
		if (enemy->attack == 1)
		{
			if (enemy->smallWolfTimeAttack1 < 170)
			{
				enemy->SmallWolfAttack(enemy->attack);

				PlayerHitLogic();
			}
			else
			{
				enemy->smallWolfTimeAttack1 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_SMALLWOLF_POSX;
				playerHitAble = true;

				if (app->scene->player1->health > 0)
				{
					if (wearFeather) wearFeather = false;
					if (wearMantisLeg) wearMantisLeg = false;

					PlayerTurn();
				}
				else if (app->scene->player1->health <= 0)
				{
					PlayerDie();
				}
			}
		}
		else if (enemy->attack == 2)
		{
			if (enemy->smallWolfTimeAttack2 < 220)
			{
				enemy->SmallWolfAttack(enemy->attack);

				PlayerHitLogic();
				enemy->smallWolfTimeAttack2++;
			}
			else
			{
				enemy->smallWolfTimeAttack2 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_SMALLWOLF_POSX;
				playerHitAble = true;

				if (app->scene->player1->health > 0)
				{
					if (wearFeather) wearFeather = false;
					if (wearMantisLeg) wearMantisLeg = false;

					PlayerTurn();
				}
				else if (app->scene->player1->health <= 0)
				{
					PlayerDie();
				}
			}
		}
	}
	else if (enemyc == EnemyClass::BIRD)
	{
		if (enemy->attack == 1)
		{
			if (enemy->birdTimeAttack1 < 280)
			{
				enemy->BirdAttack(enemy->attack);

				PlayerHitLogic();
			}
			else
			{
				enemy->birdTimeAttack1 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_BIRD_POSX;
				enemy->colliderCombat.y = INIT_BIRD_POSY;
				playerHitAble = true;

				if (app->scene->player1->health > 0)
				{
					if (wearFeather) wearFeather = false;
					if (wearMantisLeg) wearMantisLeg = false;

					PlayerTurn();
				}
				else if (app->scene->player1->health <= 0)
				{
					PlayerDie();
				}
			}
		}
		else if (enemy->attack == 2)
		{
			if (enemy->birdTimeAttack2 < 200)
			{
				enemy->birdTimeAttack2++;

				if (enemy->birdTimeAttack2 >= 60 && enemy->birdTimeAttack2 < 145)
				{
					app->scene->player1->colliderCombat.x -= 2;
				}
				else if (enemy->birdTimeAttack2 == 145)
				{
					app->scene->player1->colliderCombat.x -= 1;
					steps--;
				}
			}
			else
			{
				enemy->birdTimeAttack2 = 0;
				enemyTimeWait = 0;
				playerHitAble = true;


				if (wearFeather) wearFeather = false;
				if (wearMantisLeg) wearMantisLeg = false;

				PlayerTurn();
			}
		}
	}
}

void Combat::PlayerAttack()
{
	if (playerTimeAttack < 125)
	{
		playerTimeAttack++;
	}
	else
	{
		enemy->health -= PlayerDamageLogic();
		LOG("Enemy Hit, EH: %d", enemy->health);

		playerTimeAttack = 0;
		playerAttack = false;

		PlayerPosReset();

		if (enemy->health > 0)
		{
			EnemyTurn();
		}
		else if (enemy->health <= 0)
		{
			PlayerWin();
		}
	}
}

void Combat::PlayerMove()
{
	if (playerTimeMove < 57)
	{
		app->scene->player1->colliderCombat.x += 3;
		playerTimeMove++;
	}
	else
	{
		playerTimeMove = 0;
		playerStep = false;
		steps++;

		EnemyTurn();
	}
}

void Combat::PlayerItemChoose()
{
	if (itemChoice)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			playerChoice = true;
			drawInventory = false;
			playerItem = false;
		}
		else if (itemChoice && smallMeat > 0 && app->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
		{
			itemChoice = false;
			healPlayerSmall = true;
			smallMeat--;
		}
		else if (itemChoice && largeMeat > 0 && app->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		{
			itemChoice = false;
			healPlayerLarge = true;
			largeMeat--;
		}
		else if (itemChoice && feather > 0 && app->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN)
		{
			itemChoice = false;
			featherPlayerTurn = true;
			feather--;
		}
		else if (itemChoice && mantisLeg > 0 && app->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN)
		{
			itemChoice = false;
			protectPlayerTurn = true;
			mantisLeg--;
		}
		else if (itemChoice && tamedEnemy > 0 && app->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
		{
			itemChoice = false;
			enemyThrow = true;
			tamedEnemy--;
		}
	}
	else
	{
		drawInventory = false;
		ItemUsage();
	}
}

void Combat::ItemUsage()
{
	if (healPlayerSmall)
	{
		if (playerTimeHeal < 100)
		{
			playerTimeHeal++;
		}
		else
		{
			playerTimeHeal = 0;
			playerItem = false;
			itemChoice = true;
			healPlayerSmall = false;

			app->scene->player1->health += HealPlayer(1);

			EnemyTurn();
		}
	}
	else if (healPlayerLarge)
	{
		if (playerTimeHeal < 100)
		{
			playerTimeHeal++;
		}
		else
		{
			playerTimeHeal = 0;
			playerItem = false;
			itemChoice = true;
			healPlayerLarge = false;

			app->scene->player1->health += HealPlayer(2);

			EnemyTurn();
		}
	}
	else if (featherPlayerTurn)
	{
		if (playerTimeWearFeather < 200)
		{
			playerTimeWearFeather++;
		}
		else
		{
			playerTimeWearFeather = 0;
			playerItem = false;
			itemChoice = true;
			featherPlayerTurn = false;

			wearFeather = true;
			app->scene->player1->health += HealPlayer(3);

			EnemyTurn();
		}
	}
	else if (protectPlayerTurn)
	{
		if (playerTimeWearLeg < 200)
		{
			playerTimeWearLeg++;
		}
		else
		{
			playerTimeWearLeg = 0;
			playerItem = false;
			itemChoice = true;
			protectPlayerTurn = false;

			wearMantisLeg = true;

			EnemyTurn();
		}
	}
	else if (enemyThrow)
	{
		if (playerTimeEnemyThrow < 120)
		{
			playerTimeEnemyThrow++;
		}
		else
		{
			playerTimeEnemyThrow = 0;
			playerItem = false;
			itemChoice = true;
			enemyThrow = false;

			enemy->health -= EnemyItemDamage();

			if (enemy->health > 0)
			{
				EnemyTurn();
			}
			else if (enemy->health <= 0)
			{
				PlayerWin();
			}
		}
	}
}

void Combat::PlayerSplit()
{
	if (playerTimeSplit < 200)
	{
		playerTimeSplit++;
	}
	else
	{
		playerTimeSplit = 0;
		playerSplit = false;

		int random = rand() % 5;

		// USING LUCK
		if (app->scene->player1->luck > 0)
		{
			int luck = rand() % 100;

			if (luckArray[luck]) //40% to lose
			{
				if (random < 2)
				{
					PlayerPosReset();
					EnemyTurn();
					return;
				}
				else if (random >= 2)
				{
					app->scene->player1->health -= enemy->health;

					if (app->scene->player1->health <= 0)
					{
						PlayerDie();
					}
					else if (app->scene->player1->health > 0)
					{
						PlayerSplitWin();
					}
					return;
				}
			}
		}

		// WITHOUT LUCK
		if (random < 3) //60% to lose
		{
			PlayerPosReset();
			EnemyTurn();
			return;
		}
		else if (random >= 3)
		{
			app->scene->player1->health -= enemy->health;

			if (app->scene->player1->health <= 0)
			{
				PlayerDie();
			}
			else if (app->scene->player1->health > 0)
			{
				PlayerSplitWin();
			}
		}
	}

	//M�xim d'enemics spliteats? 3
}

int Combat::HealPlayer(int typeOfHeal)
{
	int healthLeft = app->scene->player1->maxHealth - app->scene->player1->health;
	int healthRestored = 0;

	if (typeOfHeal == 1) healthRestored = ceil(app->scene->player1->maxHealth * 30 / 100);
	else if (typeOfHeal == 2) healthRestored = ceil(app->scene->player1->maxHealth * 60 / 100);
	else if (typeOfHeal == 3)
	{
		int random = rand() % 5;
		int neg = rand() % 2;
		if (neg == 0) random = -random;
		healthRestored = ceil(app->scene->player1->maxHealth * (15 + random) / 100);
	}

	if (healthLeft <= healthRestored) return healthLeft;
	else if (healthLeft > healthRestored) return healthRestored;

	return 0;
}

int Combat::EnemyItemDamage()
{
	int itemDamage = 25;
	int damage = itemDamage + floor(app->scene->player1->strength / 5) - enemy->defense;

	int damagePlus = rand() % 5;
	int negOrPos = rand() % 2;

	if (negOrPos == 0) damagePlus = -damagePlus;

	return damage + damagePlus;
}

void Combat::PlayerResponse()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->player1->jump && playerResponseAble && !app->scene->player1->crouch)
	{
		app->scene->player1->jump = true;
		currPlayerAnim = &app->scene->player1->cJumpAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump)
	{
		app->scene->player1->crouch = true;
		currPlayerAnim = &app->scene->player1->cCrouchAnim;
	}

	if (app->scene->player1->jump)
	{
		if (wearFeather) app->scene->player1->FeatherJump();
		else
		{
			app->scene->player1->Jump();
		}
	}

	if (app->scene->player1->crouch) app->scene->player1->Crouch();
}

void Combat::ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xtamedEnemy)
{
	smallMeat = xsmallMeat;
	largeMeat = xlargeMeat;
	feather = xfeather;
	mantisLeg = xmantisLeg;
	tamedEnemy = xtamedEnemy;
}

void Combat::EnemyAttackProbability()
{
	if (enemy->enemyClass == EnemyClass::SMALL_WOLF)
	{
		int random = rand() % 8;
		if (random < 6) enemy->attack = 1;
		else if (random >= 6) enemy->attack = 2;
	}
	else if (enemy->enemyClass == EnemyClass::BIRD)
	{
		if (steps > 0)
		{
			int random = rand() % 6;
			if (random < 1) enemy->attack = 2;
			return;
		}

		enemy->attack = 1;
	}
}

void Combat::PlayerMoneyLose()
{
	int lostMoney = ceil(app->scene->player1->lvl / 10);
	app->scene->player1->money -= lostMoney;

	if (app->scene->player1->money < 0) app->scene->player1->money = 0;
}

void Combat::EscapeProbability(short int probabilityRange)
{
	if (probabilityRange <= -8) playerScape = true;
	else if (probabilityRange >= -7 && probabilityRange <= -4)
	{
		int random = rand() % 4;

		if (random != 0) playerScape = true;
		else
		{
			playerAttack = false;
			playerResponseAble = true;
			playerChoice = true;

			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);

			PlayerMoneyLose();

			combatState = ENEMY_TURN;
		}
	}
	else if (probabilityRange >= -3 && probabilityRange <= 3)
	{
		int random = rand() % 2;

		if (random == 0) playerScape = true;
		else
		{
			playerAttack = false;
			playerResponseAble = true;
			playerChoice = true;

			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
			combatState = ENEMY_TURN;
		}
	}
	else if (probabilityRange >= 4 && probabilityRange <= 7)
	{
		int random = rand() % 4;

		if (random == 0) playerScape = true;
		else
		{
			playerAttack = false;
			playerResponseAble = true;
			playerChoice = true;

			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
			combatState = ENEMY_TURN;
		}
	}
	else if (probabilityRange >= 8)
	{
		playerAttack = false;
		playerResponseAble = true;
		playerChoice = true;

		LOG("ENEMY TURN");
		LOG("Enemy Health: %d", enemy->health);
		combatState = ENEMY_TURN;
	}
}

void Combat::PlayerHitLogic()
{
	if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, enemy->colliderCombat))
	{
		playerHitAble = false;
		if (!wearMantisLeg)
		{
			app->scene->player1->health -= EnemyDamageLogic();
			LOG("Player Hit - PH: %d", app->scene->player1->health);
		}
		else if (wearMantisLeg) wearMantisLeg = false;
	}
}

void Combat::PlayerPosReset()
{
	app->scene->player1->colliderCombat.x = INIT_COMBAT_POSX;
	steps = 0;
}

void Combat::ItemDrop(EnemyClass enemy)
{
	int random = rand() % 20;
	int a = rand() % 100;

	if (enemy == EnemyClass::SMALL_WOLF)
	{
		if (random < 9)
		{
			if (luckArray[a]) smallMeat += 2;
			else if (!luckArray[a]) smallMeat++;

			if (smallMeat > MAX_MEAT) smallMeat = MAX_MEAT;
		}
	}
	else if (enemy == EnemyClass::BIG_WOLF)
	{
		if (random < 6)
		{
			if (luckArray[a]) largeMeat += 2;
			else if (!luckArray[a])
			{
				largeMeat++;
				random = rand() % 20;
				if (random < 3)
				{
					a = rand() % 100;
					if (luckArray[a]) smallMeat += 2;
					else if (!luckArray[a]) smallMeat++;
				}
			}

			if (largeMeat > MAX_MEAT) largeMeat = MAX_MEAT;
			if (smallMeat > MAX_MEAT) smallMeat = MAX_MEAT;
		}
	}
	else if (enemy == EnemyClass::BIRD)
	{
		int probability = 3;
		if (luckArray[a]) probability = 10;

		if (random < probability) feather++;

		if (feather > MAX_COMBATITEM) feather = MAX_COMBATITEM;
	}
	else if (enemy == EnemyClass::MANTIS)
	{
		if (random < 2)
		{
			int probability = 2;
			if (luckArray[a]) probability = 8;

			if (random < probability) mantisLeg++;

			if (mantisLeg > MAX_COMBATITEM) mantisLeg = MAX_COMBATITEM;
		}
	}
}

// State Changing Functions

void Combat::EnemyTurn()
{
	LOG("ENEMY TURN");
	LOG("Enemy Health: %d", enemy->health);

	combatState = ENEMY_TURN;

	currPlayerAnim = &app->scene->player1->cIdleAnim;
	currPlayerAnim->Reset();

	playerResponseAble = true;
	playerChoice = true;
}

void Combat::PlayerTurn()
{
	combatState = PLAYER_TURN;
	LOG("PLAYER TURN");
	LOG("Player Health: %d", app->scene->player1->health);
}

void Combat::PlayerWin()
{
	LOG("PLAYER WIN");
	combatState = WIN;
}

void Combat::PlayerDie()
{
	currPlayerAnim = &app->scene->player1->cDieAnim;
	currPlayerAnim->Reset();
	LOG("PLAYER LOSE");
	combatState = LOSE;
}

void Combat::PlayerSplitWin()
{
	LOG("ENEMY SPLITED");
	tamedEnemy++;
	combatState = SPLIT;
}