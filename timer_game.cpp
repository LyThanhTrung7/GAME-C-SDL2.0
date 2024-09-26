#include "timer_game.h"

timer_game::timer_game(){

	start_tick = 0;
	pause_tick = 0;
	is_pause = false;
	is_started = false;
}

timer_game:: ~timer_game() {

}

void timer_game::start() {
	is_started = true;
	is_pause = false;
	start_tick = SDL_GetTicks();
}

void timer_game::stop() {
	is_started = false;
	is_pause = false;

}
void timer_game::pause() {
	if (is_started == true && is_pause == false) {
		is_pause = true;
		pause_tick = SDL_GetTicks() - start_tick;
	}
}

void timer_game::unpause() {
	if (is_pause==true) {
		is_pause = false;
		start_tick = SDL_GetTicks() - pause_tick;
		pause_tick = 0;
	}
}
int timer_game::get_ticks() {

	if (is_started==true) {
		if (is_pause==true) return pause_tick;
		else return SDL_GetTicks() - start_tick;
	}
	return 0;
}

bool timer_game::isStart() {
	return is_started;
}
bool timer_game::isPause() {
	return is_pause;
}