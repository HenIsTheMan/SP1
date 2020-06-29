#include "game.h"
#include <conio.h>
#include <fstream>
#include <sstream>
#include <string>

bool menuStart = 0;
int buttonSelect = 0, altButtonSelect = 0, menuStartTime, volMaster = 3, volMusic = 3, volSFX = 3;
double menuBounceTime = 0.0;
bulletPool<Bullet> *water = new bulletPool<Bullet>, *fire = new bulletPool<Bullet>;
bulletPool<Homing> *hom = new bulletPool<Homing>;

extern SHORT xPos, yPos;
extern THEMES themeSelected;
extern MUSIC music, curPlaying;
extern SFX sfx;
extern double g_dElapsedTime, gameTimeStart, gameTimePause, gameTime;
extern enum EGAMESTATES g_eGameState;
extern int difficulty, playerKeys[11], defaultPlayerKeys[11], score, gameInProgress;
extern bool debugInfo, endless;
extern WORD ThemeColours[4][9], BackgroundColours[4][7];
extern SGameChar g_sChar;
extern Console *g_Console;

void splashScreenWait() {
	music = M_SPLASH;
	if (g_dElapsedTime > 11.0 || _kbhit()) {
		menuStart = 1;
		menuBounceTime = g_dElapsedTime + 0.005;
		g_eGameState = S_MENU;
		music = M_ERASE;
	}
}

void menu() {
	if (menuBounceTime <= g_dElapsedTime && !menuStart) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// LEFT
		else if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			--altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		else if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			++altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			switch (buttonSelect) {
			case 0:
				altButtonSelect = 0;
				g_eGameState = S_DIFFICULTY;
				break;
			case 1: g_eGameState = S_HOWTOPLAY; break;
			case 2: g_eGameState = S_OPTIONS;	break;
			case 3: g_bQuitGame = true;			break;
			}
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}

	if (buttonSelect > 3) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 3;
	if (altButtonSelect > 2) altButtonSelect = 0;
	if (altButtonSelect < 0) altButtonSelect = 2;

	if (!menuStart) music = M_MAINMENU;
}

void difficultySelect() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			++altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			--altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// LEFT
		else if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			if (altButtonSelect == 0) sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		else if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			if (altButtonSelect == 0) sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			if (altButtonSelect == 1)
				if (endless) endless = 0; else endless = 1;
			else {
				music = M_ERASE;
				musicPlayer();
				init();
				switch (buttonSelect) {
				case 0: difficulty = 1; break;
				case 1: difficulty = 2; break;
				case 2: difficulty = 3; break;
				};
				switch (themeSelected) {
				case FIRETHEME: music = M_FIRETHEME; break;
				case WATERTHEME: music = M_WATERTHEME; break;
				case SPACETHEME: music = M_SPACETHEME; break;
				case JAMESBONDTHEME: music = M_JAMESBONDTHEME; break;
				};
				g_eGameState = S_GAME;
			}
			menuBounceTime = g_dElapsedTime + 0.15;
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			if (gameInProgress == 1 || gameInProgress == 2) g_eGameState = S_PAUSE;
			else g_eGameState = S_MENU;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}
	if (buttonSelect > 2) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 2;
	if (altButtonSelect > 1) altButtonSelect = 0;
	if (altButtonSelect < 0) altButtonSelect = 1;
}

void HowToPlay() {
	if (menuBounceTime <= g_dElapsedTime) {
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			g_eGameState = S_MENU;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}
}

void options() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			switch (buttonSelect) {
			case 0: g_eGameState = S_THEME; break;
			case 1: g_eGameState = S_CONTROLS; break;
			case 2: g_eGameState = S_SOUND; break;
			case 3:
				if (debugInfo) debugInfo = 0; else debugInfo = 1;
				break;
			}
			menuBounceTime = g_dElapsedTime + 0.15;
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			if (gameInProgress == 1 || gameInProgress == 2) g_eGameState = S_PAUSE;
			else g_eGameState = S_MENU;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}

	if (buttonSelect > 3) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 3;
}

