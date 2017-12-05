#include "SearchState.h"
#include <iostream>
#include <Windows.h>
#include "StateManager.h"

SearchState::SearchState(StateManager* _manager) : State(_manager)
{
	std::cout << "Searching..." << std::endl;
}

void SearchState::update()
{
	return;
}