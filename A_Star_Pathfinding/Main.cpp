#include <SDL.h>   // Including SDL2 Extension
#include <iostream>   // Including IOStream for command console
#include "Node.h"   // Including Node header file
#include "Sprite.h"   // Including the Sprite header file
#include "Player.h"   // Including the Player header file
#include <vector>   // Including the vector functions
#include <fstream>   // Including the ability to read from a text file
#include <list>   // Including the ability to use lists
#include "State.h"   // Including the ability to use states
#include "StateManager.h"   // Including the ability to use the state manager
#include "IdleState.h"   // Including the ability to use the idle state
#include "SearchState.h"   // Including the ability to use the search state
#include <Windows.h>   // Including the ability to use the sleep timer
#include <time.h>   // Allows for the use of the time function which helps with the SRAND

// Function prototypes
void GameLoop(SDL_Window* _window, SDL_Renderer* _renderer, int _window_w, int _window_h);
void MapLoader(int const _gridHeight, int const _gridWidth, int _offset, SDL_Renderer* _renderer, std::vector<Node*>& _squares);
std::vector<Node*> BreadthFirst(std::vector<Node*>& _squares, int _start, int _end);
void SetNeighbours(std::vector<Node*>& _squares, int const _gridHeight, int const _gridWidth);
bool MainMenu(SDL_Renderer* _renderer);
bool PathfindingMenu(SDL_Renderer* _renderer);
std::vector<Node*> BestFirst(Node* _startNode, Node* _endNode, std::vector<Node*>& _squares);

int main(int, char**)
{
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	int _window_w = 1200;
	int _window_h = 800;

	if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )   // Try to initialising SDL, log error and pause if fail
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
		system("pause");
		return 0;
	}

	_window = SDL_CreateWindow("A_Star_Pathfinding", 100, 100, _window_w, _window_h, SDL_WINDOW_SHOWN);   // Try to create the window, log error and pause if fail

	if ( _window == NULL )
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("pause");
		return 0;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);   // Try to create the renderer, log error and pause if fail

	if ( _renderer == NULL )
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	GameLoop(_window, _renderer, _window_w, _window_h);   // This function call goes into the actual game loop

	SDL_DestroyRenderer(_renderer);   // Clean up, free any memeory we have used
	SDL_DestroyWindow(_window);
	SDL_Quit();

	return 0;
}

