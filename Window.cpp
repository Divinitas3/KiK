#include "Window.h"



InitializeSDL::InitializeSDL() {
	SDL_Init(SDL_INIT_VIDEO);
}

InitializeSDL::InitializeSDL(Uint32 value) {
	initValue = value;
	if (SDL_Init(this->initValue) < 0) {
		printf("BLAD W INIT!\n");
		exit(-1);
	}
}

InitializeSDL::~InitializeSDL() {
	SDL_Quit();
}

CreateWindow::CreateWindow() {
	this->windowHandle = SDL_CreateWindow("<UKNOWN>",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_SHOWN);
	if (!this->windowHandle) {
		printf("BLAD W CREATE!\n");
		exit(-1);
	}

	this->windowSurface = SDL_GetWindowSurface(this->windowHandle);
}

CreateWindow::CreateWindow(PCHAR name, INT wWidth, INT hHeight, INT sShowType) {
	this->windowHandle = SDL_CreateWindow(name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		wWidth,
		hHeight,
		sShowType);
	if (!this->windowHandle) {
		printf("BLAD W CREATE!\n");
		exit(-1);
	}

	this->windowSurface = SDL_GetWindowSurface(this->windowHandle);
}

void CreateWindow::BuiltSurface(SDL_Surface* mediaSurface) {
	SDL_BlitSurface(mediaSurface, NULL, this->windowSurface, NULL);
	SDL_UpdateWindowSurface(this->windowHandle);
}

void CreateWindow::BiltScaled(SDL_Surface* mediaSurface, SDL_Rect* rect) {
	SDL_BlitScaled(mediaSurface, NULL, this->windowSurface, rect);
	SDL_UpdateWindowSurface(this->windowHandle);
}

CreateWindow::~CreateWindow() {
	SDL_DestroyWindow(windowHandle);
	this->windowHandle = NULL;
}

LoadMedia::LoadMedia() {
	this->pathToGraphic = "04_key_presses/press.bmp";
	this->mediaHandle = SDL_LoadBMP(this->pathToGraphic);
	if (!(this->mediaHandle)) {
		printf("ERROR: Cannot import media from: %s", this->pathToGraphic);
		exit(0);
	}
	//printf("ERROR: You have to give path !\n");
	//exit(0);
}

LoadMedia::LoadMedia(PCHAR path) {
	this->pathToGraphic = path;
	this->mediaHandle = SDL_LoadBMP(this->pathToGraphic);
	if (!(this->mediaHandle)) {
		printf("ERROR: Cannot import media from: %s", this->pathToGraphic);
		exit(0);
	}

}

void LoadMedia::optimzeSurface(SDL_Surface* windowSurface) {
	if (this->mediaHandle) {
		SDL_Surface* optimizedSurface = NULL;
		optimizedSurface = SDL_ConvertSurface(this->mediaHandle, windowSurface->format, NULL);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image! SDL Error: %s\n", SDL_GetError());
		}

		SDL_FreeSurface(this->mediaHandle);
		this->mediaHandle = optimizedSurface;
	}
}

LoadMedia::~LoadMedia() {
	SDL_FreeSurface(this->mediaHandle);
	this->mediaHandle = NULL;
	this->pathToGraphic = NULL;
}

mediaForKeyboard::mediaForKeyboard() {
	for (int i = 0; i < ARROWS; i++) {
		this->pathToGraphic[i] = "04_key_presses/press.bmp";
		this->mediaHandle[i] = SDL_LoadBMP(this->pathToGraphic[i]);
		if (!(this->mediaHandle)) {
			printf("ERROR: Cannot import media from: %s", this->pathToGraphic[i]);
			exit(0);
		}
	}
}

mediaForKeyboard::mediaForKeyboard(PCHAR* path) {
	for (int i = 0; i < ARROWS; i++) {
		if (!path[i])
			continue;
		this->pathToGraphic[i] = path[i];
		this->mediaHandle[i] = SDL_LoadBMP(this->pathToGraphic[i]);
		if (!(this->mediaHandle)) {
			printf("ERROR: Cannot import media from: %s", this->pathToGraphic[i]);
			exit(0);
		}
	}
}

mediaForKeyboard::~mediaForKeyboard() {
	for (int i = 0; i < ARROWS; i++) {
		SDL_FreeSurface(this->mediaHandle[i]);
		this->mediaHandle[i] = NULL;
		this->pathToGraphic[i] = NULL;
	}
}

