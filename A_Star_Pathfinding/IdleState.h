#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "State.h"

class IdleState : public State
{
private:
	float timer;

public:
	IdleState(StateManager* _manager);
	void update();
};

#endif