void GameLoop(SDL_Window* _window, SDL_Renderer* _renderer, int _window_w, int _window_h)
{
	StateManager manager;   // Creates a state manager called 'manager'
	SDL_Surface* footsteps = SDL_LoadBMP("Block_Template.bmp");   // Creates a surface from a bmp file
	SDL_Surface* water = SDL_LoadBMP("Block_Template3.bmp");   // Creates a surface from a bmp file
	SDL_Texture* footstepsTexture = SDL_CreateTextureFromSurface(_renderer, footsteps);   // Creates a texture from that surface
	SDL_Texture* waterTexture = SDL_CreateTextureFromSurface(_renderer, water);   // Creates a texture from that surface
	SDL_FreeSurface(footsteps);   // Deletes the surface and keeps the texture to save memory
	SDL_FreeSurface(water);   // Deletes the surface and keeps the texture to save memory
	Node* _startNode = NULL;   // Initialise the start node as NULL
	Node* _endNode = NULL;   // Initialise the end node as NULL
	Player* player = new Player(_renderer, "player.bmp", 50, 50, 0, 0);
	Sprite* ready_up = new Sprite(_renderer, "ready_up.bmp", 0, 0, 1200, 800);
	Sprite* game_over = new Sprite(_renderer, "game_over.bmp", 0, 0, 1200, 800);
	Sprite* five_lives = new Sprite(_renderer, "5lives.bmp", 0, 0, 1200, 800);
	Sprite* four_lives = new Sprite(_renderer, "4lives.bmp", 0, 0, 1200, 800);
	Sprite* three_lives = new Sprite(_renderer, "3lives.bmp", 0, 0, 1200, 800);
	Sprite* two_lives = new Sprite(_renderer, "2lives.bmp", 0, 0, 1200, 800);
	Sprite* one_life = new Sprite(_renderer, "1lives.bmp", 0, 0, 1200, 800);
	const Uint8* key = SDL_GetKeyboardState(NULL);   // Allows for the use of key inputs from the keyboard
	bool clear = true;   // Boolean for clearing renderer in loop
	bool quit = false;   // Boolean for quiting the window
	bool searching = false;   // Boolean for search to help with state management
	bool setBreadth = false;   // Boolean for setting the pathfinding algorithm to be used
	bool mapdrawn = false;   // Boolean for setting mapdrawn to false
	bool pathFoundPlayer = false;   // Initialise boolean for pathFoundPlayer as false
	bool pathLostPlayer = false;   // Initialises a boolean for pathLostPlayer set to false
	bool gameOver = false;   // Initialises a boolean for gameOver and sets to false
	int const _gridHeight = 16;   // Constant integer variable for grid height
	int const _gridWidth = 24;   // Constant integer variable for grid width
	int _offset = 50;   // Integer for the square offsets (50 pixels)
	int _start = -1;   // Integer for start of path
	int _end = -1;   // Integer for end of path
	int mouseX = 0;   // Integer for mouseX
	int mouseY = 0;   // Integer for mouseY
	int playerLives = 5;   // Integer for the playerLives set to 5
	int timer = 300;   // Initialise integer called timer set to 500
	int spawnTimer = 300;   // Initialises an integer for spawnTimer set to 1000 ticks
	int startPoint = (rand() % 382);   // Initialises the AI's start point to a random position;
	std::vector<Node*> _squares;   // Creates a vector for all the squares
	SDL_Event e;   // SDL event for exiting window
	srand(time(NULL));   // Generates SRAND using the computers time

	MainMenu(_renderer);   // Calls the main menu function to load the menu

	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);   // Stops the left click going through the menu and placing it on the map behind after a button is pressed

	MapLoader(_gridHeight, _gridWidth, _offset, _renderer, _squares);   // Calls the map loader function

	PathfindingMenu(_renderer);   // Calls the pathfinding menu function

	if ( PathfindingMenu(_renderer) == true )   // If the function returns true
	{
		setBreadth = true;   // Set breadth to true
	}

	if ( PathfindingMenu(_renderer) == false )   // If the function returns false
	{
		setBreadth = false;   // Set breadth to false
	}

	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);   // Stops the left click going through the menu and placing a block after its loaded

	SetNeighbours(_squares, _gridHeight, _gridWidth);   // Calls the set neighbour function

	std::cout << "Setting state to idle" << std::endl;
	Sleep(100);
	manager.setState(new IdleState(&manager));   // Sets the game state to the idle state
	manager.update();

	while (!quit)   // This is the main game loop
	{
		if ( spawnTimer <= 0 )
		{
			startPoint = (rand() % 382);   // Generates a position on the map between 0 and 382
			spawnTimer = 1000;   // Resets spawn timer to 1000
		}

		_start = startPoint;   // Sets the breadth first start point to the randomly generated number above
		_startNode = _squares.at(_start);   // Generates a position on the map between 0 and 382
		_squares[_start]->image = footstepsTexture;   // Sets the start positions texture to a footstep

		SDL_GetMouseState(&mouseX, &mouseY);   // Gets the mouse x and y position and passes it into the variables

		float playerpos = ((player->GetX() / 50) * 16) + (player->GetY() / 50);   // Puts the players X and Y position into the float variable

		if ( key[SDL_SCANCODE_ESCAPE] )   // If escape is pressed the program will close
		{
			quit = true;
		}

		while (SDL_PollEvent(&e) && gameOver == false) // Allows for mouse and keyboard events
		{	
			if ( e.type == SDL_KEYDOWN )
			{
				switch ( e.button.button )
				{
				case SDL_SCANCODE_SPACE:   // If the spacebar has been pressed
					if ( (mouseX > 0) && (mouseX < 1200) && (mouseY > 0) && (mouseY < 800) )   // Set coordinates for where your mouse is when the spacebar is pressed
					{
						mapdrawn = true;   // Sets mapdrawn to true so the player can be drawn
						break;
					}
					break;
				}
			}

			if ( Player::s_self->m_moved )   // If the player has moved
			{
				e.type = SDL_MOUSEBUTTONDOWN;   // Emulate the mouse button being pressed
				e.button.button = SDL_BUTTON_RIGHT;   // Emulate the right button so it sets a new end point
			}

			if ( e.type == SDL_MOUSEBUTTONDOWN )   // If a mouse button has been pressed
			{
				switch ( e.button.button )
				{
				case SDL_BUTTON_RIGHT:
					if ( _start != -1 )
					{
						for (int i = 0; i < _squares.size(); ++i)   // Loop through the squares vector that holds the map
						{
							if ( mouseX >= _squares[i]->getRect().x && mouseX <= _squares[i]->getRect().x + 50 && mouseY >= _squares[i]->getRect().y && mouseY <= _squares[i]->getRect().y + 50 )   // Checks the mouse position in the vector and then offsets it by the offset = 50
							{
								searching = true;
								_end = playerpos;   // Sets the end as the x and y position
								_endNode = _squares.at(playerpos);
								break;   // Breaks out of the switch
							}
						}
					}
					break;

				case SDL_BUTTON_MIDDLE:
					if (mapdrawn == false)
					{
						for (int i = 0; i < _squares.size(); ++i)   // Loop through the squares vector that holds the map
						{
							if (mouseX >= _squares[i]->getRect().x && mouseX <= _squares[i]->getRect().x + 50 && mouseY >= _squares[i]->getRect().y && mouseY <= _squares[i]->getRect().y + 50)   // Checks the mouse position in the vector and then offsets it by the offset = 50
							{
								std::cout << "Non-Walkable block has been placed" << std::endl;
								_squares[i]->image = waterTexture;   // Replaces the square in the vector with the water texture
								_squares[i]->SetWalkable(false);
								break;   // Breaks out of the switch
							}
						}
					}
				}
			}
		}

		spawnTimer--;

		// Below is how the algorithms are chosen and drawn out

		if ( _start != -1 && _end != -1 )
		{
			for (Node* drawnNode : _squares)   // Loops through the path vector
			{
				drawnNode->reset(_renderer);   // Deletes the path drawn
			}

			if ( setBreadth == true )   // If set breadth is set to true then run the breadth-first algorithm
			{
				std::vector<Node*> path = BreadthFirst(_squares, _start, _end);   // Calls the breadth-first pathfinding function and returns the path to a vector called 'path'

				if ( timer < 0 )   // If timer is less than 0 then draw path
				{
					if ( path.size() <= 6 )
					{
						for (Node* drawnNode : path)   // Loops through the path vector
						{
							drawnNode->DEBUG_ChangeImage(_renderer);   // Replaces all the nodes with a different image
						}

						std::cout << "AI has spotted the player!" << std::endl;
						pathFoundPlayer = true;
					}

					if ( path.size() >= 7 && path.size() <= 8 && pathFoundPlayer == true )
					{
						--playerLives;
						std::cout << "AI has been alerted!" << std::endl;
						pathFoundPlayer = false;
						pathLostPlayer = true;
						std::cout << playerLives << " Lives left!" << std::endl;
					}

					if ( path.size() >= 9 && pathLostPlayer == true )
					{
						std::cout << "AI has lost sight of the player!" << std::endl;
						pathLostPlayer = false;
					}

					_end = -1;
				}

				--timer;   // Reduce timer
			}

			if ( setBreadth == false )   // If set breadth is set to false then run the best-first algorithm
			{
				std::vector<Node*> path = BestFirst(_startNode, _endNode, _squares);   // Calls the best-first pathfinding function and returns the path to a vector called 'path'

				if ( timer < 0 )   // If timer is less than 0 then draw path
				{
					if ( path.size() <= 6 )
					{
						for (Node* drawnNode : path)   // Loops through the path vector
						{
							drawnNode->DEBUG_ChangeImage(_renderer);   // Replaces all the nodes with a different image
						}

						std::cout << "AI has spotted the player!" << std::endl;
						pathFoundPlayer = true;
					}

					if ( path.size() >= 7 && path.size() <= 8 && pathFoundPlayer == true )
					{
						--playerLives;
						std::cout << "AI has been alerted!" << std::endl;
						pathFoundPlayer = false;
						pathLostPlayer = true;
						std::cout << playerLives << " Lives left!" << std::endl;
					}

					if ( path.size() >= 9 && pathLostPlayer == true )
					{
						std::cout << "AI has lost sight of the player!" << std::endl;
						pathLostPlayer = false;
					}

					_end = -1;
				}

				--timer;   // Reduces timer
			}
		}

		// This if statement handles player collisions with placed non walkable blocks
		
		if ( mapdrawn == true )   // If the map has been drawn then allow the player to move
		{
			if ( player )   // If the player exists
			{
				float previousX = player->GetX();   // Set the current X position to the previous
				float previousY = player->GetY();   // Set the current Y position to the previous

				player->Update();   // Update players position

				for (int i = 0; i < _squares.size(); ++i)   // Loop through the squares vector that holds the map
				{
					if ( player->GetX() == _squares[i]->getRect().x && player->GetY() == _squares[i]->getRect().y && _squares[i]->GetWalkable() == false )
					{
						player->SetX(previousX);   // Set X position to previous one
						player->SetY(previousY);   // Set Y position to previous one
					}
				}
			}
		}

		SDL_RenderClear(_renderer);   // Clears the renderer

		if ( searching == true )   // If searching is true
		{
			manager.update();   // Updates the manager and changes game state
		}

		for (Node* square : _squares)   // Drawing the amount of squares on the square vector
		{
			square->Draw();
		}

		if ( mapdrawn == false )   // If mapdrawn is false
		{
			ready_up->Draw();   // Draw the ready up button
		}

		if ( mapdrawn == true )   // If mapdrawn is true
		{
			player->Draw();   // Draw players position
		}

		if ( playerLives == 5 )
		{
			five_lives->Draw();
		}

		if ( playerLives == 4 )
		{
			four_lives->Draw();
		}

		if ( playerLives == 3 )
		{
			three_lives->Draw();
		}

		if ( playerLives == 2 )
		{
			two_lives->Draw();
		}

		if ( playerLives == 1 )
		{
			one_life->Draw();
		}

		if ( playerLives <= 0 )   // If the players lives have reached zero
		{
			player->~Player();   // Deconstructs the player
			one_life->~Sprite();   // Deconstructs the one_life sprite
			game_over->Draw();   // Draw the game over
			gameOver = true;   // Set game over to true
		}

		SDL_RenderPresent(_renderer);   //Update the screen
	}

	for (Node* square : _squares)   // Deleting the amount of squares on the square vector
	{
		delete square;
	}

	_squares.clear();   // Clears the vector
}