Renderer::Renderer(const CreateWindow& window){
	this->renderer = SDL_CreateRenderer(window.windowHandle, -1, SDL_RENDERER_ACCELERATED);
	if (!this->renderer) {
		printf("ERROR: CANNOT RENDER !");
		exit(-1);
	}
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;
}

void Renderer::clearScreen(){
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);
}

void Renderer::updateScreen() {
	SDL_RenderPresent(this->renderer);
}

void Renderer::filledQuad(const Colors& colors, const RelativePosition& pos) {
	SDL_Rect fillRect = { pos.baseX, pos.baseY, pos.relativeX,  pos.relativeY};
	SDL_SetRenderDrawColor(this->renderer, colors.red, colors.green, colors.blue, colors.alpha);
	SDL_RenderFillRect(this->renderer, &fillRect);
}

void Renderer::outlinedQuad(const Colors& colors, const RelativePosition& pos) {
	SDL_Rect fillRect = { pos.baseX, pos.baseY, pos.relativeX,  pos.relativeY };
	SDL_SetRenderDrawColor(this->renderer, colors.red, colors.green, colors.blue, colors.alpha);
	SDL_RenderDrawRect(this->renderer, &fillRect);
}

void Renderer::horizontalQuad(const Colors& colors, const RelativePosition& pos) {
	SDL_SetRenderDrawColor(this->renderer, colors.red, colors.green, colors.blue, colors.alpha);
	SDL_RenderDrawLine(this->renderer, pos.baseX, pos.baseY, pos.relativeX, pos.relativeY);
}

void Renderer::pointQuad(const Colors& colors, const RelativePosition& pos) {
	SDL_SetRenderDrawColor(this->renderer, colors.red, colors.green, colors.blue, colors.alpha);
	SDL_RenderDrawPoint(this->renderer, pos.baseX, pos.baseY);
}

void Renderer::drawCircle(const Colors& colors, const RelativePosition& pos, INT radius) {
	const double pi = 3.1415;
	for (int i = 0; i < 360; i++) {
		pointQuad(
			colors,
			RelativePosition(pos.baseX + int(radius * cos(i * pi / 180.0)), pos.baseY + int(radius * sin(i * pi / 180.0)), 0, 0)
		);
	}
}

void Renderer::drawX(const Colors& colors, const RelativePosition& pos, const INT& radius) {
	const double pi = 3.1415;
	horizontalQuad(
		colors,
		RelativePosition(pos.baseX, pos.baseY, pos.baseX + int(radius * cos(45 * pi / 180.0)), pos.baseY + int(radius * sin(45 * pi / 180.0)))
	);
	horizontalQuad(
		colors,
		RelativePosition(pos.baseX, pos.baseY, pos.baseX + int(radius * cos(135 * pi / 180.0)), pos.baseY + int(radius * sin(135 * pi / 180.0)))
	);
	horizontalQuad(
		colors,
		RelativePosition(pos.baseX, pos.baseY, pos.baseX + int(radius * cos(225 * pi / 180.0)), pos.baseY + int(radius * sin(225 * pi / 180.0)))
	);
	horizontalQuad(
		colors,
		RelativePosition(pos.baseX, pos.baseY, pos.baseX + int(radius * cos(315 * pi / 180.0)), pos.baseY + int(radius * sin(315 * pi / 180.0)))
	);
}

void Renderer::drawGameField(const Colors& colors, const INT& howMany, infoAboutGameFields& info) {
	INT deltaX = 0;
	INT deltaY = 0;
	INT pola = howMany * howMany;
	INT iteratorX = int(SCREEN_WIDTH / (howMany * 2));
	INT iteratorY = int(SCREEN_HEIGHT / (howMany * 2));
	INT centerX = iteratorX;
	INT centerY = iteratorY;
	INT tab[DIMENSION] = {};
	for (int i = 1; i <= pola; i++) {
		outlinedQuad(
			colors,
			RelativePosition(deltaX, deltaY, SCREEN_WIDTH / howMany, SCREEN_HEIGHT / howMany)
		);

		info.centerOfFields[i - 1][0] = centerX;
		info.centerOfFields[i - 1][1] = centerY;
		centerX += 2 * iteratorX;
		deltaX += SCREEN_WIDTH / howMany;
		if (!(i % howMany)) {
			deltaX = 0;
			centerX = iteratorX;
			deltaY += SCREEN_HEIGHT / howMany;
			centerY += 2 * iteratorY;
		}
	}
}