void controls() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			--altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			++altButtonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// LEFT
		else if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		else if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			if (buttonSelect + (4 * altButtonSelect) == 11) {
				for (int i = 0; i < 11; ++i) playerKeys[i] = defaultPlayerKeys[i];
			}
			else {
				while (GetAsyncKeyState(int(toupper(_getch()))) != GetAsyncKeyState(playerKeys[9])) {
					playerKeys[buttonSelect + (4 * altButtonSelect)] = GetAsyncKeyState(int(toupper(_getch())));
				}
				menuBounceTime = g_dElapsedTime + 0.15;
			}
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			g_eGameState = S_OPTIONS;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}

	if (buttonSelect > 3) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 3;
	if (altButtonSelect > 2) altButtonSelect = 0;
	if (altButtonSelect < 0) altButtonSelect = 2;
}

void theme() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// LEFT
		if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			switch (buttonSelect) {
			case 0: themeSelected = FIRETHEME;	break;
			case 1: themeSelected = WATERTHEME; break;
			case 2: themeSelected = SPACETHEME; break;
			case 3: themeSelected = JAMESBONDTHEME; break;
			};
			menuBounceTime = g_dElapsedTime + 0.15;
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			g_eGameState = S_OPTIONS;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}
	if (buttonSelect > 3) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 3;
}

void sound() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// LEFT
		if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			switch (buttonSelect) {
			case 0:
				--volMaster;
				break;
			case 1:
				--volMusic;
				break;
			case 2:
				--volSFX;
				break;
			};

			menuBounceTime = g_dElapsedTime + 0.15;
			if (volMaster > 3) volMaster = 3;
			if (volMaster < 0) volMaster = 0;
			if (volMusic > 3) volMusic = 3;
			if (volMusic < 0) volMusic = 0;
			if (volSFX > 3) volSFX = 3;
			if (volSFX < 0) volSFX = 0;
			
			music = M_ERASE;
			sfx = SFX_ERASE;
			musicPlayer();
			music = M_MAINMENU;
			sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			switch (buttonSelect) {
			case 0:
				++volMaster;
				break;
			case 1:
				++volMusic;
				break;
			case 2:
				++volSFX;
				break;
			};

			menuBounceTime = g_dElapsedTime + 0.15;
			if (volMaster > 3) volMaster = 3;
			if (volMaster < 0) volMaster = 0;
			if (volMusic > 3) volMusic = 3;
			if (volMusic < 0) volMusic = 0;
			if (volSFX > 3) volSFX = 3;
			if (volSFX < 0) volSFX = 0;
			
			music = M_ERASE;
			sfx = SFX_ERASE;
			musicPlayer();
			music = M_MAINMENU;
			sfx = SFX_BUTTONSELECT;
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			g_eGameState = S_OPTIONS;
			menuBounceTime = g_dElapsedTime + 0.15;
		}
	}
	if (buttonSelect > 2) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 2;
}

void gameOver() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// LEFT
		if (GetAsyncKeyState(playerKeys[2]) || GetAsyncKeyState(playerKeys[6])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// RIGHT
		if (GetAsyncKeyState(playerKeys[3]) || GetAsyncKeyState(playerKeys[7])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			music = M_ERASE;
			musicPlayer();
			menuBounceTime = g_dElapsedTime + 0.15;
			if (buttonSelect == 0) g_eGameState = S_MENU;
			else g_eGameState = S_DIFFICULTY;
		}
	}
	if (buttonSelect > 1) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 1;
}

