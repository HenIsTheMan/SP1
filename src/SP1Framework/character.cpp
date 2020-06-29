#include "game.h"

SGameChar g_sChar; //For char
double PowerBounceTime = 0.0, PowerDisplayTime = 0.0;

extern Console *g_Console;
extern bulletPool<Bullet> *water, *fire;
extern bulletPool<Homing> *hom;
extern MUSIC music;
extern std::vector<std::vector<int>> levelMap;
extern EGAMESTATES g_eGameState;
extern int score, playerKeys[11], tileSizeX, difficulty, powerup;
extern double g_dElapsedTime, gameTimePause, gameTimeStart, gameTime, charBounceTime, waterBounceTime, menuBounceTime;

void characterLogicMap() {
	bool bSomethingHappened = 0;
	if (charBounceTime <= g_dElapsedTime) {
		//Sprint
		if (GetAsyncKeyState(playerKeys[8]) < 0) {
			//Up
			if (GetAsyncKeyState(playerKeys[0]) < 0) {
				g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y > 6 ? g_sChar.m_cLocation.Y - 2 : 5);
				score -= 2;
				bSomethingHappened = 1;
			}
			//Down
			if (GetAsyncKeyState(playerKeys[1]) < 0) {
				g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y < g_Console->getConsoleSize().Y - 7 ? g_sChar.m_cLocation.Y + 2 : g_Console->getConsoleSize().Y - 6);
				score -= 2;
				bSomethingHappened = 1;
			}
			//Left
			if (GetAsyncKeyState(playerKeys[2]) < 0) {
				//g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X > 14 ? g_sChar.m_cLocation.X - 4 : 10);
				if (g_sChar.m_cLocation.X > 14) g_sChar.m_cLocation.X -= 4;
				else { g_sChar.m_cLocation.X = 10; --g_sChar.health; }
				score -= 2;
				bSomethingHappened = 1;
			}
			//Right
			if (GetAsyncKeyState(playerKeys[3]) < 0) {
				g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X < g_Console->getConsoleSize().X - 15 ? g_sChar.m_cLocation.X + 4 : g_Console->getConsoleSize().X - 12);
				score -= 2;
				bSomethingHappened = 1;
			}
		} 
		else {
			//Up
			if (GetAsyncKeyState(playerKeys[0]) < 0) {
				g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y > 5 ? g_sChar.m_cLocation.Y - 1 : 5);
				bSomethingHappened = 1;
			}
			//Down
			if (GetAsyncKeyState(playerKeys[1]) < 0) {
				g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y < g_Console->getConsoleSize().Y - 6 ? g_sChar.m_cLocation.Y + 1 : g_Console->getConsoleSize().Y - 6);
				bSomethingHappened = 1;
			}
			//Left
			if (GetAsyncKeyState(playerKeys[2]) < 0) {
				//g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X > 12 ? g_sChar.m_cLocation.X - 2 : 10);
				if (g_sChar.m_cLocation.X > 12) g_sChar.m_cLocation.X -= 2;
				else { g_sChar.m_cLocation.X = 10; --g_sChar.health; }
				bSomethingHappened = 1;
			}
			//Right
			if (GetAsyncKeyState(playerKeys[3]) < 0) {
				g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X < g_Console->getConsoleSize().X - 13 ? g_sChar.m_cLocation.X + 2 : g_Console->getConsoleSize().X - 12);
				bSomethingHappened = 1;
			}
		}
		if (bSomethingHappened) charBounceTime = g_dElapsedTime + 0.075;
	}

	// Bullets
	if (waterBounceTime <= g_dElapsedTime) {
		// Up
		if (GetAsyncKeyState(playerKeys[4]) < 0) {
			Bullet *instance = water->getPooledObj();
			instance->active = 1;
			instance->location = g_sChar.m_cLocation;
			instance->dir = 1;
			waterBounceTime = g_dElapsedTime + 0.1;
		}
		// Down
		else if (GetAsyncKeyState(playerKeys[5]) < 0) {
			Bullet *instance = water->getPooledObj();
			instance->active = 1;
			instance->location = g_sChar.m_cLocation;
			instance->dir = 2;
			waterBounceTime = g_dElapsedTime + 0.1;
		}
		// Left
		else if (GetAsyncKeyState(playerKeys[6]) < 0) {
			Bullet *instance = water->getPooledObj();
			instance->active = 1;
			instance->location = g_sChar.m_cLocation;
			instance->dir = 3;
			waterBounceTime = g_dElapsedTime + 0.1;
		}
		// Right
		else if (GetAsyncKeyState(playerKeys[7]) < 0) {
			Bullet *instance = water->getPooledObj();
			instance->active = 1;
			instance->location = g_sChar.m_cLocation;
			instance->dir = 4;
			waterBounceTime = g_dElapsedTime + 0.1;
		}
	}

	// Water Collisions
	for (int i = 0; i < water->mySize(); ++i) {
		if (!(water->myVec())[i]->active) {
			(water->myVec())[i]->location = g_sChar.m_cLocation;
			(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
		}
		if ((water->myVec())[i]->active) {
			bool condition5 = 0;
			switch ((water->myVec())[i]->dir) {
			case 1: --(water->myVec())[i]->location.Y; break;
			case 2: ++(water->myVec())[i]->location.Y; break;
			case 3: (water->myVec())[i]->location.X -= 2; break;
			case 4: (water->myVec())[i]->location.X += 2; break;
			}

			// Collision Wall
			if (levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][((water->myVec())[i]->location.Y) / 5] == 2 ||
				(levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][((water->myVec())[i]->location.Y) / 5] == 2 &&
				((water->myVec())[i]->location.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX))
			{
				condition5 = 1;
			}
			// Collision Fire
			if (levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][((water->myVec())[i]->location.Y) / 5] == 1 ||
				(levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][((water->myVec())[i]->location.Y) / 5] == 1 &&
				((water->myVec())[i]->location.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX))
			{
				if (levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][((water->myVec())[i]->location.Y) / 5] == 1)
					levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][((water->myVec())[i]->location.Y) / 5] = 0;
				if (levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][((water->myVec())[i]->location.Y) / 5] == 1)
					levelMap[((water->myVec())[i]->location.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][((water->myVec())[i]->location.Y) / 5] = 0;
				condition5 = 1;
			}

			//Water bullet destruction when water bullets leave the game room
			bool condition1 = ((water->myVec())[i]->location.Y <= 4 && (water->myVec())[i]->dir == 1),
				condition2 = ((water->myVec())[i]->location.Y >= g_Console->getConsoleSize().Y - 5 && (water->myVec())[i]->dir == 2),
				condition3 = ((water->myVec())[i]->location.X <= 9 && (water->myVec())[i]->dir == 3),
				condition4 = ((water->myVec())[i]->location.X >= g_Console->getConsoleSize().X - 10 && (water->myVec())[i]->dir == 4);

			if (condition1 || condition2 || condition3 || condition4 || condition5) {
				(water->myVec())[i]->active = 0;
				(water->myVec())[i]->location = g_sChar.m_cLocation;
				(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
			}
		}
	}

	// Health
	if (g_sChar.health > 10) g_sChar.health = 10;
	if (g_sChar.health <= 0) {
		newHighscore();
		menuBounceTime = g_dElapsedTime + 1.0;
		g_eGameState = S_GAMEOVER;
		music = M_ERASE;
	}
}

