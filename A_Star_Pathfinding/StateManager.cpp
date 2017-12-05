#include "StateManager.h"
#include "State.h"

#include <iostream>

StateManager::StateManager()
{
	currentState = NULL;
}

void StateManager::setState(State* _state)
{
	if (currentState != NULL)
	{
		delete currentState;
	}

	currentState = _state;
}

void StateManager::update()
{
	if (currentState != NULL)
	{
		currentState->update();
	}
}