void pause() {
	if (menuBounceTime <= g_dElapsedTime) {
		sfx = SFX_ERASE;
		// UP
		if (GetAsyncKeyState(playerKeys[0]) || GetAsyncKeyState(playerKeys[4])) {
			--buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// DOWN
		else if (GetAsyncKeyState(playerKeys[1]) || GetAsyncKeyState(playerKeys[5])) {
			++buttonSelect;
			menuBounceTime = g_dElapsedTime + 0.15;
			sfx = SFX_BUTTONSELECT;
		}
		// SELECT
		if (GetAsyncKeyState(playerKeys[9])) {
			switch (buttonSelect) {
			case 0:
				gameTimePause = g_dElapsedTime - gameTime - gameTimeStart;
				menuBounceTime = g_dElapsedTime + 0.15;
				music = M_ERASE;
				musicPlayer();
				switch (themeSelected) {
				case FIRETHEME: music = M_FIRETHEME; break;
				case WATERTHEME: music = M_WATERTHEME; break;
				case SPACETHEME: music = M_SPACETHEME; break;
				case JAMESBONDTHEME: music = M_JAMESBONDTHEME; break;
				};
				if (gameInProgress == 1) g_eGameState = S_GAME;
				else if (gameInProgress == 2) g_eGameState = S_BOSS;
				break;
			case 1: g_eGameState = S_DIFFICULTY; break;
			case 2: g_eGameState = S_OPTIONS;	 break;
			case 3: g_eGameState = S_MENU;		 break;
			}
			menuBounceTime = g_dElapsedTime + 0.15;
		}
		// BACK
		if (GetAsyncKeyState(playerKeys[10])) {
			gameTimePause = g_dElapsedTime - gameTime - gameTimeStart;
			menuBounceTime = g_dElapsedTime + 0.15;
			music = M_ERASE;
			musicPlayer();
			switch (themeSelected) {
			case FIRETHEME: music = M_FIRETHEME; break;
			case WATERTHEME: music = M_WATERTHEME; break;
			case SPACETHEME: music = M_SPACETHEME; break;
			case JAMESBONDTHEME: music = M_JAMESBONDTHEME; break;
			};
			if (gameInProgress == 1) g_eGameState = S_GAME;
			else if (gameInProgress == 2) g_eGameState = S_BOSS;
		}
	}

	if (buttonSelect > 3) buttonSelect = 0;
	if (buttonSelect < 0) buttonSelect = 3;
}

void renderSplashScreen() {
	std::ifstream splashTeam("../src/SP1Framework/resources/splashFire.txt", std::ios::in), splashGame("../src/SP1Framework/resources/splashFireHelm.txt", std::ios::in);
	if (g_dElapsedTime < 6.75) {
		if (splashTeam.is_open()) {
			std::string line;
			WORD colors[] = { 0x40, 0xE0, 0xF0 };
			int y = 0;
			while (getline(splashTeam, line)) {
				for (unsigned int x = 0; x < line.length(); ++x) {
					if (line[x] == 32) continue;
					if (g_dElapsedTime < 1.75) {
						switch (int(g_dElapsedTime * 4)) {
						case 4: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "Û", colors[line[x] - 49]); break;
						case 5: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "²", colors[line[x] - 49]); break;
						case 6: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "±", colors[line[x] - 49]); break;
						case 7: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "°", colors[line[x] - 49]); break;
						default: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, " ", 0x00); break;
						};
					}
					else if (g_dElapsedTime < 4.75) {
						switch (int(g_dElapsedTime * 4)) {
						case 16: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "°", colors[line[x] - 49]); break;
						case 17: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "±", colors[line[x] - 49]); break;
						case 18: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "²", colors[line[x] - 49]); break;
						case 19: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "Û", colors[line[x] - 49]); break;
						default: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, " ", colors[line[x] - 49]); break;
						};
					}
					/*if (g_dElapsedTime < 3.75) {
						switch (int(g_dElapsedTime * 4)) {
						case 12: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "°", colors[line[x] - 49]); break;
						case 13: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "±", colors[line[x] - 49]); break;
						case 14: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "²", colors[line[x] - 49]); break;
						case 15: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, "Û", colors[line[x] - 49]); break;
						default: g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 2) }, " ", colors[line[x] - 49]); break;
						};
					}*/
					else break;
				}
				y++;
			}
			splashTeam.close();
		}
		//g_Console->writeToBuffer({ SHORT(86), SHORT(47) }, "Press Enter for Quick Start", 0x0F);
	}
	else {
		std::string line;
		WORD colors[] = { 0x40, 0x60, 0x40, 0x80, 0xF0 };
		if (splashGame.is_open()) {
			int y = 0;
			while (getline(splashGame, line)) {
				for (unsigned int x = 0; x < line.length(); ++x) {
					if (line[x] == 32) continue;
					if (g_dElapsedTime < 7.5) {				
						switch (int(g_dElapsedTime * 4)) {
						case 27: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "Û", colors[line[x] - 49]); break;
						case 28: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "²", colors[line[x] - 49]); break;
						case 29: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "±", colors[line[x] - 49]); break;
						case 30: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "°", colors[line[x] - 49]); break;
						default: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, " ", colors[line[x] - 49]); break;
						};
					}
					else if (g_dElapsedTime < 9.75) {
						switch (int(g_dElapsedTime * 4)) {
						case 36: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "°", colors[line[x] - 49]); break;
						case 37: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "±", colors[line[x] - 49]); break;
						case 38: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "²", colors[line[x] - 49]); break;
						case 39: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, "Û", colors[line[x] - 49]); break;
						default: g_Console->writeToBuffer({ SHORT(x + 30), SHORT(y + 12) }, " ", colors[line[x] - 49]); break;
						};
					}
					else break;
				}
				y++;
			}
			splashGame.close();
		}
	}
}

