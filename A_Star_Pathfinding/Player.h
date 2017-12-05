#pragma once

#include "Sprite.h"

class Player : public Sprite
{
public:

	Player(SDL_Renderer* renderer, char* file, int _w, int _h, int _x, int _y);
	~Player();

	int GetX() { return position.x; }
	int GetY() { return position.y; }

	void SetX(int _x) { position.x = _x; }
	void SetY(int _y) { position.y = _y; }
	void Update();

	bool m_positionUpdated;
	bool m_moved;

	float m_timer;

	static Player* s_self;
};