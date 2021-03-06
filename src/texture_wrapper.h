#pragma once
#include <string.h>

class texture_wrapper {
	public:
		texture_wrapper();
		~texture_wrapper();

		//getters
		int get_height();
		int get_width();

		//other functions
		void free();
		bool load_texture(std::string path);
		bool load_text(std::string text, SDL_Color colour, std::string font, int size);
		void render(int x, int y, double angle);
		void render(int x, int y);

	protected:
		int width, height;
		SDL_Texture* texture;
};

#include "texture_wrapper.cpp"