infoAboutGameFields::infoAboutGameFields(INT fields) {
	this->fields = fields;
	INT fieldsPow = fields * fields;
	//INT area = (SCREEN_WIDTH / fieldsPow) / (SCREEN_HEIGHT / fieldsPow);
	this->radius = (SCREEN_WIDTH / fieldsPow);
	this->centerOfFields = new INT*[fieldsPow];

	for (int i = 0; i < fieldsPow; i++) {
		this->centerOfFields[i] = new INT[DIMENSION];
		for (int j = 0; j < DIMENSION; j++) {
			this->centerOfFields[i][j] = 0;
		}
	}

}

void infoAboutGameFields::fill(INT whichField, INT* value) {
	this->centerOfFields[whichField] = value;
}

infoAboutGameFields::~infoAboutGameFields() {
	INT fieldsPow = fields * fields;
	for (int i = 0; i < fieldsPow; i++) {
		delete [] this->centerOfFields[i];
		this->centerOfFields[i] = NULL;
	}
	delete [] this->centerOfFields;
	this->centerOfFields = NULL;
}


MouseControl::MouseControl() {
	this->position.x = 0;
	this->position.y = 0;
}

MouseControl::~MouseControl() { }

void MouseControl::setPosition(INT x, INT y) {
	this->position.x = x;
	this->position.y = y;
}

void MouseControl::handleEvent(SDL_Event* event) {
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
	{
		switch (event->type)
		{
			case SDL_MOUSEMOTION:
				this->status = CLICK_MOUSE_USING;
				break;
			case SDL_MOUSEBUTTONDOWN:
				this->status = CLICK_MOUSE_DOWN;
				getButton(event->button);
				break;
			case SDL_MOUSEBUTTONUP:
				this->status = CLICK_MOUSE_UP;
				getButton(event->button);
				break;
		}
	}
}

void MouseControl::getButton(const SDL_MouseButtonEvent& event) {
	if (this->status == CLICK_MOUSE_USING || this->status == CLICK_MOUSE_UP || this->status == CLICK_MOUSE_DOWN) {
		switch (event.button)
		{
			case SDL_BUTTON_LEFT:
				this->button = LEFT_BUTTON;
				break;
			case SDL_BUTTON_RIGHT:
				this->button = RIGHT_BUTTON;
				break;
		}
	}

}

INT MouseControl::checkPosition(infoAboutGameFields& info) {
	INT x, y, top, down, left, right;
	SDL_GetMouseState(&x, &y);
	INT allFields = info.fields * info.fields;
	const INT height = INT(SCREEN_HEIGHT * (1.0 / info.fields));
	const INT width = INT(SCREEN_WIDTH * (1.0 / info.fields));
	for (int i = 0; i < allFields; i++) {
		top = info.centerOfFields[i][1] + INT((0.5 * height));
		down = info.centerOfFields[i][1] - INT((0.5 * height));
		left = info.centerOfFields[i][0] - INT((0.5 * width));
		right = info.centerOfFields[i][0] + INT((0.5 * width));
		if (x > left && x < right && y < top && y > down) {
			return i;
		}
	}

	return -1;
}

GameControl::GameControl() {
	this->fields = 9;
	INT field = int(sqrt(9));
	this->gameFields = new INT*[field];
	for (int i = 0; i < field; i++) {
		this->gameFields[i] = new INT[field];
		for (int j = 0; j < field; j++) {
			this->gameFields[i][j] = 0;
		}
	}

}

GameControl::~GameControl() {
	INT field = this->fields;
	for (int i = 0; i < fields; i++) {
		delete[] this->gameFields[i];
		this->gameFields[i] = NULL;
	}
	delete[] this->gameFields;
	this->gameFields = NULL;
}

GameControl::GameControl(const infoAboutGameFields& info) {
	this->fields = info.fields;
	INT field = info.fields;
	this->gameFields = new INT*[field];
	for (int i = 0; i < field; i++) {
		this->gameFields[i] = new INT[field];
		for (int j = 0; j < field; j++) {
			this->gameFields[i][j] = 0;
		}
	}
}

