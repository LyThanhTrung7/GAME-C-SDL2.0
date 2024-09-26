#include "ThreatsObject.h"


threats_object::threats_object() {
	width_frame = 0;
	height_frame = 0;
	value_x = 0.0;
	value_y = 0.0;
	real_x = 0.0;
	real_y = 0.0;
	on_ground  = false;
	come_back_time = 0;
	frame = 0;
	threat_type = 0;
	map_x = 0;
	map_y = 0;
	animation_a = 0;
	animation_b = 0;
	input_type.left = 0;
	type_move = STATIC_THREAT;
	//health = 100;
}

threats_object::~threats_object() {

}

bool threats_object::load_img_threat(std::string path, SDL_Renderer* screen) {

	bool ret = baseObject::LoadImg(path, screen);
	if (ret) {
		width_frame = rect_.w / 8;
		height_frame = rect_.h;
	}
	return ret;
}

void threats_object::set_clip() {
	if (width_frame > 0 && height_frame > 0) {
		frame_clip[0].x = 0;
		frame_clip[0].y = 0;
		frame_clip[0].w = width_frame;
		frame_clip[0].h = height_frame;

		frame_clip[1].x = width_frame;
		frame_clip[1].y = 0;
		frame_clip[1].w = width_frame;
		frame_clip[1].h = height_frame;

		frame_clip[2].x = 2 * width_frame;
		frame_clip[2].y = 0;
		frame_clip[2].w = width_frame;
		frame_clip[2].h = height_frame;

		frame_clip[3].x = 3 * width_frame;
		frame_clip[3].y = 0;
		frame_clip[3].w = width_frame;
		frame_clip[3].h = height_frame;

		frame_clip[4].x = 4 * width_frame;
		frame_clip[4].y = 0;
		frame_clip[4].w = width_frame;
		frame_clip[4].h = height_frame;

		frame_clip[5].x = 5 * width_frame;
		frame_clip[5].y = 0;
		frame_clip[5].w = width_frame;
		frame_clip[5].h = height_frame;

		frame_clip[6].x = 6 * width_frame;
		frame_clip[6].y = 0;
		frame_clip[6].w = width_frame;
		frame_clip[6].h = height_frame;

		frame_clip[7].x = 7 * width_frame;
		frame_clip[7].y = 0;
		frame_clip[7].w = width_frame;
		frame_clip[7].h = height_frame;
	}
}

void threats_object::show(SDL_Renderer* des, main_object* object) {

	if (come_back_time == 0) {
		rect_.x = real_x - map_x;
		rect_.y = real_y - map_y;
		
		    if (object->checkCollision(real_x, real_y, width_frame, height_frame))  
				show_hit_effect(des);
			  frame++;
			if (frame >= 8) frame = 0;

			SDL_Rect* current = &frame_clip[frame];
			SDL_Rect rectqual = { rect_.x,rect_.y,width_frame,height_frame };
			SDL_RenderCopy(des, p_object_, current, &rectqual);
		}

	}
void threats_object::do_player(map& map) {

	if (come_back_time == 0) {
		value_x = 0;
		value_y += 0.8;
		
		if (value_y >= MAX_FALL_THREATS) value_y = MAX_FALL_THREATS;

		if (input_type.left == 1) value_x -= THREAT_SPEED;
		if (input_type.right == 1) value_x += THREAT_SPEED;

		check_map(map);
	}
	else if(come_back_time >0){
		come_back_time--;
		if (come_back_time == 0) {
			value_x = 0;
			value_y = 0;
			if (real_x >= 256) {
				real_x -= 256;
				animation_a -= 256;
				animation_b -= 256;
			}
			else real_x = 0;
			real_y = 0;
			come_back_time = 0;
			input_type.left = 1;
		}
	}

}

