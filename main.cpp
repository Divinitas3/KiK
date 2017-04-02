#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Window.h"
#include <cmath>
#include <Windows.h>
#undef CreateWindow

int main(int argc, char* args[])
{
	int pola;
	printf("Podaj ilosc pol: \n");
	scanf_s("%d", &pola);
	InitializeSDL open;
	CreateWindow window("TESTOWY", 640, 480, SDL_WINDOW_SHOWN);
	
	Renderer render(window);

	bool quit = false;

	infoAboutGameFields info(pola);
	Colors color(0xFF, 0x00, 0x00, 0xFF);
	MouseControl mouse;
	GameControl game(info);
	Player one("Placek", Iks, Colors(0x00, 0xFF, 0x00, 0xFF));
	Player two("Jacek", Circle, Colors(0x00, 0xFF, 0x00, 0xFF));
	Game play(one, two);

	render.drawGameField(
		color,
		pola,
		info
	);
	volatile INT whoIsNow = 1;
	while (!quit)
	{

		if (SDL_WaitEvent(&info.event) != 0) {
			if (info.event.type == SDL_QUIT) {
				quit = true;
			}
		}

		INT winner = play.checkIsWinner(game);
		if (winner) {
			switch (winner) {
			case Iks:
				MessageBox(NULL, L"Wygral IKS", L"Kolko i krzyzyk", NULL);
				break;
			case Circle:
				MessageBox(NULL, L"Wygralo KOLKO", L"Kolko i krzyzyk", NULL);
				break;
			}
			quit = true;
		}
		
		mouse.handleEvent(&info.event);
		if (whoIsNow == 1) {
			if (mouse.button == play.playerOne.getWhat()) {
				game.objectSerives(mouse, info);
				whoIsNow = -1;
			}
		}

		if (whoIsNow == -1) {
			if (mouse.button == play.playerTwo.getWhat()) {
				game.objectSerives(mouse, info);
				whoIsNow = 1;
			}
			
		}
		game.renderGameField(info, color, render);
		render.updateScreen();
	}
	render.clearScreen();
	
	return 0;
}