void renderMenu() {
	// Menu Start
	if (menuStart) {
		if (menuBounceTime <= g_dElapsedTime) {
			++menuStartTime;
			menuBounceTime = g_dElapsedTime/* + menuStartTime / 40*/;
		}
		g_Console->writeToBuffer({ SHORT(menuStartTime), SHORT(16) }, "Start", 0x04);
		g_Console->writeToBuffer({ SHORT(menuStartTime), SHORT(22) }, "How To Play", 0x04);
		g_Console->writeToBuffer({ SHORT(menuStartTime), SHORT(28) }, "Options", 0x04);
		g_Console->writeToBuffer({ SHORT(menuStartTime), SHORT(34) }, "Quit", 0x04);

		if (menuStartTime >= 20) sfx = SFX_MENUDING;
		if (menuStartTime >= 24) {
			g_Console->clearBuffer(0xFF);
		}
		if (menuStartTime == 28) {
			menuStartTime = 0;
			sfx = SFX_ERASE;
			menuStart = 0;
		}
	}
	else {
		// Render Background
		renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

		// Render Buttons
		g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "Start", 0x0F);
		g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "How To Play", 0x0F);
		g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "Options", 0x0F);
		g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "Quit", 0x0F);
		if (buttonSelect == 0) g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "> Start <", ThemeColours[themeSelected][2]);
		if (buttonSelect == 1) g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "> How To Play <", ThemeColours[themeSelected][2]);
		if (buttonSelect == 2) g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "> Options <", ThemeColours[themeSelected][2]);
		if (buttonSelect == 3) g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "> Quit <", ThemeColours[themeSelected][2]);
		
		// Render Highscore Table
		std::ifstream scoretable("../src/SP1Framework/resources/highscoreTable.txt", std::ios::in);
		if (scoretable.is_open()) {
			std::string line;
			int y = 0;
			while (getline(scoretable, line)) {
				for (unsigned int x = 0; x < line.length(); ++x) g_Console->writeToBuffer({ SHORT(x + 110), SHORT(y + 5) }, line[x], 0x0F);
				y++;
			}
			scoretable.close();
		}
		std::string iString;
		if (altButtonSelect == 0) {
			g_Console->writeToBuffer({ SHORT(148), SHORT(8) }, " Easy ", 0x0F);
			g_Console->writeToBuffer({ SHORT(117), SHORT(7) }, "Hard", 0x0F);
			g_Console->writeToBuffer({ SHORT(180), SHORT(7) }, "Normal", 0x0F);
			iString = "../src/SP1Framework/resources/highscoreEasy.txt";
		}
		if (altButtonSelect == 1) {
			iString = "../src/SP1Framework/resources/highscoreNormal.txt";
		}
		if (altButtonSelect == 2) {
			g_Console->writeToBuffer({ SHORT(148), SHORT(8) }, " Hard ", 0x0F);
			g_Console->writeToBuffer({ SHORT(116), SHORT(7) }, "Normal", 0x0F);
			g_Console->writeToBuffer({ SHORT(181), SHORT(7) }, "Easy", 0x0F);
			iString = "../src/SP1Framework/resources/highscoreHard.txt";

		}
		std::ifstream highscoreDisplay(iString, std::ios::in);
		if (highscoreDisplay.is_open()) {
			std::string line;
			int y = 0;
			while (getline(highscoreDisplay, line)) {
				for (unsigned int x = 0; x < line.length(); ++x) {
					if (x < 20) g_Console->writeToBuffer({ SHORT(x + 126), SHORT(y + 13) }, line[x], 0x0F);
					else g_Console->writeToBuffer({ SHORT(x + 144), SHORT(y + 13) }, line[x], 0x0F);
				}
				y += 2;
			}
			highscoreDisplay.close();
		}
	}
}