void MapLoader(int const _gridHeight, int const _gridWidth, int _offset, SDL_Renderer* _renderer, std::vector<Node*>& _squares)
{
	std::ifstream inFile("mapgrid.txt");   // Loads the correct text file into the stream

	if ( inFile.is_open() )
	{
		for (int x = 0; x < _gridWidth; ++x)   // Loop for creating multiple squares of the screen width
		{
			for (int y = 0; y < _gridHeight; ++y)   // Loop for creating multiple squares of the screen height
			{
				char b;

				do   // Goes through the text file until it reaches the end of the file
				{
					b = (char)inFile.get();
				}
				while (b == '\n');

				Node* square = new Node();   // Initialising a pointer for the square node

				switch ( b )   // Drawing the grid at the correct positions
				{
				case '0':   // If the number is 0 draw a grass block
					square = new Node(_renderer, "Block_Template.bmp", x * _offset, y * _offset, 50, 50);
					break;

				case '1':   // If the number is 1 draw a dirt block
					square = new Node(_renderer, "Block_Template2.bmp", x * _offset, y * _offset, 50, 50);
					break;

				case '2':   // If the number is 2 draw a water block
					square = new Node(_renderer, "Block_Template3.bmp", x * _offset, y * _offset, 50, 50);
					break;

				case '3':   // If the number is 3 draw a bridge block
					square = new Node(_renderer, "Block_Template4.bmp", x * _offset, y * _offset, 50, 50);
					break;

				default:
					break;
				}

				_squares.push_back(square);   // Pushing the squares onto the vector
			}
		}
	}

	inFile.close();
}

