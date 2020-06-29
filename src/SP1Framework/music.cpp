#include "game.h"
#include <sstream>
#include <string>

MUSIC music = M_SPLASH, curPlaying = M_SPLASH;
SFX sfx = SFX_BLANK;
double sfxBounceTime;
extern int volMaster, volMusic, volSFX, playerKeys[11];
extern double g_dElapsedTime;
extern EGAMESTATES g_eGameState;

void musicPlayer() {
	if (volMaster != 0) {
		switch (music) {
		case M_SPLASH:
			mciSendStringA("seek \"../src/SP1Framework/resources/M_Splash.wav\"to start", NULL, 100, NULL);
			mciSendStringA("play \"../src/SP1Framework/resources/M_Splash.wav\"", NULL, 0, NULL);

			break;

		case M_MAINMENU:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_MainMenu_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_MainMenu_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_MainMenu_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_MainMenu_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_MainMenu_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_MainMenu_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_FIRETHEME:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_FireTheme_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_FireTheme_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_FireTheme_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_FireTheme_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_FireTheme_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_FireTheme_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_WATERTHEME:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_WaterTheme_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_WaterTheme_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_WaterTheme_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_WaterTheme_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_WaterTheme_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_WaterTheme_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_SPACETHEME:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_SpaceTheme_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_SpaceTheme_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_SpaceTheme_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_SpaceTheme_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_SpaceTheme_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_SpaceTheme_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_JAMESBONDTHEME:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_JamesBondTheme_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_JamesBondTheme_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_JamesBondTheme_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_JamesBondTheme_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_JamesBondTheme_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_JamesBondTheme_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_GAMEOVER:
			if (volMaster != 0 && volMusic != 0) {
				if (volMaster == 1 || volMusic == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_GameOver_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_GameOver_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volMusic == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_GameOver_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_GameOver_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volMusic == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/M_GameOver_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/M_GameOver_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case M_ERASE:
			mciSendStringA("stop \"../src/SP1Framework/resources/M_Splash.wav\"", NULL, 0, NULL);

			mciSendStringA("stop \"../src/SP1Framework/resources/M_MainMenu_Soft.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_FireTheme_Soft.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_WaterTheme_Soft.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_SpaceTheme_Soft.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_JamesBondTheme_Soft.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_GameOver_Soft.wav\"", NULL, 0, NULL);
			
			mciSendStringA("stop \"../src/SP1Framework/resources/M_MainMenu_Normal.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_FireTheme_Normal.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_WaterTheme_Normal.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_SpaceTheme_Normal.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_JamesBondTheme_Normal.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_GameOver_Normal.wav\"", NULL, 0, NULL);
			
			mciSendStringA("stop \"../src/SP1Framework/resources/M_MainMenu_Loud.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_FireTheme_Loud.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_WaterTheme_Loud.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_SpaceTheme_Loud.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_JamesBondTheme_Loud.wav\"", NULL, 0, NULL);
			mciSendStringA("stop \"../src/SP1Framework/resources/M_GameOver_Loud.wav\"", NULL, 0, NULL);

			music = M_BLANK;
			break;
		};
		switch (sfx) {
		case SFX_MENUDING:
				mciSendStringA("seek \"../src/SP1Framework/resources/SFX_MenuDing.wav\"to start", NULL, 100, NULL);
				mciSendStringA("play \"../src/SP1Framework/resources/SFX_MenuDing.wav\"", NULL, 0, NULL);
			break;
			
		case SFX_BUTTONSELECT:
			if (volMaster != 0 && volSFX != 0) {
				if (volMaster == 1 || volSFX == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_ButtonSelect_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_ButtonSelect_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volSFX == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_ButtonSelect_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_ButtonSelect_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volSFX == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_ButtonSelect_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_ButtonSelect_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case SFX_EXPLOSION1:
			if (volMaster != 0 && volSFX != 0) {
				if (volMaster == 1 || volSFX == 1) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_Explosion_Soft.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_Explosion_Soft.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 2 || volSFX == 2) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_Explosion_Normal.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_Explosion_Normal.wav\"", NULL, 0, NULL);
				}
				else if (volMaster == 3 || volSFX == 3) {
					mciSendStringA("seek \"../src/SP1Framework/resources/SFX_Explosion_Loud.wav\"to start", NULL, 100, NULL);
					mciSendStringA("play \"../src/SP1Framework/resources/SFX_Explosion_Loud.wav\"", NULL, 0, NULL);
				}
			}
			break;

		case SFX_ERASE:
			sfxBounceTime = g_dElapsedTime + 0.5;
			if (sfxBounceTime <= g_dElapsedTime) {

				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_MenuDing_Soft.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_ButtonSelect_Soft.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_Explosion_Soft.wav\"", NULL, 0, NULL);

				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_MenuDing_Normal.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_ButtonSelect_Normal.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_Explosion_Normal.wav\"", NULL, 0, NULL);
		
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_MenuDing_Loud.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_ButtonSelect_Loud.wav\"", NULL, 0, NULL);
				mciSendStringA("stop \"../src/SP1Framework/resources/SFX_Explosion_Loud.wav\"", NULL, 0, NULL);
				
				sfx = SFX_BLANK;
			}
			break;
		};
	}
}