#include <iostream>
#include <thread>
#include <string>

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

//screen dimensions
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;


bool quit = false; //looping flag

//other files
#include "texture_wrapper.h"
#include "ants.h"

enum ui {
	MENU,
	TWO_PLAYER_GAME
};

ui ui_state = MENU;
ant right_ant(YA_BOY, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);//right ant
//ant right_ant(YA_BOY, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);//right ant
bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Could not init SDL: " << SDL_GetError() << std::endl;
		success = false;
	} else {
		window = SDL_CreateWindow("ANT WARS HD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
			success = false;
		} 
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL) {
			std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
			success = false;
		}
	}

	return success;
}

void close()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
}

void render_loop()
{
	class background_texture : public texture_wrapper {
		SDL_Rect rect;
		public:
		background_texture()
		{
			rect.x = 0;
			rect.y = 0;
			rect.w = SCREEN_WIDTH;
			rect.h = SCREEN_HEIGHT;
		}
		void render()
		{
			SDL_RenderCopy(renderer, texture, NULL, &rect);
		}
	};
	//load background
	background_texture background;
	background.load_texture((std::string)"res/" + (std::string)RES_PACK + (std::string)"/bg.jpg");

	//load title
	texture_wrapper title;
	title.load_texture((std::string)"res/" + (std::string)RES_PACK + (std::string)"/title.png");

	//load options
	texture_wrapper options;
	options.load_texture((std::string)"res/" + (std::string)RES_PACK + (std::string)"/options.png");

	while (!quit) {
		//render background
		background.render();

		//render menu
		if (ui_state == MENU) {
			title.render(SCREEN_WIDTH/2 - 250, 0);
			options.render(SCREEN_WIDTH/2 - 220, SCREEN_HEIGHT/2);
		} else if (ui_state == TWO_PLAYER_GAME) {
			right_ant.render();
		}

		//render
		SDL_RenderPresent(renderer);
	}
}

int main()
{
	if (!init()) {
		std::cerr << "failed to init\n";
		return 1;
	}

	
	std::thread render_loop_thread(render_loop);

	//main loop
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				quit = true;
			if (ui_state == MENU && e.key.keysym.sym == SDLK_SPACE) {
				ui_state = TWO_PLAYER_GAME;
			}
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		/*if (currentKeyStates[SDL_SCANCODE_LEFT])
			right_ant.move(LEFT);
		if (currentKeyStates[SDL_SCANCODE_UP])
			right_ant.move(FORWARDS);*/
	}

	render_loop_thread.join();

	close();
	return 0;
}