std::vector<Node*> BreadthFirst(std::vector<Node*>& _squares, int _start, int _end)
{
	for (Node* node : _squares)   // Loops through the nodes in the square vector
	{
		node->Reset();   // Resets all the nodes
	}

	std::vector<Node*> openList;   // Creates a vector called openList

	Node* startNode = _squares.at(_start);   // Passes in left click x and y as start node
	Node* endNode = _squares.at(_end);   // Passes in right click x and y as end node

	openList.push_back(startNode);   // Pushes back the start node into the openList vector

	bool endFound = false;   // Sets a bool variable called endFound
	Node* currentNode = NULL;   // Sets the current node to NULL because it doesnt have a parent

	int i = 0;   // Initialises an integer to 0

	while (!endFound && openList.size() > 0)   // While end has not been found and the openList still has vectors in it
	{
		i++;   // Iterate the i variable
		currentNode = openList.front();   // Set the current node to the front of the list
		openList.erase(openList.begin());   // Erase the front of the list

		if ( currentNode == endNode )   // If the current node is the end node then
		{
			endFound = true;   // Set endFound to true
			break;   // Leave pathfinding function
		}

		for (int i = 0; i < currentNode->GetNeighbours().size(); ++i)   // Loop through the current nodes neighbours
		{
			Node* neighbour = currentNode->GetNeighbours().at(i);   // Create a neighbour node as the node its currently looking at
			bool found = false;   // Set found to false

			for (size_t o = 0; o < openList.size(); ++o)   // Loop through the current open list
			{
				if ( openList.at(o) == neighbour )   // If the node its looking at is a neighbour
				{
					found = true;   // Set found to true
					break;   // Break out of function
				}
			}

			if ( neighbour->GetHasBeenChecked() == false && found == false && neighbour->GetWalkable() == true ) // If the neighbour hasnt been checked (is false) and found is false and the block is walkable
			{
				openList.push_back(neighbour);   // Push the neighbour node back into the openList
				neighbour->SetParent(currentNode);   // Set the neighbour nodes parent to the current node its looking at
			}
		}

		currentNode->SetHasBeenChecked(true);   // Set the current nodes 'HasBeenChecked' boolean to true
	}

	std::vector<Node*> path;   // Create a vector called path

	while (true)
	{
		currentNode = currentNode->GetParent();   // Set the current node to the current nodes parent (e.g the block before)

		if ( currentNode == NULL )   // If the current node is the end
		{
			break;   // Break out of the function
		}
		else
		{
			path.push_back(currentNode);   // Push the current node onto the path vector
		}

	}

	return path;   // Return the path vector
}

