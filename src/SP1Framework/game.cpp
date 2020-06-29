#include "game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <random>
#include <vector>

bool endless = 0;
int tileSizeX = 10, tileSizeY = 5, wallLimit = 2, score = 0, difficulty = 1, powerup = 0;
double gameTimePause, gameTimeStart, gameTime;//, mapTime, mapStartTime = 0.0;
extern SGameChar g_sChar; // For char

extern Console *g_Console;
extern WORD ThemeColours[4][9];
extern MUSIC music;
extern std::vector<int> column;
extern std::vector<std::vector<int>> levelMap;
extern bulletPool<Bullet> *water, *fire;
extern bulletPool<Homing> *hom;
extern double g_dElapsedTime, g_dDeltaTime, menuBounceTime, PowerBounceTime;
extern enum EGAMESTATES g_eGameState;
extern enum THEMES themeSelected;
extern bool gameInProgress;
extern int playerKeys[11], bossHealth;
extern SHORT xPos, yPos;

void init(){
	water->create(25);
	fire->create(200);
	hom->create(10);
	gameTimePause = 0.0;
	gameTimeStart = g_dElapsedTime;
	gameTime = g_dElapsedTime - gameTimePause - gameTimeStart;
	g_sChar.health = 10, score = 0;
	g_sChar.m_cLocation = {g_Console->getConsoleSize().X / 4, g_Console->getConsoleSize().Y / 2};
	g_Console->setConsoleFont(0, 16, L"Consolas"); //Set font width, height and name

	levelMap.clear(); column.clear();
	for(int i = 0; i < 10; ++i) column.push_back(0);
	for(int i = 0; i < 20; ++i) levelMap.push_back(column);	
}

