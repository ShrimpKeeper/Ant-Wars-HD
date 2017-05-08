texture_wrapper::texture_wrapper()
{
	texture = NULL;
	width = -1;
	height = -1;
}

texture_wrapper::~texture_wrapper()
{
	free();
}

bool texture_wrapper::load_texture(std::string path)
{
	this->free();
	SDL_Texture* new_texture = NULL;

	SDL_Surface* new_surface = IMG_Load(path.c_str());
	if (new_surface == NULL) {
		std::cout << "could not load image: " << SDL_GetError() << std::endl;
	} else {
		SDL_SetColorKey(new_surface, SDL_TRUE, SDL_MapRGB(new_surface->format, 0xff, 0x00, 0xff));
		new_texture = SDL_CreateTextureFromSurface(renderer, new_surface);
		if (new_texture == NULL) {
			std::cout << "could not create texture: " << SDL_GetError() << std::endl;
		} else {
			width = new_surface->w;
			height = new_surface->h;
		}
		SDL_FreeSurface(new_surface);
	}

	texture = new_texture;
	return texture != NULL;
}

void texture_wrapper::free()
{
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = -1;
		height = -1;
	}
}

int texture_wrapper::get_height()
{
	return height;
}

int texture_wrapper::get_width()
{
	return width;
}

void texture_wrapper::render(int x, int y)
{
	SDL_Rect render_quad = {x, y, width, height};
	SDL_RenderCopy(renderer, texture, NULL, &render_quad);
}
