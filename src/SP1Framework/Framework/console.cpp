#include "console.h"
#include <cstdio>
#define PERR(bSuccess, api){ if(!bSuccess) printf("%s:Error %d from %s \on line %d\n", __FILE__, GetLastError(), api, __LINE__); } //Standard error macro for reporting API errors

void gotoXY(COORD c){ //Set pos of console cursor (x and y are SHORT)
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void gotoXY(SHORT iX, SHORT iY){ //Set pos of console cursor using int (input: int, int)
	COORD c = {iX, iY};
	gotoXY(c);
}

void colour(WORD wAttrib){ //Set colour of console text, WORD (2 bytes data type)   
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, wAttrib);        
}

void cls(HANDLE hConsole){
	if(!hConsole){
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	CONSOLE_SCREEN_BUFFER_INFO csbi; //Get buffer info 
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD cCharsWritten, dwConSize = csbi.dwSize.X * csbi.dwSize.Y; //Get the no. of char cells in the current buffer//
	FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, {0, 0}, &cCharsWritten); //Fill the entire screen with blanks
	GetConsoleScreenBufferInfo(hConsole, &csbi); //Get current text attribute
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, {0, 0}, &cCharsWritten); //Set the buffer's attributes accordingly
	SetConsoleCursorPosition(hConsole, {0, 0});
}

Console::Console(COORD consoleSize, LPCSTR lpConsoleTitle): m_u32ScreenDataBufferSize(consoleSize.X * consoleSize.Y){
	initConsole(consoleSize, lpConsoleTitle);
}

Console::Console(SHORT consoleWidth, SHORT consoleHeight, LPCSTR lpConsoleTitle): m_u32ScreenDataBufferSize(consoleWidth * consoleHeight){
	COORD consoleSize = {consoleWidth, consoleHeight};
	initConsole(consoleSize, lpConsoleTitle);
}

Console::~Console(){
	shutDownConsole();
}

void Console::initConsole(COORD consoleSize, LPCSTR lpConsoleTitle){
	this->m_cConsoleSize = consoleSize;

	//Use the ASCII version for the console title
	SetConsoleTitleA(lpConsoleTitle);
	SetConsoleCP(437);

	//Set up screen buffer    
	m_ciScreenDataBuffer = new CHAR_INFO[m_u32ScreenDataBufferSize];
	m_hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //1st NULL for default security attributes
	m_cMaxConsoleSize = GetLargestConsoleWindowSize(m_hScreenBuffer);

	//Set console size
	setConsoleWindowSize();
	SetConsoleActiveScreenBuffer(m_hScreenBuffer); 	
}

void Console::setConsoleTitle(LPCSTR lpConsoleTitle){
	SetConsoleTitleA(lpConsoleTitle);
}

void Console::setConsoleFont(SHORT width, SHORT height, LPCWSTR lpcwFontName){
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = width;
	cfi.dwFontSize.Y = height;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, lpcwFontName);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Console::shutDownConsole(){
	delete[] m_ciScreenDataBuffer;
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

void Console::flushBufferToConsole(){
	writeToConsole(m_ciScreenDataBuffer);    
}

void Console::setConsoleWindowSize(){
	SMALL_RECT windowSize = {0, 0, m_cConsoleSize.X - 1, m_cConsoleSize.Y - 1};
	COORD buffSize = {m_cConsoleSize.X, m_cConsoleSize.Y};
	CONSOLE_SCREEN_BUFFER_INFO csbi; //Get buffer info
	HANDLE hConsole = m_hScreenBuffer; //GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	if((DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD)m_cConsoleSize.X * m_cConsoleSize.Y){ //If the current buffer is larger than what we want, resize the console window then the buffer 
		SetConsoleWindowInfo(hConsole, TRUE, &windowSize);    
		SetConsoleScreenBufferSize(hConsole, buffSize);
	} else{ //If... is smaller than..., resize the buffer then...
		SetConsoleScreenBufferSize(hConsole, buffSize);
		SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
	}
}

void Console::clearBuffer(WORD attribute){
	for(size_t i = 0; i < m_u32ScreenDataBufferSize; ++i){
		m_ciScreenDataBuffer[i].Char.AsciiChar = ' ';
		m_ciScreenDataBuffer[i].Attributes = attribute;
	}
}

void Console::writeToConsole(const CHAR_INFO* lpBuffer){
	COORD c = {0, 0};
	SMALL_RECT WriteRegion = {0, 0, m_cConsoleSize.X - 1, m_cConsoleSize.Y - 1};
	WriteConsoleOutputA(m_hScreenBuffer, lpBuffer, m_cConsoleSize, c, &WriteRegion); //For ASCII text
}

void Console::writeToBuffer(SHORT x, SHORT y, LPCSTR str, WORD attribute){
	size_t index = max(x + m_cConsoleSize.X * y, 0);
	for(size_t i = 0; i < min(m_u32ScreenDataBufferSize - index, strlen(str)); ++i){
		m_ciScreenDataBuffer[index + i].Char.AsciiChar = str[i];
		m_ciScreenDataBuffer[index + i].Attributes = attribute;
	}
}

void Console::writeToBuffer(COORD c, LPCSTR str, WORD attribute){    
	writeToBuffer(c.X, c.Y, str, attribute);
}

void Console::writeToBuffer(SHORT x, SHORT y, std::string& s, WORD attribute){
	writeToBuffer(x, y, s.c_str(), attribute);
}

void Console::writeToBuffer(COORD c, std::string& s, WORD attribute){
	writeToBuffer(c.X, c.Y, s.c_str(), attribute);
}

void Console::writeToBuffer(SHORT x, SHORT y, char ch, WORD attribute){
	if(x >= 0 && x < m_cConsoleSize.X && y >= 0 && y < m_cConsoleSize.Y){
		m_ciScreenDataBuffer[x + m_cConsoleSize.X * y].Char.AsciiChar = ch;
		m_ciScreenDataBuffer[x + m_cConsoleSize.X * y].Attributes = attribute;
	}
}

void Console::writeToBuffer(COORD c, char ch, WORD attribute){
	writeToBuffer(c.X, c.Y, ch, attribute);
}