void SetNeighbours(std::vector<Node*>& _squares, int const _gridHeight, int const _gridWidth)
{
	int i = 0;   // Initialise an i variable to count the nodes

	for (int i = 0; i < _squares.size(); ++i)   // Loop through the squares vector
	{
		_squares[i]->m_tileID = i;   // Gives ever square or node a tileID
	}

	for (int x = 0; x < _gridWidth; ++x)   // Loop through the grid width
	{
		for (int y = 0; y < _gridHeight; ++y)   // Loop through the grid height
		{
			Node* current = _squares[i];   // Create a node called 'current' that stores the current node its sat on

			_squares[i]->SetWalkable(true);

			if ( y == 0 )   // If the block is on the top edge
			{
				_squares[i]->GetNeighbours().push_back(_squares[i + 1]);   // Add a block below it into the neighbour vector
			}
			else if ( y == _gridHeight - 1 )   // If the block is on the bottom edge
			{
				_squares[i]->GetNeighbours().push_back(_squares[i - 1]);   // Add a block above it into the neighbour vector
			}
			else   // If the block is in the middle
			{
				_squares[i]->GetNeighbours().push_back(_squares[i + 1]);   // Add a block below it into the neighbour vector
				_squares[i]->GetNeighbours().push_back(_squares[i - 1]);   // Add a block above it into the neighbour vector
			}

			if ( x == 0 )   // If the block is on the left edge
			{
				_squares[i]->GetNeighbours().push_back(_squares[i + _gridHeight]);   // Add a block to the right into the neighbour vector
			}
			else if ( x == _gridWidth - 1 )   // If the block is on the right edge
			{
				_squares[i]->GetNeighbours().push_back(_squares[i - _gridHeight]);   // Add a block to the left into the neighbour vector
			}
			else   // If the block is in the middle
			{
				_squares[i]->GetNeighbours().push_back(_squares[i + _gridHeight]);   // Add a block to the right into the neighbour vector
				_squares[i]->GetNeighbours().push_back(_squares[i - _gridHeight]);   // Add a block to the left into the neighbour vector
			}

			i++;   // Iterate the i variable to go to the next node
		}
	}
}