void GameControl::putNewObject(const INT& where, const INT& what) {
	if (!this->IsEmptyField(where / this->fields, where % this->fields)) {
		return;
	}

	this->gameFields[where / this->fields][where % this->fields] = what;
}

void GameControl::renderGameField(const infoAboutGameFields& info, const Colors& color, Renderer& render) {
	int sq = this->fields;
	for (int i = 0; i < sq; i++) {
		for (int j = 0; j < sq; j++) {
			if (!IsEmptyField(i, j))
			{
				switch (this->gameFields[i][j])
				{
				case Circle:
					render.drawCircle(
						Colors(0x00, 0x00, 0xFF, 0xFF),
						RelativePosition(info.centerOfFields[i * this->fields + j][0], info.centerOfFields[i * this->fields + j][1], 0, 0),
						info.radius
					);
					break;
				case Iks:
					render.drawX(
						Colors(0x00,0xFF,0x00,0xFF),
						RelativePosition(info.centerOfFields[i * this->fields + j][0], info.centerOfFields[i * this->fields + j][1], 0, 0),
						info.radius
					);
					break;
				}
			}
		}
	}
}

bool GameControl::IsEmptyField(const INT& i, const INT& j) {
	return !(this->gameFields[i][j]);
}

void GameControl::objectSerives(MouseControl& mouse, infoAboutGameFields& info) {
	INT posInfo = mouse.checkPosition(info);
	if (posInfo >= 0) {
		if (mouse.status == CLICK_MOUSE_DOWN) {
			switch (mouse.button) {
			case LEFT_BUTTON:
				putNewObject(posInfo, Circle);
				break;
			case RIGHT_BUTTON:
				putNewObject(posInfo, Iks);
				break;
			}
		}
	}
}

INT GameControl::getFields() const {
	return this->fields;
}

INT GameControl::getSingleObject(const INT& i, const INT& j) const {
	return this->gameFields[i][j];
}

Player::Player(PCHAR name, WhatItIs what, Colors color) {
	this->name = name;
	this->what = what;
	this->color = color;
}

INT Player::getWhat() {
	return this->what;
}

Colors Player::getColors() {
	return this->color;
}

Game::Game(Player playerOne, Player playerTwo) {
	this->playerOne = playerOne;
	this->playerTwo = playerTwo;
}

Game::~Game() { }

INT Game::checkIsWinner(const GameControl& game) {
	INT iksCount = 0;
	INT circleCount = 0;

	for (int i = 0; i < game.getFields(); i++) {
		for (int j = 0; j < game.getFields(); j++) {
			if (game.getSingleObject(i, j) == Iks) {
				iksCount++;
			}
			if (game.getSingleObject(i, j) == Circle) {
				circleCount++;
			}
			if (iksCount == game.getFields()) {
				return Iks;
			}
			if (circleCount == game.getFields()) {
				return Circle;
			}
		}
		iksCount = 0;
		circleCount = 0;
	}
	for (int j = 0; j < game.getFields(); j++) {
		for (int i = 0; i < game.getFields(); i++) {
			if (game.getSingleObject(i, j) == Iks) {
				iksCount++;
			}
			if (game.getSingleObject(i, j) == Circle) {
				circleCount++;
			}
			if (iksCount == game.getFields()) {
				return Iks;
			}
			if (circleCount == game.getFields()) {
				return Circle;
			}
		}
		iksCount = 0;
		circleCount = 0;
	}
	for (int j = 0; j < game.getFields(); j++) {
		if (game.getSingleObject(j, j) == Iks) {
			iksCount++;
		}
		if (game.getSingleObject(j, j) == Circle) {
			circleCount++;
		}
		if (iksCount == game.getFields()) {
			return Iks;
		}
		if (circleCount == game.getFields()) {
			return Circle;
		}
	}
	iksCount = 0;
	circleCount = 0;
	for (int j = game.getFields() - 1; j >= 0; j--) {
		for (int i = 0; i < game.getFields(); i++) {
			if (game.getSingleObject(j, i) == Iks) {
				iksCount++;
			}
			if (game.getSingleObject(j, i) == Circle) {
				circleCount++;
			}
			if (iksCount == game.getFields()) {
				return Iks;
			}
			if (circleCount == game.getFields()) {
				return Circle;
			}
		}
	}
	return 0;
}

bool Game::play(infoAboutGameFields& info, GameControl& game, MouseControl& mouse) {
	return true;
}