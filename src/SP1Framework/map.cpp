#include "game.h"
#include <vector>
#include <random>

bool inv = 0;
std::vector<int> column, invColumn;
std::vector<std::vector<int>> levelMap;

extern int tileSizeX, tileSizeY, wallLimit, score, difficulty, powerup;
extern double g_dElapsedTime, gameTimeStart, gameTimePause, gameTime, PowerBounceTime, PowerDisplayTime;
extern SGameChar g_sChar;
extern MUSIC music;
extern SFX sfx;
extern THEMES themeSelected;
extern WORD ThemeColours[4][9];
extern Console *g_Console;

void generateMap() {
	int Pchance = 10; // 1/10 to be powerup
	int range = 2 + ((levelMap.size() % 10 == 0) * Pchance), walls = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	column.clear();
	for (int j = 0; j < 10; ++j) {
		if (j == 0 || j == 9) {
			column.push_back(0);
			continue;
		}
		std::uniform_int_distribution<> dist(0, range);
		column.push_back(dist(gen));
		if (column[j] >= 3) range = 2;
		if (column[j] == 2) ++walls;
		if (walls == wallLimit) range = 1;
	}
	if (inv) {
		invColumn.clear();
		for (int j = 0; j < 10; ++j) invColumn.push_back(column[j]);
		levelMap.push_back(invColumn);
		inv = 0;
	}
	else {
		levelMap.push_back(column);
		inv = 1;
	}
}

