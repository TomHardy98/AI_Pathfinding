#include "Player.h"

Player* Player::s_self;

Player::Player(SDL_Renderer* _renderer, char* _file, int _w, int _h, int _x, int _y) : Sprite(_renderer, _file, 100, 100, _w, _h)
{
	m_timer = 0;

	m_positionUpdated = false;

	position.x = _x;
	position.y = _y;
	position.w = _w;
	position.h = _h;

	s_self = this;
	m_moved = false;
}

Player::~Player()
{
	if (image)
	{
		SDL_DestroyTexture(image);
	}
}

void Player::Update()
{
	m_timer--;
	
	if (m_timer > 0)
	{
		return;
	}

	m_moved = false;

	const Uint8* key = SDL_GetKeyboardState(NULL);

	if (key[SDL_SCANCODE_LEFT])
	{
		position.x -= 50;
		m_timer = 20;

		if (position.x < 0)
		{
			position.x = 0;
		}
		m_moved = true;
	}

	if (key[SDL_SCANCODE_RIGHT])
	{
		position.x += 50;
		m_timer = 20;

		if (position.x > 1150)
		{
			position.x = 1150;
		}
		m_moved = true;
	}

	if (key[SDL_SCANCODE_UP])
	{
		position.y -= 50;
		m_timer = 20;

		if (position.y < 0)
		{
			position.y = 0;
		}
		m_moved = true;
	}

	if (key[SDL_SCANCODE_DOWN])
	{
		position.y += 50;
		m_timer = 20;

		if (position.y > 750)
		{
			position.y = 750;
		}
		m_moved = true;
	}
}