#ifndef _SEARCHSTATE_H
#define _SEARCHSTATE_H

#include "State.h"

class SearchState : public State
{
public:
	SearchState(StateManager* _manager);

	void update();
};

#endif
