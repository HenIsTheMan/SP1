#include "game.h"
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

Console *g_Console = new Console(200, 50, "One Man, Fireman");
CStopWatch g_Timer; //Keep track of time and frame rate
bool g_bQuitGame = 0, debugInfo = 0;
int gameInProgress = 0;
double g_dElapsedTime = 0.0, g_dDeltaTime = 0.0;
const unsigned char gc_ucFPS = 100; //FPS
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS; //Time for each frame

extern int difficulty, score;

EGAMESTATES g_eGameState = S_SPLASHSCREEN;
THEMES themeSelected = FIRETHEME;

WORD ThemeColours[4][9] = { {0x0F, 0x44, 0xF4, 0x66, 0x0F, 0x80, 0x40, 0x11, 0x44}, //Health, HealthBar, MenuButton, Player, GBackground, Obs, DObs, Bullet, BBullet 
							{0x0F, 0xDD, 0xF1, 0x22, 0x1F, 0x50, 0xD0, 0x66, 0x22},
							{0x0F, 0xBB, 0xF8, 0xFF, 0x5F, 0x00, 0xB0, 0xBB, 0x66},
							{0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x60, 0x00, 0x00, 0x88} };
WORD BackgroundColours[4][7] = {{ 0x00, 0x88, 0xFF, 0x00, 0x44, 0x00, 0x44 },
								{ 0x11, 0xBB, 0xFF, 0x11, 0x66, 0x11, 0x66 },
								{ 0x55, 0x88, 0xFF, 0x55, 0xDD, 0x55, 0xDD },
								{ 0xFF, 0x66, 0x00, 0xFF, 0x88, 0xFF, 0x00 }};
WORD BossColours[4][4] = { { 0x40, 0x10, 0x60, 0xF0 },
							{ 0xBB, 0x11, 0x11, 0xF0 },
							{ 0x55, 0xBB, 0xFF, 0x00 },
							{ 0xF0, 0x00, 0x40, 0x80 }};

int defaultPlayerKeys[11] = { 87, 83, 65, 68, 16, 38, 40, 37, 39, 13, 27 },
	playerKeys[11] = { 87, 83, 65, 68, 38, 40, 37, 39, 16, 13, 27 }; // 0 UP, 1 DOWN, 2 LEFT , 3 RIGHT, 4 S_UP, 5 S_DOWN, 6 S_LEFT , 7 S_RIGHT, 8 SPRINT, 9 CONFIRM, 10 BACK

void update(double dt) { //Do not write to the console here
	g_dElapsedTime += dt;
	g_dDeltaTime = dt;
	switch (g_eGameState) {
	case S_SPLASHSCREEN: splashScreenWait(); break;
	case S_MENU: 
		gameInProgress = 0; 
		menu();
		break;
	case S_DIFFICULTY:	 difficultySelect(); break;
	case S_GAME:
		gameInProgress = 1;
		gameplay();
		break;
	case S_PAUSE:		 pause();			 break;
	case S_HOWTOPLAY:	 HowToPlay();		 break;
	case S_OPTIONS:		 options();			 break;
	case S_THEME:		 theme();			 break;
	case S_CONTROLS:	 controls();		 break;
	case S_SOUND:		 sound();			 break;
	case S_GAMEOVER:	 
		gameInProgress = 0;
		gameOver();		 
		break;
	case S_HIGHSCORE:	 newHighscore();	 break;
	case S_BOSS:
		gameInProgress = 2;
		boss(); 
		gameplay(); 
		break;
	}
	musicPlayer();
}