void gameplay(){
	if (GetAsyncKeyState(playerKeys[10]) && menuBounceTime <= g_dElapsedTime) {
		menuBounceTime = g_dElapsedTime + 0.5;
		g_eGameState = S_PAUSE;
		music = M_ERASE;
	}
	gameTime = g_dElapsedTime - gameTimePause - gameTimeStart;
	if (g_eGameState == S_GAME) {
		//mapTime = gameTime - mapStartTime;
		generateMap();
		characterLogicMap();
		if (gameTime > (90.0)) {
			bossHealth = (difficulty * 150);
			xPos = g_Console->getConsoleSize().X;
			yPos = g_Console->getConsoleSize().Y;
			g_sChar.m_cLocation = { 5, 30 };
			g_sChar.renderLocation = { 5, 25 };
			for (int i = 0; i < water->mySize(); ++i) {
				(water->myVec())[i]->active = 0;
				(water->myVec())[i]->location = g_sChar.m_cLocation;
				(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
			}
			for (int i = 0; i < fire->mySize(); ++i) {
				(fire->myVec())[i]->active = 0;
				(fire->myVec())[i]->location = { SHORT(200), SHORT(50) };
				(fire->myVec())[i]->renderLocation = { SHORT(1), SHORT(1) };
			}
			for (int i = 0; i < hom->mySize(); ++i) {
				(hom->myVec())[i]->active = 0;
				(hom->myVec())[i]->location = { SHORT(200), SHORT(50) };
				(hom->myVec())[i]->renderLocation = { SHORT(1), SHORT(1) };
			}
			g_eGameState = S_BOSS;
		}
	}
	if (g_eGameState == S_BOSS) {
		moveCharacterBoss();
		if (bossHealth <= 0) {
			for (int i = 0; i < water->mySize(); ++i) {
				(water->myVec())[i]->active = 0;
				(water->myVec())[i]->location = g_sChar.m_cLocation;
				(water->myVec())[i]->renderLocation = g_sChar.renderLocation;
			}
			gameTimeStart = g_dElapsedTime;
			gameTimePause = 0.0;
			g_sChar.m_cLocation = { g_Console->getConsoleSize().X / 4, g_Console->getConsoleSize().Y / 2 };
			g_eGameState = S_GAME;
			levelMap.clear(); column.clear();
			for (int i = 0; i < 10; ++i) column.push_back(0);
			for (int i = 0; i < 20; ++i) levelMap.push_back(column);
		}
	}
}

void renderGame() {
	renderMap();
	renderObjects();
}

void renderCharacterBoss() {
	// Water render
	for (int i = 0; i < water->mySize(); ++i) {
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
		(water->myVec())[i]->renderLocation = { g_Console->getConsoleSize().X / 2 + SHORT((water->myVec())[i]->location.X - xLocation), g_Console->getConsoleSize().Y / 2 + SHORT((water->myVec())[i]->location.Y - yLocation) };
		if ((water->myVec())[i]->renderLocation.X > 0 && (water->myVec())[i]->renderLocation.Y > 0 && (water->myVec())[i]->renderLocation.X < g_Console->getConsoleSize().X - 1 && (water->myVec())[i]->renderLocation.Y < g_Console->getConsoleSize().Y) {
			if ((water->myVec())[i]->active) {
				g_Console->writeToBuffer((water->myVec())[i]->renderLocation, "  ", ThemeColours[themeSelected][7]);
			}
		}
	}
	/*for (int i = 0; i < water->mySize(); ++i) {
		if ((water->myVec())[i]->active) {
			g_Console->writeToBuffer((water->myVec())[i]->renderLocation, "  ", ThemeColours[themeSelected][7]);*/

	g_Console->writeToBuffer(g_sChar.renderLocation, "  ", ThemeColours[themeSelected][3]);
	std::ostringstream ss;
	ss << water->mySize();
	g_Console->writeToBuffer({ g_Console->getConsoleSize().X - 9, 2 }, ss.str());

	// Display health
	g_Console->writeToBuffer({ SHORT(89), SHORT(1) }, "HP ", ThemeColours[themeSelected][4]);
	for (int i = 0; i < 10; ++i)g_Console->writeToBuffer({ SHORT(92 + 2 * i), SHORT(1) }, "  ", ThemeColours[themeSelected][4]);
	for (int i = 0; i < g_sChar.health; ++i) g_Console->writeToBuffer({ SHORT(92 + 2 * i), SHORT(1) }, "  ", ThemeColours[themeSelected][1]);
	
	// Display boss health
	g_Console->writeToBuffer({ SHORT(43), SHORT(47) }, "Boss Health: ", ThemeColours[themeSelected][4]);
	//for (int i = 0; i < 10; ++i)g_Console->writeToBuffer({ SHORT(57 + 15 * i), SHORT(47) }, "               ", ThemeColours[themeSelected][4]);
	for (int i = 0; i < (bossHealth/8); ++i) g_Console->writeToBuffer({ SHORT(57 + 3*i), SHORT(47) }, "   ", ThemeColours[themeSelected][1]);

	// Display Score
	ss.str("");
	ss << "Score: " << score;
	g_Console->writeToBuffer({ SHORT(95), SHORT(3) }, ss.str(), 0x0F);

}

void renderObjects(){
	for(int i = 0; i < water->mySize(); ++i) 
		if((water->myVec())[i]->active) 
			g_Console->writeToBuffer((water->myVec())[i]->location, "  ", ThemeColours[themeSelected][7]);
	
	// Display Character
	if ((PowerBounceTime >= g_dElapsedTime + 0.5 && PowerBounceTime <= g_dElapsedTime + 0.6) ||
		(PowerBounceTime >= g_dElapsedTime + 0.3 && PowerBounceTime <= g_dElapsedTime + 0.4) || 
		(PowerBounceTime >= g_dElapsedTime + 0.1 && PowerBounceTime <= g_dElapsedTime + 0.2))
		g_Console->writeToBuffer(g_sChar.m_cLocation, "  ", ThemeColours[themeSelected][3]);
	else if (PowerBounceTime >= g_dElapsedTime) g_Console->writeToBuffer(g_sChar.m_cLocation, "  ", ThemeColours[themeSelected][6]);
	else g_Console->writeToBuffer(g_sChar.m_cLocation, "  ", ThemeColours[themeSelected][3]);
	
	// Display health
	g_Console->writeToBuffer({ SHORT(89), SHORT(1) }, "HP ", ThemeColours[themeSelected][4]);
	for (int i = 0; i < 10; ++i)g_Console->writeToBuffer({ SHORT(92 + 2 * i), SHORT(1) }, "  ", ThemeColours[themeSelected][4]);
	for (int i = 0; i < g_sChar.health; ++i) g_Console->writeToBuffer({ SHORT(92 + 2*i), SHORT(1) }, "  ", ThemeColours[themeSelected][1]);
	
	// Display Score
	std::ostringstream ss; ss << "Score: " << score;
	g_Console->writeToBuffer({ SHORT(95), SHORT(3) }, ss.str(), 0x0F);
}

void renderDebugInfo(){
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(3);
	
	ss.str(""); ss << "Elapsed Time: " << g_dElapsedTime;
	g_Console->writeToBuffer({ 0, 0 }, ss.str(), 0x59);
	
	ss.str(""); ss << "Game Time:    " << gameTime;
	g_Console->writeToBuffer({ 0, 1 }, ss.str(), 0x59);
	
	ss.str(""); ss << "Pause Time:   " << gameTimePause;
	g_Console->writeToBuffer({ 0, 2 }, ss.str(), 0x59);

	ss.str(""); ss << "Char Tile X Pos:   " << g_sChar.m_cLocation.X / 10 + int(gameTime * 2);
	g_Console->writeToBuffer({ 25, 0 }, ss.str(), 0x59);
	
	ss.str(""); ss << "Char Tile Y Pos:   " << g_sChar.m_cLocation.Y / 5;
	g_Console->writeToBuffer({ 25, 1 }, ss.str(), 0x59);

	ss.str(""); ss << "Char Block Offset: " << (g_sChar.m_cLocation.X - 10) % 10;
	g_Console->writeToBuffer({ 25, 2 }, ss.str(), 0x59);

	ss.str(""); ss << "PowerUp: " << powerup;
	g_Console->writeToBuffer({ 50, 1 }, ss.str(), 0x59);

	ss.str(""); ss << "Render Block Offset: " << (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX;
	g_Console->writeToBuffer({ 50, 2 }, ss.str(), 0x59); 

	ss.str(""); ss << 1.0 / g_dDeltaTime << "FPS";
	g_Console->writeToBuffer({g_Console->getConsoleSize().X - 9, 0}, ss.str(), 0x59);
}