bool MainMenu(SDL_Renderer* _renderer)
{
	bool isPlaying = false;

	Sprite* main_menu = new Sprite(_renderer, "MainMenu.bmp", 0, 0, 1200, 800);    // Initialise the main menu bmp

	int MouseX = 0;
	int MouseY = 0;

	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);

	SDL_RenderClear(_renderer);

	main_menu->Draw();    // Draw the main menu bmp

	SDL_RenderPresent(_renderer);

	while (isPlaying == false)    // Run while isPlaying variable is set to false
	{
		SDL_PumpEvents();

		int x = SDL_GetMouseState(&MouseX, &MouseY) & 1;

		if ( x == 1 )    // If left mouse button was pressed
		{
			if ( (MouseX > 385) && (MouseX < 819) && (MouseY > 145) && (MouseY < 260) )
			{
				isPlaying = true;
			}

			if ( (MouseX > 219) && (MouseX < 978) && (MouseY > 400) && (MouseY < 473) )
			{
				// Call instructions function
			}

			if ( (MouseX > 416) && (MouseX < 788) && (MouseY > 613) && (MouseY < 718) )
			{
				delete main_menu;    // Deletes the main menu object
				SDL_Quit();    // Closes the SDL graphics window
				exit(0);    // Closes the command console
			}
		}
	}

	return true;
}

