#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

class State;

class StateManager
{
private:
	State* currentState;

public:
	StateManager();

	void setState(State* _state);
	void update();
};

#endif