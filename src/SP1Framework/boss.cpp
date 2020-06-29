#include "game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <random>

extern Console *g_Console;
SHORT xPos, yPos; //General
int bossDir = 0, count = 0, bossHealth = 400;
double charBounceTime = 0.0, fireBounceTime = 0.0, waterBounceTime = 0.0, spawnBounceTime = 0.0, dirBounceTime = 0.0, moveBounceTime = 0.0, tpBounceTime = 0.0;
extern bulletPool<Bullet> *water, *fire;
extern bulletPool<Homing> *hom;

extern THEMES themeSelected;
extern WORD BossColours[4][4], ThemeColours[4][9];
extern SGameChar g_sChar;
extern double gameTime;

void boss() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> range(1, 8);
	srand(int(time(NULL)));

	if (bossHealth >= (bossHealth / 10)) {
		//Determine boss direction
		if (dirBounceTime <= gameTime) {
			if (count % 3) {
				bossDir = rand() % 2 + 1;
			}
			else {
				bossDir = rand() % 2 + 3;
			}
			count++;
			dirBounceTime = gameTime + 5;
		}

		//Boss movement when health is not low
		if (moveBounceTime <= gameTime) {
			switch (bossDir) {
			case 1: xPos -= 2; break;
			case 2: xPos += 2; break;
			case 3: --yPos; break;
			case 4: ++yPos;
			}
			moveBounceTime = gameTime + 0.1;
		}
	}
	else {
		//Boss teleportation when health is low
		if (tpBounceTime <= gameTime) {
			xPos = g_Console->getConsoleSize().X + SHORT(range(gen) % 3 - 1) * 5;
			yPos = g_Console->getConsoleSize().Y + SHORT(range(gen) % 3 - 1) * 5;
			//tpBounceTime = gameTime + 0.1;
		}
	}

	//Spawning of homing bullets
	if (spawnBounceTime <= gameTime) {
		Homing *homInstance = hom->getPooledObj();
		homInstance->active = 1;
		homInstance->delay = 0.20;
		spawnBounceTime = gameTime + 1;
	}
	for (int i = 0; i < hom->mySize(); ++i) {
		bool adjusted = 0, collided = 0;

		//Homing bullet goes to boss location when it's not active
		if (!(hom->myVec())[i]->active) {
			(hom->myVec())[i]->location = { xPos, yPos };
		}
		// Player & Homming Collision
		if ((hom->myVec())[i]->location.X == g_sChar.m_cLocation.X && (hom->myVec())[i]->location.Y == g_sChar.m_cLocation.Y) {
			--g_sChar.health;
			(hom->myVec())[i]->active = 0;
			(hom->myVec())[i]->location = g_sChar.m_cLocation;
			(hom->myVec())[i]->renderLocation = g_sChar.renderLocation;
			continue;
		}
		(hom->myVec())[i]->move1 = ((hom->myVec())[i]->location.Y > g_sChar.m_cLocation.Y ? 3 : 4); //determine whether vert displacement is +ve or -ve
		(hom->myVec())[i]->move2 = ((hom->myVec())[i]->location.X > g_sChar.m_cLocation.X ? 1 : 2); //determine whether horizontal displacement is +ve or -ve
		(hom->myVec())[i]->move = (abs((hom->myVec())[i]->location.Y - g_sChar.m_cLocation.Y) < abs((hom->myVec())[i]->location.X - g_sChar.m_cLocation.X) ? (hom->myVec())[i]->move1 : (hom->myVec())[i]->move2); //Set movement
		
		if ((hom->myVec())[i]->location.X == g_sChar.m_cLocation.X) { //Change dir when homing bullet and char have the same x-coordinate
			adjusted = 1;
			(hom->myVec())[i]->move = (hom->myVec())[i]->move1;
		}
		else if ((hom->myVec())[i]->location.Y == g_sChar.m_cLocation.Y) { //Change dir when homing bullet and char have the same y-coordinate
			adjusted = 1;
			(hom->myVec())[i]->move = (hom->myVec())[i]->move2;
		}
		if ((hom->myVec())[i]->homBounceTime <= gameTime) {
			for (int j = 0; j < water->mySize(); ++j) {
				// Water & Homing Collision
				if ((((hom->myVec())[i]->move == 1 && (hom->myVec())[i]->location.X - 1 <= (water->myVec())[j]->location.X) || ((hom->myVec())[i]->move == 2 && 
					(hom->myVec())[i]->location.X + 1 >= (water->myVec())[j]->location.X)|| 
					((hom->myVec())[i]->location.X == (water->myVec())[j]->location.X)) && 
					(hom->myVec())[i]->location.Y == (water->myVec())[j]->location.Y) 
				{
					(hom->myVec())[i]->active = 0;
					(hom->myVec())[i]->location = g_sChar.m_cLocation;
					(hom->myVec())[i]->renderLocation = g_sChar.renderLocation;
					(water->myVec())[j]->active = 0;
					(water->myVec())[j]->location = g_sChar.m_cLocation;
					(water->myVec())[j]->renderLocation = g_sChar.renderLocation;
					collided = 1;
					break;
				}
				else if (j == water->mySize() - 1) {
					switch ((hom->myVec())[i]->move) { //Homing bullet movement
						case 1: (hom->myVec())[i]->location.X -= 1 + ((hom->myVec())[i]->location.X - 3 != g_sChar.m_cLocation.X); break;
						case 2: (hom->myVec())[i]->location.X += 1 + ((hom->myVec())[i]->location.X + 3 != g_sChar.m_cLocation.X); break;
						case 3: --(hom->myVec())[i]->location.Y; break;
						case 4: ++(hom->myVec())[i]->location.Y;
					}
				}
			}
			if (collided) { //Continue with loop if collided
				continue;
			}
			(hom->myVec())[i]->homBounceTime = gameTime + (adjusted ? 0.25 : (hom->myVec())[i]->delay); //Bullet movement delay
			(hom->myVec())[i]->delay -= 0.001 * ((hom->myVec())[i]->delay > 0.05); //Bullet movement delay change
		}
	}

	//Spawning of fire bullets
	if (fireBounceTime <= gameTime) {
		Bullet *fireInstance = fire->getPooledObj();
		fireInstance->active = 1;
		fireInstance->dir = range(gen);
		fireBounceTime = gameTime + 0.001;
	}
	for (int i = 0; i < fire->mySize(); ++i) {
		if (!(fire->myVec())[i]->active) { //Fire bullet goes to boss location when it's not active
			(fire->myVec())[i]->location = { xPos, yPos };
		}
		for (int j = 0; j < water->mySize(); ++j) {
			//Fire bullets and water bullets collision
			if ((fire->myVec())[i]->location.X == (water->myVec())[j]->location.X && (fire->myVec())[i]->location.Y == (water->myVec())[j]->location.Y) {
				(water->myVec())[j]->active = 0;
				(water->myVec())[j]->location = g_sChar.m_cLocation;
				(water->myVec())[j]->renderLocation = g_sChar.renderLocation;
				continue;
			}
		}
		switch ((fire->myVec())[i]->dir) { //Movement of fire bullet based on its dir
		case 1: (fire->myVec())[i]->location.X -= 1 + ((fire->myVec())[i]->location.X - 3 != g_sChar.m_cLocation.X); break;
		case 2: (fire->myVec())[i]->location.X += 1 + ((fire->myVec())[i]->location.X + 3 != g_sChar.m_cLocation.X); break;
		case 3: (fire->myVec())[i]->location.Y -= 1; break;
		case 4: (fire->myVec())[i]->location.Y += 1; break;
		case 5: (fire->myVec())[i]->location.X -= 1 + ((fire->myVec())[i]->location.X - 3 != g_sChar.m_cLocation.X); (fire->myVec())[i]->location.Y -= 1; break;
		case 6: (fire->myVec())[i]->location.X -= 1 + ((fire->myVec())[i]->location.X - 3 != g_sChar.m_cLocation.X); (fire->myVec())[i]->location.Y += 1; break;
		case 7: (fire->myVec())[i]->location.X += 1 + ((fire->myVec())[i]->location.X + 3 != g_sChar.m_cLocation.X); (fire->myVec())[i]->location.Y -= 1; break;
		case 8: (fire->myVec())[i]->location.X += 1 + ((fire->myVec())[i]->location.X + 3 != g_sChar.m_cLocation.X); (fire->myVec())[i]->location.Y += 1; break;
		}
		//Fire bullet destruction when it leaves the game room
		if ((fire->myVec())[i]->location.X < 0 || (fire->myVec())[i]->location.Y < 0 || (fire->myVec())[i]->location.X > g_Console->getConsoleSize().X * 2 - 2 || (fire->myVec())[i]->location.Y > g_Console->getConsoleSize().Y * 2) {
			(fire->myVec())[i]->active = 0;
			(fire->myVec())[i]->location = { xPos, yPos };
		}
		// Fire & Player Collision
		if ((((fire->myVec())[i]->dir == 1 && (fire->myVec())[i]->location.X - 1 <= g_sChar.m_cLocation.X)
			|| ((fire->myVec())[i]->dir == 2 && (fire->myVec())[i]->location.X + 1 >= g_sChar.m_cLocation.X)
			|| (fire->myVec())[i]->location.X == g_sChar.m_cLocation.X)
			&& (fire->myVec())[i]->location.Y == g_sChar.m_cLocation.Y) 
		{
			--g_sChar.health;
			(fire->myVec())[i]->active = 0;
			(fire->myVec())[i]->location = { xPos, yPos };
		}
	}
}

