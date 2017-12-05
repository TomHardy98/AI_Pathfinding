#pragma once
#include <SDL.h>

class Sprite
{
public:

	Sprite(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h);   // Constructor
	~Sprite();   // Destructor

	virtual void Draw();

protected:

	SDL_Renderer* renderer;   // SDL Renderer used for drawing

	SDL_Texture* image;

	SDL_Rect position;
};