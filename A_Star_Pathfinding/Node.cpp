#include "Node.h"
#include <Windows.h>

Node::Node()
{
	m_walkable = true;
	m_hasBeenChecked = false;
	parent = NULL;
	tmp = NULL;
}

Node::Node(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h)
{
	SDL_Surface* bmpFile = SDL_LoadBMP(_file);
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	position.x = _x;
	position.y = _y;
	position.w = _w;
	position.h = _h;

	renderer = _renderer;
}

Node::~Node()
{
	if (image)
	{
		SDL_DestroyTexture(image);
	}
}

void Node::Draw()
{
	if (tmp)   // If temp exists then draw it and return
	{
		SDL_RenderCopy(renderer, tmp, NULL, &position);
		return;
	}
	if (image)   // If image exists then draw it and return
	{
		SDL_RenderCopy(renderer, image, NULL, &position);
	}
}

void Node::Reset()
{
	m_hasBeenChecked = false;
	parent = NULL;
}

void Node::DEBUG_ChangeImage(SDL_Renderer* _renderer)
{
	if (tmp)   // If the temp texture exists then destroy it
	{
		SDL_DestroyTexture(tmp);
	}

	SDL_Surface* bmpFile = SDL_LoadBMP("Block_Template5.bmp");   // Draws the current node to a footstep block
	tmp = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);
}

void Node::reset(SDL_Renderer* _renderer)
{
	if (tmp)   // If the temp texture exists then destroy it
	{
		SDL_DestroyTexture(tmp);
	}

	tmp = NULL;
}

void Node::genF(Node* _goal)   // Generating the F value takes in the goal node
{
	//f = abs(position.x - _goal->position.x) + abs(position.y - _goal->position.y);   // Generates the F value using the Manhattan Heuristic (MANHATTAN DISTANCE = | cellx - exitx | + | celly - exity |)

	float x = position.x - _goal->position.x;   // Generates F value using the Euclidean Distance Heuristic
	float y = position.y - _goal->position.y;
	f = sqrt(x*x + y*y);
}