void renderBoss() {
	//Rendering of homing bullets
	for (int i = 0; i < hom->mySize(); ++i) {
		int xLocation, yLocation;
		if (g_sChar.m_cLocation.X <= 98) {
			xLocation = (g_sChar.renderLocation.X % 2 ? 99 : 100);
		}
		else if (g_sChar.m_cLocation.X >= 302) {
			xLocation = (g_sChar.renderLocation.X % 2 ? 303 : 300);
		}
		else {
			xLocation = g_sChar.m_cLocation.X;
		}
		if (g_sChar.m_cLocation.Y < 25) {
			yLocation = 24;
		}
		else if (g_sChar.m_cLocation.Y > 76) {
			yLocation = 77;
		}
		else {
			yLocation = g_sChar.m_cLocation.Y;
		}
		(hom->myVec())[i]->renderLocation = { g_Console->getConsoleSize().X / 2 + SHORT((hom->myVec())[i]->location.X - xLocation), g_Console->getConsoleSize().Y / 2 + SHORT((hom->myVec())[i]->location.Y - yLocation) };
		if ((hom->myVec())[i]->renderLocation.X > 0 && (hom->myVec())[i]->renderLocation.Y > 0 && (hom->myVec())[i]->renderLocation.X < g_Console->getConsoleSize().X && (hom->myVec())[i]->renderLocation.Y < g_Console->getConsoleSize().Y) {
			if((hom->myVec())[i]->active){
				if (themeSelected != JAMESBONDTHEME) g_Console->writeToBuffer((hom->myVec())[i]->renderLocation, "  ", 0xFF);
				else g_Console->writeToBuffer((hom->myVec())[i]->renderLocation, "  ", 0x00);
			}
		}
	}
	//Rendering of fire bullets
	for (int i = 0; i < fire->mySize(); ++i) {
		int xLocation, yLocation;
		if (g_sChar.m_cLocation.X <= 98) {
			xLocation = (g_sChar.renderLocation.X % 2 ? 99 : 100);
		}
		else if (g_sChar.m_cLocation.X >= 302) {
			xLocation = (g_sChar.renderLocation.X % 2 ? 303 : 300);
		}
		else {
			xLocation = g_sChar.m_cLocation.X;
		}
		if (g_sChar.m_cLocation.Y <= 25) {
			yLocation = 24;
		}
		else if (g_sChar.m_cLocation.Y >= 75) {
			yLocation = 77;
		}
		else {
			yLocation = g_sChar.m_cLocation.Y;
		}
		(fire->myVec())[i]->renderLocation = { g_Console->getConsoleSize().X / 2 + SHORT((fire->myVec())[i]->location.X - xLocation), g_Console->getConsoleSize().Y / 2 + SHORT((fire->myVec())[i]->location.Y - yLocation) };
		if ((fire->myVec())[i]->renderLocation.X > 0 && (fire->myVec())[i]->renderLocation.Y > 0 && (fire->myVec())[i]->renderLocation.X < g_Console->getConsoleSize().X - 1 && (fire->myVec())[i]->renderLocation.Y < g_Console->getConsoleSize().Y) {
			if ((fire->myVec())[i]->active) {
				g_Console->writeToBuffer((fire->myVec())[i]->renderLocation, "  ", ThemeColours[themeSelected][8]);
			}
		}
	}

	//Rendering of boss
	COORD bossLocation = { xPos - 20, yPos - 15 }, bossRenderLocation = { 0, 0 };
	std::string line, txtRender;
	if (themeSelected == FIRETHEME) txtRender = "../src/SP1Framework/resources/fireBoss.txt";
	if (themeSelected == WATERTHEME) txtRender = "../src/SP1Framework/resources/oceanBoss.txt";
	if (themeSelected == SPACETHEME) txtRender = "../src/SP1Framework/resources/spaceBoss.txt";
	if (themeSelected == JAMESBONDTHEME) txtRender = "../src/SP1Framework/resources/BOSS.txt";
	std::ifstream inClientFile(txtRender, std::ios::in);
	if (!inClientFile) {
		//g_Console->writeToBuffer({ 100, 25 }, "You have no boss file.");
	}
	if (inClientFile.is_open()) {
		int y = 0;
		while (getline(inClientFile, line)) {
			for (unsigned int x = 0; x < line.length(); ++x) {
				if (line[x] != ' ') {
					int xLocation, yLocation;
					if (g_sChar.m_cLocation.X <= 98) {
						xLocation = 98;
					}
					else if (g_sChar.m_cLocation.X >= 302) {
						xLocation = 302;
					}
					else {
						xLocation = g_sChar.m_cLocation.X;
					}
					if (g_sChar.m_cLocation.Y <= 25) {
						yLocation = 25;
					}
					else if (g_sChar.m_cLocation.Y >= 75) {
						yLocation = 75;
					}
					else {
						yLocation = g_sChar.m_cLocation.Y;
					}
					bossRenderLocation = { SHORT(x) + g_Console->getConsoleSize().X / 2 + SHORT(bossLocation.X - xLocation), SHORT(y) + g_Console->getConsoleSize().Y / 2 + SHORT(bossLocation.Y - yLocation) };
					if (bossRenderLocation.X > 0 && bossRenderLocation.Y > 0 && bossRenderLocation.X < g_Console->getConsoleSize().X - 1 && bossRenderLocation.Y < g_Console->getConsoleSize().Y) {
						g_Console->writeToBuffer(bossRenderLocation, "  ", BossColours[themeSelected][line[x] - 49]);
					}
					for (int i = 0; i < water->mySize(); ++i) {
						if (x + bossLocation.X == (water->myVec())[i]->location.X && y + bossLocation.Y == (water->myVec())[i]->location.Y) {
							--bossHealth;

							(water->myVec())[i]->active = 0;
							(water->myVec())[i]->location = g_sChar.m_cLocation;
							(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
						}
					}
				}
			}
			y++;
		}
		inClientFile.close();
	}

	//Rendering of game room borders
	if (g_sChar.m_cLocation.Y <= 25) {
		for (int x = 0; x < 200; ++x) {
			g_Console->writeToBuffer({ SHORT(x), 0 }, '#', 0x0F);
		}
	}
	if (g_sChar.m_cLocation.X <= 98) {
		for (int y = 0; y < 50; ++y) {
			g_Console->writeToBuffer({ 0, SHORT(y) }, '#', 0x0F);
		}
	}
	if (g_sChar.m_cLocation.Y >= 75) {
		for (int x = 0; x < 200; ++x) {
			g_Console->writeToBuffer({ SHORT(x), 49 }, '#', 0x0F);
		}
	}
	if (g_sChar.m_cLocation.X >= 302) {
		for (int y = 0; y < 50; ++y) {
			g_Console->writeToBuffer({ 199, SHORT(y) }, '#', 0x0F);
		}
	}
}