bool PathfindingMenu(SDL_Renderer* _renderer)
{
	bool selected = false;

	Sprite* breadth_best_buttons = new Sprite(_renderer, "breadth_best_buttons.bmp", 0, 0, 1200, 800);    // Initialise the main menu bmp

	int MouseX = 0;
	int MouseY = 0;

	breadth_best_buttons->Draw();    // Draw the breadth button bmp here

	SDL_RenderPresent(_renderer);

	while (selected == false)    // Run while isPlaying variable is set to false
	{
		SDL_PumpEvents();

		int x = SDL_GetMouseState(&MouseX, &MouseY) & 1;

		if ( x == 1 )    // If left mouse button was pressed
		{
			if ( (MouseX > 14) && (MouseX < 314) && (MouseY > 726) && (MouseY < 785) )   // Put button coordinates in here
			{
				selected = true;
				return true;
			}

			if ( (MouseX > 1014) && (MouseX < 1178) && (MouseY > 728) && (MouseY < 786) )   // Put button coordinates in here
			{
				selected = true;
				return false;
			}
		}
	}

	return true;
}

std::vector<Node*> BestFirst(Node* _startNode, Node* _endNode, std::vector<Node*>& _squares)
{
	for (Node* node : _squares)   // Reset all the nodes with the reset function
	{
		node->Reset();
	}

	bool endFound = false;   // Creates a bool called end found

	std::vector <Node*> openList;   // Creates a openList node
	std::vector <Node*> closedList;   // Creates a closedList node
	std::vector <Node*> Neighbours;   // Creates a Neighbours node
	Node* currentNode = NULL;   // Sets the current node to NULL because it doesnt have a parent

	openList.push_back(_startNode);   // Pushes back the start node to the open list

	while (openList.size() > 0 && endFound == false)   // Finds the best and closest possible node while looping through the openList
	{
		Node* best = openList.front();   // Creates a best node as the front of the open list
		best->genF(_endNode);   // Generate an F value from the first node to the end node


		for (size_t i = 1; i < openList.size(); ++i)   // Loop through the open list excluding the start
		{
			openList.at(i)->genF(_endNode);   // Generate an F value for the current node its looking at comparing it from the end node
			if ( openList.at(i)->GetF() < best->GetF() )   // If the open list F value is less than the best list F value
			{
				best = openList.at(i);   // Replace the open list position with the best list position
			}
		}

		for (size_t i = 0; i < openList.size(); ++i)   // Loop through the open list
		{
			if ( openList.at(i) == best )   // If the open list is equal to the best node or (closest node to end)
			{
				openList.erase(openList.begin() + i);   // Erase the node in the open list
				break;
			}
		}		

		if ( best == _endNode )   // If best node is equal to the end node then exit
		{
			endFound = true;   // Set end found to true
			break;
		}

		Node* currentNode = best;   // Set the currentNode to the best node

		for (int i = 0; i < currentNode->GetNeighbours().size(); ++i)   // Loop through the current nodes neighbours
		{
			Node* neighbour = currentNode->GetNeighbours().at(i);   // Create a neighbour node as the node its currently looking at
			bool found = false;   // Set found to false

			for (size_t o = 0; o < openList.size(); ++o)   // Loop through the current open list
			{
				if ( openList.at(o) == neighbour )   // If the node its looking at is a neighbour
				{
					found = true;   // Set found to true
					break;   // Break out of function
				}
			}

			if ( neighbour->GetHasBeenChecked() == false && found == false && neighbour->GetWalkable() == true ) // If the neighbour hasnt been checked (is false) and found is false and the block is walkable
			{
				openList.push_back(neighbour);   // Push the neighbour node back into the openList
				neighbour->SetParent(currentNode);   // Set the neighbour nodes parent to the current node its looking at
			}
		}

		currentNode->SetHasBeenChecked(true);   // Set the current nodes 'HasBeenChecked' boolean to true
	}

	std::vector<Node*> path;   // Create a vector called path
	currentNode = _endNode; // Set the current node to the current nodes parent (e.g the block before)

	while (true)
	{			   

		if ( currentNode == NULL )   // If the current node is the end
		{
			break;   // Break out of the function
		}
		else
		{
			path.push_back(currentNode);   // Push the current node onto the path vector
			currentNode = currentNode->GetParent();   // Sets the new current node to the nodes parent
		}
	}

	return path;   // Return the path node
}