#ifndef TIMER_GAME_H  
#define TIMER_GAME_H

#include "function.h"

class timer_game {

public:
	timer_game();
	~timer_game();

	void start();
	void stop();
	void pause();
	void unpause();

	int get_ticks();

	bool isStart();
	bool isPause();

private:
	int start_tick;
	int pause_tick;

	bool is_pause;
	bool is_started;

};


#endif // !TIMER_GAME_H_
