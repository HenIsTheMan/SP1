#pragma once
#include "Framework\timer.h"
#include "Framework\console.h"
#include <vector>

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

enum EGAMESTATES { S_SPLASHSCREEN, S_GAME, S_MENU, S_DIFFICULTY, S_PAUSE, S_OPTIONS, S_HOWTOPLAY, S_THEME, S_CONTROLS, S_SOUND, S_GAMEOVER, S_BOSS, S_HIGHSCORE };
enum MUSIC { M_SPLASH, M_MAINMENU, M_FIRETHEME, M_WATERTHEME, M_SPACETHEME, M_JAMESBONDTHEME, M_GAMEOVER, M_BLANK, M_ERASE };
enum SFX { SFX_MENUDING, SFX_BUTTONSELECT, SFX_PLAYERHIT, SFX_PLAYERSHOOT, SFX_PLAYERDAMAGE,SFX_EXPLOSION1, SFX_WATERBULLET, SFX_BLANK, SFX_ERASE };

struct SGameChar { COORD m_cLocation, renderLocation; int health; }; //For char

struct Bullet{ //For normal bullets
	bool active;
	int dir = 0;
	double bulletBounceTime = 0.0;
	COORD location = {0, 0}, renderLocation = {0, 0};
};

struct Homing{ //For homing bullet
	bool active;
	COORD location = {0, 0}, renderLocation = {0, 0};
	int move = 2, move1 = 3, move2 = 1;
	double homBounceTime = 0.0, delay = 0.20;
};

//Object Pooling
template<typename type>
class bulletPool{
	public: //Public access specifier
	void create(int amt){ //Creation of bullet pool with fixed amt of bullets
		amtToPool = amt;
		for(int i = 0; i < amtToPool; ++i){
			type *obj = (type*)(operator new(sizeof(type)));
			obj->active = 0;
			pool.push_back(obj);
		}
	}
	type *getPooledObj(){ //Get oldest inactive pool object to use
		if(pool[pool.size() - 1]->active){
			pool[oldest]->active = 0;
			oldest += 1 - amtToPool * (oldest == amtToPool - 1);
		}
		for(unsigned int i = 0; i < pool.size(); ++i){
			if(!pool[i]->active){
				return pool[i];
			}
		}
	}
	int mySize(){ //Get size of bullet pool
		return pool.size();
	}
	int *myOldest(){ //Get pool vector
		return &oldest;
	}
	const std::vector<type*> & myVec() const{
		return pool;
	}
	private: //Private access specifier
		std::vector<type*> pool; //Pool vector
		int amtToPool, oldest = 0; //Vars
};

enum THEMES { FIRETHEME, WATERTHEME, SPACETHEME, JAMESBONDTHEME };

enum DIFFICULTIES { EASY, MEDIUM, HARD };

void init(); //Init vars, allocate memory, load data from file, etc.
void update(double dt); //Update the game and its state
void render(); //Render current state of game to the console
void shutdown(); //Reset before exiting the program, clean up and free memory
void gameplay(); //Gameplay logic
void splashScreenWait(); //Wait for time to pass in splash screen
void generateMap(); //Generate map
void characterLogicMap(); //Move game char, collision detection, physics, etc.
void PowerUps();
void menu(); //Menu
void difficultySelect();
void pause(); //Pause
void options(); //Options
void HowToPlay(); //How To Play
void theme(); //Theme
void controls(); //Controls
void sound(); //Sound
void musicPlayer();
void gameOver(); //Gameover
void boss();
void moveCharacterBoss();
void newHighscore();
void renderSplashScreen(); //Render splash screen
void renderMap(); //Render map 1st
void renderObjects();
void renderCharacterBoss();
void renderDebugInfo(); //Render debug info
void renderMenu(); //Render menu
void renderDifficultySelect();
void renderGame(); //Render Game
void renderPause(); //Render pause
void renderOptions(); //Render options
void renderHowToPlay(); //Renders How To Play
void renderTheme(); //Render Theme
void renderControls(); //Render Controls
void renderSound(); //Render Sound
void renderGameOver(); // Render Game Over
void renderBoss();
void renderTxt(std::string filename, COORD c, WORD colours[4][7]);
const std::string currentDateTime();