void renderMap() {
	const WORD colors[] = { 0x00, ThemeColours[themeSelected][6], ThemeColours[themeSelected][5], 0x20};
	//Collision Wall Player
	if  ((levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] == 2 &&				// Back Detect Tile
		((g_sChar.m_cLocation.X) % 10 == -1 *((10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||	// Back Detect Offset
		(g_sChar.m_cLocation.X) % 10 == -1 * ((10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX))) ||	// Back Detect Offset
		(levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] == 2 &&			// Front Detect Tile
		((g_sChar.m_cLocation.X) % 10 == (10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||			// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (11) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (12) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (13) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (14) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (15) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1)) * tileSizeX)) % tileSizeX ||			// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (16) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1)) * tileSizeX)) % tileSizeX ||			// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX)))			// Front Detect Offset

	/*if ((levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * 2)][(g_sChar.m_cLocation.Y) / 5] == 2 ||
		levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * 2) + 1][(g_sChar.m_cLocation.Y) / 5] == 2) &&
		((g_sChar.m_cLocation.X) % 10 == (10) - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX ||
		(g_sChar.m_cLocation.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX) && 
		(g_sChar.m_cLocation.Y % 5 == 4 || g_sChar.m_cLocation.Y % 5 == 0))*/
		/*((g_sChar.m_cLocation.Y) % 5 == 0 && 
		levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * 2)][(g_sChar.m_cLocation.Y) / 5] == 2))*/
	{
		if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] == 2) 
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] = 0;
		else if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] == 2) 
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] = 0;
		
		if (PowerBounceTime <= g_dElapsedTime) --g_sChar.health;
	}

	//Collision Fire Player
	if  ((levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] == 1 &&				// Back Detect Tile
		((g_sChar.m_cLocation.X) % 10 == -1 * ((10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||	// Back Detect Offset
		(g_sChar.m_cLocation.X) % 10 == -1 * ((11) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		(g_sChar.m_cLocation.X) % 10 == -1 * ((12) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		(g_sChar.m_cLocation.X) % 10 == -1 * ((13) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		(g_sChar.m_cLocation.X) % 10 == -1 * ((10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX))) ||	// Back Detect Offset
		(levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] == 1 &&			// Front Detect Tile
		((g_sChar.m_cLocation.X) % 10 == (10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||			// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (11) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (12) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (13) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (14) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX)))			// Front Detect Offset
	{
		if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] == 1)
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] = 0;
		else if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] == 1)
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] = 0;
		
		if (PowerBounceTime <= g_dElapsedTime) --g_sChar.health;
	}
	//Collision Civilian Player
	if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] >= 3 ||				// Back Detect Tile
		//((g_sChar.m_cLocation.X) % 10 == -1 * ((10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||	// Back Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == -1 * ((11) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == -1 * ((12) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == -1 * ((13) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX) ||		// Back Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == -1 * ((10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX))) ||	// Back Detect Offset
		(levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] >= 3 &&			// Front Detect Tile
		((g_sChar.m_cLocation.X) % 10 == (10) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1)) * tileSizeX)) % tileSizeX ||			// Front Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == (11) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == (12) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == (13) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		//(g_sChar.m_cLocation.X) % 10 == (14) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1))* tileSizeX)) % tileSizeX ||				// Front Detect Offset
		(g_sChar.m_cLocation.X) % 10 == (10 - 1) - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1)) * tileSizeX)) % tileSizeX)))			// Front Detect Offset
	{
		if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] == 4 || 
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] == 4) {

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, 5);
			powerup = dist(gen);
			PowerUps();
		}
		
		if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] >= 3)
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1))][(g_sChar.m_cLocation.Y) / 5] = 0;
		else if (levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] >= 3)
			levelMap[(g_sChar.m_cLocation.X) / 10 + int(gameTime * (difficulty * 0.5 + 1)) + 1][(g_sChar.m_cLocation.Y) / 5] = 0;

		score += 10;
		++g_sChar.health;

	}


	//Autoscroll
	for (int tileY = 0; tileY < 50 / tileSizeY; ++tileY)
		for (int tileX = 0; tileX < 200 / tileSizeX; ++tileX) {
			if (levelMap[tileX + int(gameTime * (difficulty * 0.5 + 1))][tileY] != 0)
				for (int tilePixelY = 0; tilePixelY < tileSizeY; ++tilePixelY)
					for (int tilePixelX = 0; tilePixelX < tileSizeX; ++tilePixelX) {
						WORD i;
						if (levelMap[tileX + int(gameTime * (difficulty * 0.5 + 1))][tileY] <= 3) i = colors[levelMap[tileX + int(gameTime * (difficulty * 0.5 + 1))][tileY]];
						else if (levelMap[tileX + int(gameTime * (difficulty * 0.5 + 1))][tileY] == 4) i = 0x90;
						else i = 0x20;
						g_Console->writeToBuffer({ SHORT(tileX * tileSizeX + tilePixelX - (tileSizeX + int((gameTime * (difficulty * 0.5 + 1)) * tileSizeX)) % tileSizeX),
													SHORT(tileY * tileSizeY + tilePixelY) }, " ", i);
					}

			// Trails
			/*if (levelMap[tileX + int(gameTime * 2)][tileY] == 2 && levelMap[tileX + int(gameTime * 2) + 1][tileY] != 2) {
				g_Console->writeToBuffer({	SHORT((tileX + 1) * tileSizeX - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX),
											SHORT(tileY * tileSizeY)}, "    ", 0xFF);
				g_Console->writeToBuffer({	SHORT((tileX + 1) * tileSizeX - (tileSizeX + int((gameTime)* tileSizeX * 2)) % tileSizeX),
											SHORT(tileY * tileSizeY + 4)}, "    ", 0xFF);
			}*/
		}

	//Horizontal UI
	for (int i = 0; i < 50; i += 45) {
		for (int j = 0; j < 5; ++j) {
			for (int k = 0; k < 200; ++k) {
				if (gameTime > (20 / (difficulty * 0.5 + 1))) {
					g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, " ", ThemeColours[themeSelected][5]);
					if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 0)) g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "Û", ThemeColours[themeSelected][5]);
					if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 1)) g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "²", ThemeColours[themeSelected][5]);
					if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 2)) g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "°", ThemeColours[themeSelected][5]);
					if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 3)) g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, " ", ThemeColours[themeSelected][5]);
					/*switch (int(gameTime * 4)) {
					case 38: g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "Û", 0x80); break;
					case 39: g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "²", 0x80); break;
					case 40: g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "±", 0x80); break;
					case 41: g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, "°", 0x80); break;
					default: g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, " ", 0x80); break;
					};*/
				}
				else g_Console->writeToBuffer({ SHORT(k), SHORT(i + j) }, " ", ThemeColours[themeSelected][5]);
			}
		}
	}

	//Vertical Fire
	for (int i = 5; i < 45; ++i) {
		for (int j = 0; j < 200; j += 190) {
			if (gameTime > (20 / (difficulty * 0.5 + 1))) {
				g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "          ", ThemeColours[themeSelected][6]);
				if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 0)) {
					sfx = SFX_EXPLOSION1;
					g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "ÛÛÛÛÛÛÛÛÛÛ", ThemeColours[themeSelected][6]);
				}
				if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 1)) g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "²²²²²²²²²²", ThemeColours[themeSelected][6]);
				if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 2)) g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "°°°°°°°°°°", ThemeColours[themeSelected][6]);
				if (int(gameTime * 4) == (20 / (difficulty * 0.5 + 1) * 4 + 3)) g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "          ", ThemeColours[themeSelected][6]);
				if (int(gameTime) == (20 / (difficulty * 0.5 + 1) + 1)) sfx = SFX_ERASE;
				/*switch (int(gameTime * 4)) {
				case 38: g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "ÛÛÛÛÛÛÛÛÛÛ", 0x40); break;
				case 39: g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "²²²²²²²²²²", 0x40); break;
				case 40: g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "±±±±±±±±±±", 0x40); break;
				case 41: g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "°°°°°°°°°°", 0x40); break;
				default: g_Console->writeToBuffer({ SHORT(j), SHORT(i) }, "          ", 0x40); break;
				};*/
			}
			else g_Console->writeToBuffer({ SHORT(190), SHORT(i) }, "          ", ThemeColours[themeSelected][6]);
		}
	}

	// Display Powerup Alert
	if (PowerDisplayTime >= g_dElapsedTime) {
		switch (powerup) {
		case 0:
			g_Console->writeToBuffer({ SHORT(92), SHORT(47) }, "Powerup: Score!", ThemeColours[themeSelected][4]);
			break;
		case 1:
			g_Console->writeToBuffer({ SHORT(92), SHORT(47) }, "Powerup: Health!", ThemeColours[themeSelected][4]);
			break;
		default:
			g_Console->writeToBuffer({ SHORT(88), SHORT(47) }, "Powerup: Invincibility!", ThemeColours[themeSelected][4]);
			break;
		};
	}
}
