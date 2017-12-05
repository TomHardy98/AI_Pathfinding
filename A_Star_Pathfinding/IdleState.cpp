#include "IdleState.h"
#include "StateManager.h"
#include "SearchState.h"
#include <iostream>
#include <Windows.h>

IdleState::IdleState(StateManager* _manager) : State(_manager)
{
	timer = 150;
}

void IdleState::update()
{
	if (timer <= 0)
	{
		std::cout << "No longer idle, changing to searching state" << std::endl;
		Sleep(100);
		_manager->setState(new SearchState(_manager));
	}
	else
	{
		std::cout << "Idle" << std::endl;
		timer--;
	}
}