void render() {
	switch (g_eGameState) {
	case S_SPLASHSCREEN: g_Console->clearBuffer(0x00); renderSplashScreen();		break;
	case S_MENU:		 g_Console->clearBuffer(0x00); renderMenu();				break;
	case S_DIFFICULTY:	 g_Console->clearBuffer(0x00); renderDifficultySelect();	break;
	case S_GAME:		 g_Console->clearBuffer(ThemeColours[themeSelected][4]); renderGame(); break;
	case S_PAUSE:		 g_Console->clearBuffer(0x00); renderPause();			break;
	case S_HOWTOPLAY:	 g_Console->clearBuffer(0x00); renderHowToPlay();		break;
	case S_OPTIONS:		 g_Console->clearBuffer(0x00); renderOptions();			break;
	case S_THEME:		 g_Console->clearBuffer(0x00); renderTheme();			break;
	case S_CONTROLS:	 g_Console->clearBuffer(0x00); renderControls();			break;
	case S_SOUND:		 g_Console->clearBuffer(0x00); renderSound();			break;
	case S_GAMEOVER:	 g_Console->clearBuffer(0x00); renderGameOver();			break;
	case S_BOSS:		 g_Console->clearBuffer(ThemeColours[themeSelected][4]); renderBoss();	renderCharacterBoss();	  break;
	}
	if(debugInfo) renderDebugInfo();
	g_Console->flushBufferToConsole();
}

void mainLoop(){ //Call... at a specific frame rate
	g_Timer.startTimer(); //Start timer to calculate how long it takes to render frame
	while(!g_bQuitGame){
		update(g_Timer.getElapsedTime());
		render();
		g_Timer.waitUntil(gc_uFrameTime); //Limit each frame to a specified time in ms //BUG (waits for time from getElapsedTime to waitUntil, insignificant)!!!
	}
}

void shutdown() {
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); //Reset to white text on black bg
	g_Console->clearBuffer();
}

int main(){
	mainLoop();
	shutdown();
	return 0;
}

void renderTxt(std::string filename, COORD c, WORD colours[4][7]) { 
	std::ifstream background(filename, std::ios::in);
	if (background.is_open()) {
		std::string line;
		int y = 0;
		while (getline(background, line)) {
			for (unsigned int x = 0; x < line.length(); ++x) g_Console->writeToBuffer({ SHORT(x + c.X), SHORT(y + c.Y) }, line[x], colours[themeSelected][line[x] - 49]);
			y++;
		}
		background.close();
	}
}

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void newHighscore() {
	std::string iString;
	switch (difficulty) {
	case 1: iString = "../src/SP1Framework/resources/highscoreEasy.txt";	break;
	case 2: iString = "../src/SP1Framework/resources/highscoreNormal.txt";	break;
	case 3: iString = "../src/SP1Framework/resources/highscoreHard.txt";	break;
	};
	std::ifstream highscoreAccess(iString, std::ios::in);
	if (highscoreAccess.is_open()) {
		std::string line, scoreStr, highscoreStr[16] = { "", };
		bool newhigh = 0;
		int highscore[16] = { 0, }, newHighscore[16] = { 0, }, i = 0;
		while (getline(highscoreAccess, line)) {
			for (int x = 0; x < 20; ++x) highscoreStr[i] += line[x];
			for (unsigned int x = 20; x < line.length(); ++x) highscore[i] += (line[x] - 48) * (int)pow(10, (line.length() - x - 1));
			highscoreStr[i] += std::to_string(highscore[i]);
			if (score >= highscore[i] && !newhigh) {
				scoreStr = currentDateTime();
				scoreStr += " ";
				scoreStr += std::to_string(score);
				highscoreStr[i + 1] = highscoreStr[i];
				highscoreStr[i] = scoreStr;
				++i;
				newhigh = 1;
			}
			++i;
		}
		if (newhigh) {
			// Delete data in file
			std::ofstream ofs;
			ofs.open(iString, std::ofstream::out | std::ofstream::trunc);
			ofs.close();

			// New Data
			std::ofstream myfile;
			myfile.open(iString);
			for (int i = 0; i < 15; ++i) myfile << highscoreStr[i] << "\n";
			myfile.close();
		}
		highscoreAccess.close();
	}
}