#ifndef _HEADER_WINDOWS_
#define _HEADER_WINDOWS_

#include <SDL.h>
#include <stdio.h>

const int ARROWS = 5;
const int DIMENSION = 2;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyBoard {
	CLICK_UP,
	CLICK_DOWN,
	CLICK_RIGHT,
	CLICK_LEFT,
	CLICK_DEFAULT
};

enum Mouse {
	CLICK_MOUSE_DOWN,
	CLICK_MOUSE_UP,
	CLICK_MOUSE_USING,
};

enum whichMouse {
	LEFT_BUTTON = -1,
	RIGHT_BUTTON = 1,
};

enum WhatItIs {
	Circle = -1,
	Iks = 1,
};

typedef int INT;
typedef char* PCHAR;

class Colors {
public:
	Uint8 red;
	Uint8 green;
	Uint8 blue;
	Uint8 alpha;

	Colors() : red(0), green(0), blue(0), alpha(0) {}
	Colors(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : red(r), green(g), blue(b), alpha(a) {}
	~Colors() {}

};

class RelativePosition {
public:
	int baseX;
	int baseY;
	int relativeX;
	int relativeY;

	RelativePosition()
		: baseX(0), baseY(0), relativeX(0), relativeY(0) { }
	RelativePosition(int bBaseX, int bBaseY, int rRelativeX, int rRelativeY)
		: baseX(bBaseX), baseY(bBaseY), relativeX(rRelativeX), relativeY(rRelativeY) { }
	~RelativePosition() {}

};


class InitializeSDL {
	Uint32 initValue;
public:

	InitializeSDL();
	InitializeSDL(Uint32 value);
	~InitializeSDL();

};

class CreateWindow {
	PCHAR name;
	INT width;
	INT height;
	INT showType;
	SDL_Window* windowHandle;
public:
	SDL_Surface* windowSurface;

	CreateWindow();
	CreateWindow(PCHAR name, INT wWidth, INT hHeight, INT sShowType);
	void BuiltSurface(SDL_Surface* mediaSurface);
	void BiltScaled(SDL_Surface* mediaSurface, SDL_Rect* rect);
	~CreateWindow();
	friend class Renderer;

};

class LoadMedia {
	PCHAR pathToGraphic;
	
public:
	SDL_Surface* mediaHandle;
	LoadMedia();
	LoadMedia(PCHAR path);
	void optimzeSurface(SDL_Surface* windowSurface);
	~LoadMedia();

};

class mediaForKeyboard {
	PCHAR pathToGraphic[ARROWS];
public:
	SDL_Surface* mediaHandle[ARROWS];
	mediaForKeyboard();
	mediaForKeyboard(PCHAR* path);
	~mediaForKeyboard();
};

class infoAboutGameFields {

public:
	INT fields;
	INT radius;
	INT** centerOfFields;
	SDL_Event event;

	infoAboutGameFields(INT fields);
	void fill(INT whichField, INT* value);
	~infoAboutGameFields();
};

class Renderer {
public:
	SDL_Renderer* renderer;

	 Renderer(const CreateWindow& window);
	 void clearScreen();
	 void updateScreen();
	 void filledQuad(const Colors& colors, const RelativePosition& pos);
	 void outlinedQuad(const Colors& colors, const RelativePosition& pos);
	 void horizontalQuad(const Colors& colors, const RelativePosition& pos);
	 void pointQuad(const Colors& colors, const RelativePosition& pos);
	 void drawCircle(const Colors& colors, const RelativePosition& pos, INT radius);
	 void drawX(const Colors& colors, const RelativePosition& pos, const INT& radius);
	 void drawGameField(const Colors& colors, const INT& howMany, infoAboutGameFields& info);

	 ~Renderer();

};

class MouseControl {
	SDL_Point position;
	
public:
	INT button;
	Mouse status;
	void getButton(const SDL_MouseButtonEvent& event);
	MouseControl();
	~MouseControl();
	void setPosition(INT x, INT y);
	void handleEvent(SDL_Event* event);
	INT checkPosition(infoAboutGameFields& info);
};

class GameControl {
	INT fields;
	INT** gameFields;
	bool IsEmptyField(const INT& i, const INT& j);
public:
	GameControl();
	~GameControl();
	GameControl(const infoAboutGameFields& info);
	void putNewObject(const INT& where, const INT& what);
	void renderGameField(const infoAboutGameFields& info, const Colors& color, Renderer& render);
	void objectSerives(MouseControl& mouse, infoAboutGameFields& info);
	INT getFields() const;
	INT getSingleObject(const INT& i, const INT& j) const;
};

class Player {
	PCHAR name;
	INT what;
	Colors color;
public:
	Player() : name("<UKNOWN>"), what(Circle), color() { }
	~Player() { }
	Player(PCHAR name, WhatItIs what, Colors color);
	INT getWhat();
	Colors getColors();
};

class Game {

public:
	Player playerOne;
	Player playerTwo;
	Game() : playerOne(), playerTwo() {}
	Game(Player playerOne, Player playerTwo);
	INT checkIsWinner(const GameControl& game);
	bool play(infoAboutGameFields& info, GameControl& game, MouseControl& mouse);
	~Game();



};

#endif