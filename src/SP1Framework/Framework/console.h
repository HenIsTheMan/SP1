#pragma once
#include <windows.h>
#include <string>

void gotoXY(SHORT x, SHORT y);
void gotoXY(COORD c);
void colour(WORD attrib); //Set the test and background to be of a specific colour, bit pos 0 - 7
void cls(HANDLE hConsole = 0); //Cls for a console handle

class Console{ //Fast rendering to console obj
	public: //Constructors are used to set console size and title
		Console(COORD consoleSize, LPCSTR lpConsoleTitle = 0);
		Console(SHORT consoleWidth, SHORT consoleHeight, LPCSTR lpConsoleTitle = 0);
		~Console();
		COORD getConsoleSize(){
			return m_cConsoleSize;
		};
		COORD getMaxConsoleSize(){
			return m_cMaxConsoleSize;
		};
		void setConsoleTitle(LPCSTR lpConsoleTitle); //Set console title
		void setConsoleFont(SHORT width, SHORT height, LPCWSTR lpcwFontName); //Set console font
		void flushBufferToConsole(); //Write contents of the buffer to the screen
		void clearBuffer(WORD attribute = 0x0F); //Cls with a colour
		void writeToBuffer(COORD c, LPCSTR str, WORD attribute = 0x0F);
		void writeToBuffer(COORD c, std::string& s, WORD attribute = 0x0F);
		void writeToBuffer(COORD c, char ch, WORD attribute = 0x0F);
		void writeToBuffer(SHORT x, SHORT y, LPCSTR str, WORD attribute = 0x0F);
		void writeToBuffer(SHORT x, SHORT y, std::string& s, WORD attribute = 0x0F);
		void writeToBuffer(SHORT x, SHORT y, char ch, WORD attribute = 0x0F);
	private: //Class vars are used to set a screen buffer to write to before flushing it to the screen, don't bother trying to call the functions below
		HANDLE m_hScreenBuffer;
		CHAR_INFO* m_ciScreenDataBuffer;
		COORD m_cConsoleSize;
		COORD m_cMaxConsoleSize;
		const UINT32 m_u32ScreenDataBufferSize;
		void setConsoleWindowSize(); //Set console size (there is a limit to how big a console can be set)
		void initConsole(COORD consoleSize, LPCSTR lpConsoleTitle = 0); //Init console for a size
		void writeToConsole(const CHAR_INFO* lpBuffer);
		void shutDownConsole();
};