void moveCharacterBoss() {
	if (charBounceTime <= g_dElapsedTime) {
		// UP
		if (GetAsyncKeyState(playerKeys[0])) {
			g_sChar.renderLocation.Y -= ((g_sChar.renderLocation.Y > 1) * (g_sChar.m_cLocation.Y < 25)) || ((g_sChar.m_cLocation.Y > 76) && (g_sChar.renderLocation.Y > g_Console->getConsoleSize().Y / 2));
			g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y > 0 ? g_sChar.m_cLocation.Y - 1 : 0);
		}
		// DOWN
		if (GetAsyncKeyState(playerKeys[1])) {
			g_sChar.renderLocation.Y += ((g_sChar.renderLocation.Y < 50 - 2) * (g_sChar.m_cLocation.Y > 76)) || ((g_sChar.m_cLocation.Y < 25 && g_sChar.renderLocation.Y < g_Console->getConsoleSize().Y / 2));
			g_sChar.m_cLocation.Y = (g_sChar.m_cLocation.Y < g_Console->getConsoleSize().Y * 2 ? g_sChar.m_cLocation.Y + 1 : g_Console->getConsoleSize().Y * 2);
		}
		// LEFT
		if (GetAsyncKeyState(playerKeys[2])) {
			g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X > 0 ? g_sChar.m_cLocation.X - 2 : 0);
			g_sChar.renderLocation.X -= 2 * (g_sChar.renderLocation.X > 1) * (g_sChar.m_cLocation.X <= 98) - (g_sChar.renderLocation.X == 2);
			if (g_sChar.m_cLocation.X > 98 && g_sChar.renderLocation.X > g_Console->getConsoleSize().X / 2) {
				g_sChar.renderLocation.X -= 2 - (g_sChar.renderLocation.X - 1 == g_Console->getConsoleSize().X / 2);
			}
		}
		// RIGHT
		if (GetAsyncKeyState(playerKeys[3])) {
			g_sChar.m_cLocation.X = (g_sChar.m_cLocation.X < g_Console->getConsoleSize().X * 2 ? g_sChar.m_cLocation.X + 2 : g_Console->getConsoleSize().X * 2);
			g_sChar.renderLocation.X += 2 * (g_sChar.renderLocation.X < 200 - 3) * (g_sChar.m_cLocation.X >= 302) - (g_sChar.renderLocation.X == 196);
			if (g_sChar.m_cLocation.X < 302 && g_sChar.renderLocation.X < g_Console->getConsoleSize().X / 2) {
				g_sChar.renderLocation.X += 2 - (g_sChar.renderLocation.X + 1 == g_Console->getConsoleSize().X / 2);
			}
		}
		charBounceTime = g_dElapsedTime + 0.05;
	}

	// Bullets
	int keys[4];
	for (int i = 0; i < 4; ++i) keys[i] = playerKeys[i + 4];
	for (int j = 0; j < sizeof(keys) / sizeof(int); ++j) {
		if (GetAsyncKeyState(keys[j]) && waterBounceTime <= g_dElapsedTime) {
			Bullet *instance = water->getPooledObj();
			instance->active = 1;
			switch (j) {
				case 0: instance->dir = 3; break;
				case 1: instance->dir = 4; break;
				case 2: instance->dir = 1; break;
				case 3: instance->dir = 2; break;
			}
			waterBounceTime = g_dElapsedTime + 0.1;
		}
	}
	for(int i = 0; i < water->mySize(); ++i){
		if(!(water->myVec())[i]->active){ //Move water bullets to char location if they are not active
			(water->myVec())[i]->location = g_sChar.m_cLocation;
			(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
		}
		if((water->myVec())[i]->active && (water->myVec())[i]->bulletBounceTime <= g_dElapsedTime){
			bool collided = 0;
			for (int j = 0; j < hom->mySize(); ++j) {
				if ((((water->myVec())[i]->dir == 1 && (water->myVec())[i]->location.X - 1 <= (hom->myVec())[j]->location.X) || ((water->myVec())[i]->dir == 2 && (water->myVec())[i]->location.X + 1 >= (hom->myVec())[j]->location.X)
					|| ((water->myVec())[i]->location.X == (hom->myVec())[j]->location.X)) && (water->myVec())[i]->location.Y == (hom->myVec())[j]->location.Y) {
					collided = 1; //Collision check
				}
			}
			if (!collided) { //Water movement based on its dir if not collided
				switch ((water->myVec())[i]->dir) {
				case 1: (water->myVec())[i]->location.X -= 2; (water->myVec())[i]->renderLocation.X -= 2; break;
				case 2: (water->myVec())[i]->location.X += 2; (water->myVec())[i]->renderLocation.X += 2; break;
				case 3: --(water->myVec())[i]->location.Y; --(water->myVec())[i]->renderLocation.Y; break;
				case 4: ++(water->myVec())[i]->location.Y; ++(water->myVec())[i]->renderLocation.Y;
				}
			}
			(water->myVec())[i]->bulletBounceTime = g_dElapsedTime + 0.02;

			//Water bullet destruction when water bullets leave the boss room
			bool condition1 = ((water->myVec())[i]->renderLocation.X < 0 && (water->myVec())[i]->dir == 1), condition2 = ((water->myVec())[i]->renderLocation.X > g_Console->getConsoleSize().X - 1 && (water->myVec())[i]->dir == 2);
			bool condition3 = ((water->myVec())[i]->renderLocation.Y < -1 && (water->myVec())[i]->dir == 3), condition4 = ((water->myVec())[i]->renderLocation.Y > g_Console->getConsoleSize().Y - 1 && (water->myVec())[i]->dir == 4);
			if(condition1 || condition2 || condition3 || condition4){
				(water->myVec())[i]->active = 0;
				(water->myVec())[i]->location = g_sChar.m_cLocation;
				(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
			}
		}
	}
	// Health
	if (g_sChar.health > 10) g_sChar.health = 10;
	if (g_sChar.health <= 0) {
		newHighscore();
		menuBounceTime = g_dElapsedTime + 1.0;
		g_eGameState = S_GAMEOVER;
		music = M_ERASE;
	}
}

void PowerUps() { //Powerups
	switch (powerup) {
		case 0: score += 40; break;
		case 1: g_sChar.health += 5; break;
		default:
			PowerBounceTime = g_dElapsedTime + 4.0;
			break;
	};
	PowerDisplayTime = g_dElapsedTime + 3.0;
}