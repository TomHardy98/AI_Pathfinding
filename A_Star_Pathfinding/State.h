#ifndef _STATE_H
#define _STATE_H

class StateManager;

class State
{
public:
	State(StateManager* _manager);

	virtual ~State();

	virtual void update() = 0;

	State** self;

protected:
	StateManager* _manager;
};

#endif