void renderDifficultySelect() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", {SHORT(0), SHORT(0)}, BackgroundColours);

	// Placeholders
	for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(50), SHORT(16 + i) }, "        Easy        ", 0x20);
	for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(90), SHORT(16 + i) }, "       Normal       ", 0x60);
	for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(130), SHORT(16+ i) }, "        Hard        ", 0x40);
	if (endless) g_Console->writeToBuffer({ SHORT(90), SHORT(40) }, "Endless Mode: ENABLED", 0x0F);
	else g_Console->writeToBuffer({ SHORT(90), SHORT(40) }, "Endless Mode: DISABLED", 0x0F);
	if (altButtonSelect == 1) {
		if (endless) g_Console->writeToBuffer({ SHORT(90), SHORT(40) }, "Endless Mode: ENABLED", ThemeColours[themeSelected][2]);
		else g_Console->writeToBuffer({ SHORT(90), SHORT(40) }, "Endless Mode: DISABLED", ThemeColours[themeSelected][2]);
	}
	else if (buttonSelect == 0) for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(49), SHORT(16 + i) }, ">        Easy        <", 0x2F);
	else if (buttonSelect == 1) for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(89), SHORT(16 + i) }, ">       Normal       <", 0x6F);
	else if (buttonSelect == 2) for (int i = 0; i < 10; ++i) g_Console->writeToBuffer({ SHORT(129), SHORT(16 + i) },">        Hard        <", 0x4F);
}

void renderHowToPlay() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Text
	g_Console->writeToBuffer({ SHORT(10), SHORT(2) }, "HOW TO PLAY", 0x0F);
	g_Console->writeToBuffer({ SHORT(10), SHORT(3) }, "=============", 0x0F);
	g_Console->writeToBuffer({ SHORT(10), SHORT(4) }, "> The objective of the game is simple. Survive the fire, save as many civilians, defeat the boss. Be the man, be the Fireman.", 0x0F);
	g_Console->writeToBuffer({ SHORT(10), SHORT(6) }, "> Move your character by using", 0x0F);
	g_Console->writeToBuffer({ SHORT(41), SHORT(6) }, "WASD Keys.", 0x0A);
	g_Console->writeToBuffer({ SHORT(10), SHORT(8) }, "> Shoot water bullets by using the            in the specific direction.", 0x0F);
	g_Console->writeToBuffer({ SHORT(45), SHORT(8) }, "arrow keys", 0x0A);
	g_Console->writeToBuffer({ SHORT(10), SHORT(10) }, "> Sprint by holding       and pressing the movement key in the specific direction.", 0x0F);
	g_Console->writeToBuffer({ SHORT(30), SHORT(10) }, "Shift", 0x0A);
	g_Console->writeToBuffer({ SHORT(10), SHORT(12) }, "> Press     to pause the game.", 0x0F);
	g_Console->writeToBuffer({ SHORT(18), SHORT(12) }, "Esc", 0x0A);
	g_Console->writeToBuffer({ SHORT(10), SHORT(14) }, "> Toggle your controls under                .", 0x0F);
	g_Console->writeToBuffer({ SHORT(39), SHORT(14) }, "Options/Controls.", 0x0A);
}