void threats_object::check_map(map& mapdata) {

	int x1 = 0; // left
	int x2 = 0; //right
	int y1 = 0; // down
	int y2 = 0; // up

	// check horizontal
	int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;

	x1 = (real_x + value_x) / TILE_SIZE;
	x2 = (real_x + value_x + width_frame - 1) / TILE_SIZE;

	y1 = (real_y) / TILE_SIZE;
	y2 = (real_y + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (value_x > 0) {
			int value1 = mapdata.tile[y1][x2];
			int value2 = mapdata.tile[y2][x2];

			if ((value1 != 0 && value1 != BLOOD) || (value2 != 0 && value1 != BLOOD)) {
				real_x = x2 * TILE_SIZE - width_frame;
				value_x = 0;
			}
		}
		else if (value_x < 0) {
			int value1 = mapdata.tile[y1][x1];
			int value2 = mapdata.tile[y2][x1];

			if ((value1 != 0 && value1 != BLOOD) || (value2 != 0 && value1 != BLOOD)) {
				real_x = (x1 + 1) * TILE_SIZE;
				value_x = 0;
			}
		}
	}

	//check vertical

	int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
	x1 = (real_x) / TILE_SIZE;
	x2 = (real_x + width_min) / TILE_SIZE;

	y1 = (real_y + value_y) / TILE_SIZE;
	y2 = (real_y + value_y + height_frame - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (value_y > 0) {
			int value1 = mapdata.tile[y2][x1];
			int value2 = mapdata.tile[y2][x2];
			   if ((value1 != 0 && value1 != BLOOD) || (value2 != 0 && value1 != BLOOD)) {
					real_y = y2 * TILE_SIZE - height_frame;
					value_y = 0;
					on_ground = true;
				}
		}
		else if (value_y < 0) {
			int value1 = mapdata.tile[y1][x1];
			int value2 = mapdata.tile[y1][x2];
			if ((value1 != 0 && value1 != BLOOD) || (value2 != 0 && value1 != BLOOD)) {
					real_y = (y1 + 1) * TILE_SIZE;
					value_y = 0;
				}
		}
	}
	real_x += value_x;
	real_y += value_y;
	if (real_x < 0) real_x = 0;
	if (real_x + width_frame > mapdata.max_x) real_x = mapdata.max_x - width_frame - 1;

	//die
	if (real_y > mapdata.max_y) come_back_time = 30;
}

void threats_object::img_move_type(SDL_Renderer* screen) {
     
	if (type_move == STATIC_THREAT) {
		load_img_threat("image/threat/Flame.png", screen);
	}else {
		if (on_ground == true) {
			if (real_x > animation_b) {
				input_type.left = 1;
				input_type.right = 0;
				switch (threat_type)
				{
				case 0:
					load_img_threat("image/threat/wolf_white_walk_left.png", screen);
					break;
				case 1:
					load_img_threat("image/threat/wolf_black_walk_left.png", screen);
					break;
				case 2:
					load_img_threat("image/threat/snake_walk_left.png", screen);
					break;
				}
			}
			else if (real_x < animation_a) {
				input_type.left = 0;
				input_type.right = 1;
				switch (threat_type)
				{
				case 0:
					load_img_threat("image/threat/wolf_white_walk.png", screen);
					break;
				case 1:
					load_img_threat("image/threat/wolf_black_walk.png", screen);
					break;
				case 2:
					load_img_threat("image/threat/snake_walk.png", screen);
					break;
				}
			}
		}
	}

}

void threats_object::show_hit_effect(SDL_Renderer* screen) {
	if (type_move == STATIC_THREAT) {
		load_img_threat("image/threat/Flame.png", screen);
		if (frame >= 8) frame = 0;
	}
	else {
		switch (threat_type)
		{
		case 0:
			load_img_threat("image/threat/1Attack_wolf_white.png", screen);
			break;
		case 1:
			load_img_threat("image/threat/2Attack_black.png", screen);
			break;
		case 2:
			load_img_threat("image/threat/snake_Attack_2.png", screen);
			break;
		default:
			break;
		}
		if (frame >= 8) frame = 0;
	}
	SDL_Rect* current_clip = &frame_clip[frame];
	SDL_Rect render_quad = { rect_.x, rect_.y, width_frame, height_frame };
	SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);

}

