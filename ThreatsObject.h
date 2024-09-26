#pragma once
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "baseObject.h"
#include "timer_game.h"
#include "mainObject.h"


#define MAX_FALL_THREATS 10
#define BLOOD 11
#define THREAT_SPEED 3


class main_object;

class threats_object:public baseObject{

public:
	threats_object();
	~threats_object();


	void set_value_x(const float valuex) { value_x = valuex; }
	void set_value_y(const float valuey) { value_y = valuey; }
	void set_real_x(const float realx) { real_x = realx; }
	void set_real_y(const float realy) { real_y = realy; }


	float get_value_x() const { return value_x; }
	float get_value_y() const { return value_y; }
	float get_real_x() const { return real_x; }
	float get_real_y() const { return real_y; }

	void set_map_xy(const int mapx, const int mapy) { map_x = mapx; map_y = mapy; }

	void set_clip();
	bool load_img_threat(std::string path, SDL_Renderer* screen);
	void show(SDL_Renderer* des, main_object* object);

	int get_width_frame() const { return width_frame; }
	int get_height_frame() const { return height_frame; }

	void do_player(map& map);
	void check_map(map& map);

	enum typeMove {
		STATIC_THREAT = 0,
	    MOVE_IN_SPACE_THREAT = 1,
	};

	void set_type_move(const int& typemove) { type_move = typemove; }
	void set_animation_real(const int& real_a, const int& real_b) { animation_a = real_a; animation_b = real_b; }
	void set_input_left(const int& ipleft) { input_type.left = ipleft; }
	void img_move_type(SDL_Renderer* screen);


	SDL_Rect get_rect() const {
		SDL_Rect rect;
		rect.x = real_x;  
		rect.y = real_y;  
		rect.w = width_frame;  
		rect.h = height_frame; 
		return rect;
	}

	bool checkCollision(float playerX, float playerY, float playerWidth, float playerHeight) {
		if (playerX < real_x + width_frame &&
			playerX + playerWidth > real_x &&
			playerY < real_y + height_frame &&
			playerY + playerHeight > real_y) {
			is_attack_ = true;
			return true;
		}
		return false;
	}

	void show_hit_effect(SDL_Renderer* screen);
	int threat_type;
	bool is_attack_ = false;

private:
	float value_x;
	float value_y;

	float real_x;
	float real_y;

	int map_x;
	int map_y;

	int width_frame;
	int height_frame;
	SDL_Rect frame_clip[8];
	int frame;

	bool on_ground;
	int come_back_time;

	int type_move;
	int animation_a;
	int animation_b;
	input input_type;
};



#endif // !