void renderOptions() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Buttons
	g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "Themes", 0x0F);
	g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "Controls", 0x0F);
	g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "Sound", 0x0F);
	if (debugInfo) g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "Debug Info: SHOWN", 0x0F);
	else g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "Debug Info: HIDDEN", 0x0F);
	if (buttonSelect == 0) g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "> Themes <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 1) g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "> Controls <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 2) g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "> Sound <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 3 && debugInfo) g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "> Debug Info: SHOWN <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 3 && !debugInfo) g_Console->writeToBuffer({ SHORT(24), SHORT(34) }, "> Debug Info: HIDDEN <", ThemeColours[themeSelected][2]);
}

void renderControls() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Buttons
	g_Console->writeToBuffer({ SHORT(50), SHORT(16) }, "Move Up: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(75), SHORT(16) }, "Move Down: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(16) }, "Move Left", 0x0F);
	g_Console->writeToBuffer({ SHORT(125), SHORT(16) }, "Move Right: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(50), SHORT(22) }, "Shoot Up: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(75), SHORT(22) }, "Shoot Down: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(22) }, "Shoot Left: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(125), SHORT(22) }, "Shoot Right: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(50), SHORT(28) }, "Sprint: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(75), SHORT(28) }, "Confirm: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(28) }, "Back: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(127), SHORT(28) }, "RESET", 0x4F);
	if (buttonSelect + (4 * altButtonSelect) == 0) g_Console->writeToBuffer({ SHORT(48), SHORT(16) }, "> Move Up: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 1) g_Console->writeToBuffer({ SHORT(73), SHORT(16) }, "> Move Down: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 2) g_Console->writeToBuffer({ SHORT(98), SHORT(16) }, "> Move Left", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 3) g_Console->writeToBuffer({ SHORT(123), SHORT(16) }, "> Move Right: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 4) g_Console->writeToBuffer({ SHORT(48), SHORT(22) }, "> Shoot Up: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 5) g_Console->writeToBuffer({ SHORT(73), SHORT(22) }, "> Shoot Down: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 6) g_Console->writeToBuffer({ SHORT(98), SHORT(22) }, "> Shoot Left: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 7) g_Console->writeToBuffer({ SHORT(123), SHORT(22) }, "> Shoot Right: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 8) g_Console->writeToBuffer({ SHORT(48), SHORT(28) }, "> Sprint: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 9) g_Console->writeToBuffer({ SHORT(73), SHORT(28) }, "> Confirm: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 10) g_Console->writeToBuffer({ SHORT(98), SHORT(28) }, "> Back: ", ThemeColours[themeSelected][2]);
	if (buttonSelect + (4 * altButtonSelect) == 11) g_Console->writeToBuffer({ SHORT(125), SHORT(28) }, "> RESET <", ThemeColours[themeSelected][2]);
}

