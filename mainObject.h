#pragma once
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "function.h"
#include "baseObject.h"
#include "timer_game.h"
#include "ThreatsObject.h"

#define DROP_SPEED 0.8
#define MAX_FALL_SPEED 10
#define SPEED_CHARACTER 8
#define JUMP_VALUE 11
#define BLOOD 9
#define PLUS_BLOOD 10

class threats_object;

class main_object : public baseObject {
public:
	main_object();
	~main_object();


	int is_attacking = 0;
	int attack_range = 50;

	bool loadImg(std::string path, SDL_Renderer* screen);
	void show(std::vector<threats_object*> threats_list, SDL_Renderer* des);
	void handel_input_action(SDL_Event event, SDL_Renderer* screem);

	enum walk_type {
		ATTACK = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2,
		JUMP = 3,
	};

	void set_clip();
	void do_player(map& mapdata, std::vector<threats_object*> threats_list);
	void check_map(map& mapdata);

	void set_map_xy(const int map_x, const int map_y) {
		mapX = map_x;
		mapY = map_y;
	}
	void move_map(map& mapdata);

	int health;

	const int max_health = 100;

	void draw_health_bar(SDL_Renderer* screen);
	void take_damage(int damage);
	void plus_blood();

	bool check_collision( SDL_Rect& rect1,  SDL_Rect& rect2);
	bool check_threat = false;
	bool checkCollision(float playerX, float playerY, float playerWidth, float playerHeight) {
		return (playerX < x_real + width_frame &&
			playerX + playerWidth > x_real &&
			playerY < y_real + height_frame &&
			playerY + playerHeight > y_real);
	}

	SDL_Rect get_rect() {
		SDL_Rect rect;
		rect.x = x_real;
		rect.y = y_real;
		rect.w = width_frame;
		rect.h = height_frame;
		return rect;
	}
	bool isAttacking() {
		return is_attack_;
	}
	void checkThreats(SDL_Renderer* screen,std::vector<threats_object*> threats);
	void show_hit_effect(SDL_Renderer* screen);
	bool set_dead() { return dead; }
	void reset();

private:
	float x_value;
	float y_value;
	float x_real;
	float y_real;

	int width_frame;
	int height_frame;

	SDL_Rect frame_clip[8];
	input input_type;
	int frame;
	int status;
	bool on_ground;

	int mapX;
	int mapY;

	int come_back_time = 0;
	bool is_attack_ = false;
	bool dead = false;
};

#endif