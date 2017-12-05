#pragma once
#include <SDL.h>
#include <vector>

class Node
{
public:

	Node();
	Node(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h);   // Constructor
	~Node();   // Destructor

	virtual void Draw();

	void Reset();
	void SetWalkable(bool _value) { m_walkable = _value; }   // Creates a set walkable call which returns a boolean value
	void SetHasBeenChecked(bool _value) { m_hasBeenChecked = _value; }
	void SetParent(Node* _value) { parent = _value; }
	void SetH(int _h) { h = _h; }
	void SetF() { f = h; }
	void genF(Node* _goal);
	void DEBUG_ChangeImage(SDL_Renderer* _renderer);
	void reset(SDL_Renderer* _renderer);

	int m_tileID;
	int GetX() { return position.x; }
	int GetY() { return position.y; }
	int GetH() { return h; }
	int GetF() { return f; }

	bool GetWalkable() { return m_walkable; }
	bool GetHasBeenChecked() { return m_hasBeenChecked; }
	bool m_walkable;

	SDL_Rect getRect() { return position; }
	SDL_Texture* image;
	SDL_Texture* tmp;

	Node* GetParent() { return parent; }

	std::vector<Node*>& GetNeighbours() { return neighbours; }

private:

	int f = 0;
	int h = 0;

protected:

	bool m_hasBeenChecked;

	std::vector<Node*> neighbours;

	Node* parent;

	SDL_Renderer* renderer;   // SDL Renderer used for drawing

	SDL_Rect position;
};