void renderTheme() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Image
	std::string str;
	WORD colors[3] = {0, 0, 0};
	int x1 = 0, y1 = 0;
	switch (buttonSelect) {
	case 0: str = "../src/SP1Framework/resources/splashFire.txt";	colors[0] = 0x44; colors[1] = 0x66; colors[2] = 0x66; x1 = 65; y1 = 2;  break;
	case 1:	str = "../src/SP1Framework/resources/splashWater.txt";	colors[0] = 0x11; colors[1] = 0xBB; colors[2] = 0x66; x1 = 78; y1 = 5;  break;
	case 2:	str = "../src/SP1Framework/resources/splashSpace.txt";	colors[0] = 0x11; colors[1] = 0xBB; colors[2] = 0x65; x1 = 80; y1 = 12; break;
	case 3: str = "../src/SP1Framework/resources/splashGun.txt";	colors[0] = 0x00; colors[1] = 0x88; colors[2] = 0x66; x1 = 50; y1 = 5; break;
	};
	std::ifstream splashRender(str, std::ios::in), splashSBP("../src/SP1Framework/resources/splashSBP.txt", std::ios::in);
	if (splashRender.is_open()) {
		std::string line;
		int y = 0;
		while (getline(splashRender, line)) {
			for (unsigned int x = 0; x < line.length(); ++x) {
				if (line[x] == 32) continue;
				g_Console->writeToBuffer({ SHORT(x + x1), SHORT(y + y1) }, " ", colors[line[x] - 49]);
			}
			y++;
		}
		splashRender.close();
	}
	if (splashSBP.is_open()) {
		std::string line;
		int y = 0;
		while (getline(splashSBP, line)) {
			for (unsigned int x = 0; x < line.length(); ++x) {
				if (line[x] == 32) continue;
				g_Console->writeToBuffer({ SHORT(x + 65), SHORT(y + 35) }, " ", 0xAA);
			}
			y++;
		}
		splashSBP.close();
	}
}

void renderSound() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);
	
	// Render Sliders
	for (int i = 0; i < 4; ++i) 
		for (int j = 0; j < 18; j += 6) 
			g_Console->writeToBuffer({ SHORT(i*12 + 45), SHORT(j + 16) }, "           ", 0x66);

	g_Console->writeToBuffer({ SHORT(12 * volMaster + 45), SHORT(16) }, "           ", 0x88);
	g_Console->writeToBuffer({ SHORT(12 * volMusic + 45), SHORT(22) }, "           ", 0x88);
	g_Console->writeToBuffer({ SHORT(12 * volSFX + 45), SHORT(28) }, "           ", 0x88);

	// Render Buttons
	g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "Master Volume: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "Music Volume: ", 0x0F);
	g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "SFX Volume: ", 0x0F);
	if (buttonSelect == 0) g_Console->writeToBuffer({ SHORT(24), SHORT(16) }, "Master Volume: ", ThemeColours[themeSelected][2]);
	if (buttonSelect == 1) g_Console->writeToBuffer({ SHORT(24), SHORT(22) }, "Music Volume: ", ThemeColours[themeSelected][2]);
	if (buttonSelect == 2) g_Console->writeToBuffer({ SHORT(24), SHORT(28) }, "SFX Volume: ", ThemeColours[themeSelected][2]);
}

void renderGameOver() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Buttons
	g_Console->writeToBuffer({ SHORT(75), SHORT(40) }, "Main Menu", 0x0F);
	g_Console->writeToBuffer({ SHORT(115), SHORT(40) }, "Try Again", 0x0F);
	if (buttonSelect == 0) g_Console->writeToBuffer({ SHORT(73), SHORT(40) }, "> Main Menu <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 1) g_Console->writeToBuffer({ SHORT(113), SHORT(40) }, "> Try Again <", ThemeColours[themeSelected][2]);
}

void renderPause() {
	// Render Background
	renderTxt("../src/SP1Framework/resources/menuBackground.txt", { SHORT(0), SHORT(0) }, BackgroundColours);

	// Render Buttons
	g_Console->writeToBuffer({ SHORT(100), SHORT(8) }, "Resume Game", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(14) }, "Restart", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(20) }, "Options", 0x0F);
	g_Console->writeToBuffer({ SHORT(100), SHORT(26) }, "Return To Menu", 0x0F);
	if (buttonSelect == 0) g_Console->writeToBuffer({ SHORT(100), SHORT(8) }, "> Resume Game <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 1) g_Console->writeToBuffer({ SHORT(100), SHORT(14) }, "> Restart <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 2) g_Console->writeToBuffer({ SHORT(100), SHORT(20) }, "> Options <", ThemeColours[themeSelected][2]);
	if (buttonSelect == 3) g_Console->writeToBuffer({ SHORT(100), SHORT(26) }, "> Return To Menu <", ThemeColours[themeSelected][2]);
}