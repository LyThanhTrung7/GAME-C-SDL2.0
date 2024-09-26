#include "mainObject.h"

 int frame_delay;
 timer_game frame_timer;

main_object::main_object() {
	frame = 0;
	x_real = 0;
	y_real = 0;
	x_value = 0;
	y_value = 0;
	width_frame = 0;
	height_frame = 0;
	status = -1;
	input_type.left = 0;
	input_type.right = 0;
	input_type.attack = 0;
	input_type.jump = 0;
	on_ground = false;
	mapX = 0;
	mapY = 0;
	frame_delay = 120; // delay giữa các frame (ms)
	frame_timer.start(); // bắt đầu đếm thời gian cho frame
	check_threat = false; 
	health = 100;
	is_attack_ = false;
	dead = false;
}

main_object::~main_object() {



}


void main_object::draw_health_bar(SDL_Renderer* screen) {
	// Kích thước và vị trí của thanh máu
	int bar_width = 220;  
	int bar_height = 20;  
	int bar_x = 20;       
	int bar_y = 20;       

	// Tính toán chiều rộng của thanh máu dựa trên lượng máu hiện tại
	int health_width = (health * bar_width) / max_health;

	// Vẽ khung của thanh máu (nền màu đen)
	SDL_Rect outline_rect = { bar_x, bar_y, bar_width, bar_height };
	SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);  
	SDL_RenderFillRect(screen, &outline_rect);

	// Vẽ thanh máu (màu đỏ)
	SDL_Rect health_rect = { bar_x, bar_y, health_width, bar_height };
	SDL_SetRenderDrawColor(screen, 200, 0, 0, 255);  
	SDL_RenderFillRect(screen, &health_rect);

	// Vẽ viền bo góc (màu trắng)
	SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);  // Màu trắng
	SDL_RenderDrawRect(screen, &outline_rect);

	// Tải hình ảnh trái tim
	SDL_Surface* heart_surface = IMG_Load("image/heart.png");  // Đảm bảo đường dẫn đúng

	SDL_Texture* heart_texture = SDL_CreateTextureFromSurface(screen, heart_surface);
	SDL_FreeSurface(heart_surface);

	// Vị trí và kích thước của trái tim
	int heart_width = 30;  
	int heart_height = 30;
	int heart_x = 10;  
	int heart_y = bar_y - 5;

	// Vẽ trái tim
	SDL_Rect heart_rect = { heart_x, heart_y, heart_width, heart_height };
	SDL_RenderCopy(screen, heart_texture, NULL, &heart_rect);

	// Giải phóng texture sau khi vẽ
	SDL_DestroyTexture(heart_texture);
}


void main_object::take_damage(int damage) {
	health -= damage;
	if (health < 0) {
		health = 0;
		dead = true;
	}
	else dead = false;
}

void main_object::plus_blood() {
	if(health <= 100) health += PLUS_BLOOD;
}

bool main_object::loadImg(std::string path, SDL_Renderer* screen) {

	bool ret = baseObject::LoadImg(path, screen);

	if (ret == true) {
		width_frame = rect_.w / 8;
		height_frame = rect_.h;
	}
	return ret;
}

void main_object::set_clip() {

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

static int flag = 0;

void main_object::show(std::vector<threats_object*> threats_list, SDL_Renderer* des) {

	if (on_ground == true) {
		if (flag == 1) {
			status = WALK_LEFT;
		}
		if (flag == 2) status = WALK_RIGHT;
		if (flag == 3)status = ATTACK;

		if (status == WALK_LEFT) {
			loadImg("image//Shinobi//Run_left.png", des);
			if (frame >= 8) frame = 0;
		}
		else if (status == WALK_RIGHT) {
			loadImg("image//Shinobi//Run.png", des);
			if (frame >= 8) frame = 0;
		}
		else if (status == ATTACK) {
				loadImg("image//Shinobi//Attack_1.png", des);
				if (frame >= 8) {
					frame = 0;
					is_attack_ = false;
				}
		}
		checkThreats(des, threats_list);
	}
	 if (status == JUMP) {
		if (flag == 1) loadImg("image//Shinobi//Jump_left.png", des);
		else loadImg("image//Shinobi//Jump.png", des);
	}
	 
	 if (frame_timer.get_ticks() >= frame_delay) {
		 frame_timer.start();
		 if (on_ground==true) {
			 if (input_type.left == 1 || input_type.right == 1 || is_attack_) {
				 frame++;
				 
			 }else  frame = 0;
		 }else  frame = 0;
	 }
	 

	if (come_back_time == 0) {
		rect_.x = x_real - mapX;
		rect_.y = y_real - mapY;

		SDL_Rect* current_clip = &frame_clip[frame];
		SDL_Rect render_quad = { rect_.x,rect_.y, width_frame,height_frame };
		SDL_RenderCopy(des, p_object_, current_clip, &render_quad);

	}
	draw_health_bar(des);
}

void main_object::handel_input_action(SDL_Event event, SDL_Renderer* screen) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_RIGHT:
			input_type.right = 1;
			input_type.left = 0;
			input_type.jump = 0;
			flag = 2;
			input_type.attack = 0;
			is_attack_ = false;
			break;
		case SDLK_LEFT:
			input_type.left = 1;
			flag = 1;
			input_type.right = 0;
			input_type.jump = 0;
			is_attack_ = false;
			break;
		case SDLK_SPACE:
			status = JUMP;
			input_type.left = 0;
			input_type.right = 0;
			input_type.jump = 1;
			input_type.attack = 0;
			is_attack_ = false;
			break;
		case SDLK_d:
			if (!is_attack_) {
				status = ATTACK;
				input_type.attack = 1;
				input_type.left = 0;
				input_type.right = 0;
				input_type.jump = 0;
				flag = 0;
				is_attack_ = true;
			}
			break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		status = 0;
		switch (event.key.keysym.sym) {
		case SDLK_RIGHT:
			input_type.right = 0;
			break;
		case SDLK_LEFT:
			input_type.left = 0;
			break;
		case SDLK_SPACE:
			input_type.jump = 0;
			break;
		case SDLK_d:
			input_type.attack = 0;
			break;
		}
	}
}


void main_object::do_player(map& mapdata, std::vector<threats_object*> threats_list) {

	if (come_back_time == 0) {
		
		x_value = 0;
		y_value += 0.8;

		if (y_value >= MAX_FALL_SPEED) y_value = MAX_FALL_SPEED;

		if (input_type.left == 1) x_value -= SPEED_CHARACTER;
		else if (input_type.right == 1) x_value += SPEED_CHARACTER;
		else if (input_type.jump == 1) {
			if (on_ground == true) {
				y_value = -JUMP_VALUE;
				on_ground = false;
			}
			input_type.jump = 0;
		}

		check_map(mapdata);
		move_map(mapdata);
	}
	if (come_back_time > 0) {
		come_back_time--;
		if (come_back_time == 0) {
			if (x_real > 256) {
				x_real -= 256;
				x_value -= 256;
			}
			else {
				x_real = 0;
			}
			y_real = 0;
			x_value = 0;
			y_value = 0;
		}
	}
}

void main_object::move_map(map& mapdata) {

	mapdata.start_x = x_real - (SCREEN_WIDTH / 2);
	if (mapdata.start_x < 0) mapdata.start_x = 0;

	else if (mapdata.start_x + SCREEN_WIDTH >= mapdata.max_x)
		mapdata.start_x = mapdata.max_x - SCREEN_WIDTH;

	mapdata.start_y = y_real - (SCREEN_HEIGHT / 2);
	if (mapdata.start_y < 0) mapdata.start_y = 0;
	else if (mapdata.start_y + SCREEN_HEIGHT >= mapdata.max_y)
		mapdata.start_y = mapdata.max_y - SCREEN_HEIGHT;

}

void main_object::check_map(map& mapdata) {

	int x1 = 0; // left
	int x2 = 0; //right
	int y1 = 0; // down
	int y2 = 0; // up

	// check horizontal
	int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;

	x1 = (x_real + x_value) / TILE_SIZE;
	x2 = (x_real + x_value + width_frame - 1) / TILE_SIZE;

	y1 = (y_real) / TILE_SIZE;
	y2 = (y_real + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_value > 0) {
			int value1 = mapdata.tile[y1][x2];
			int value2 = mapdata.tile[y2][x2];

			if (value1 == BLOOD || value2 == BLOOD) {
				mapdata.tile[y1][x2] = 0;
				mapdata.tile[y2][x2] = 0;
				plus_blood();
			}else {
				if (value1 != 0 || value2 != 0 ) {
					x_real = x2 * TILE_SIZE - width_frame;
					x_value = 0;
				}
			}
		}
		else if (x_value < 0) {
			int value1 = mapdata.tile[y1][x1];
			int value2 = mapdata.tile[y2][x1];

			if (value1 == BLOOD || value2 == BLOOD) {
				mapdata.tile[y1][x1] = 0;
				mapdata.tile[y2][x1] = 0;
				plus_blood();
			}
			else {
				if (value1 != 0 || value2 != 0) {
					x_real = (x1 + 1) * TILE_SIZE;
					x_value = 0;
				}
			}
		}
	}

	//check vertical

	int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
	x1 = (x_real) / TILE_SIZE;
	x2 = (x_real + width_min) / TILE_SIZE;

	y1 = (y_real + y_value) / TILE_SIZE;
	y2 = (y_real + y_value + height_frame - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (y_value > 0) {
			int value1 = mapdata.tile[y2][x1];
			int value2 = mapdata.tile[y2][x2];

			if (value1 == BLOOD || value2 == BLOOD) {
				mapdata.tile[y2][x1] = 0;
				mapdata.tile[y2][x2] = 0;
				plus_blood();
			}
			else {
				if (mapdata.tile[y2][x1] != 0 || mapdata.tile[y2][x2] != 0) {
					y_real = y2 * TILE_SIZE - height_frame;
					y_value = 0;
					on_ground = true;
				}
			}
		}
		else if (y_value < 0) {
			int value1 = mapdata.tile[y1][x1];
			int value2 = mapdata.tile[y1][x2];

			if (value1 == BLOOD || value2 == BLOOD) {
				mapdata.tile[y1][x1] = 0;
				mapdata.tile[y1][x2] = 0;
				plus_blood();
			}
			else {
				if (mapdata.tile[y1][x1] != 0 || mapdata.tile[y1][x2] != 0) {
					y_real = (y1 + 1) * TILE_SIZE;
					y_value = 0;
				}
			}
		}
	}
	x_real += x_value;
	y_real += y_value;
	if (x_real < 0) x_real = 0;
	if (x_real + width_frame > mapdata.max_x) x_real = mapdata.max_x - width_frame - 1;

	//die
	if (y_real > mapdata.max_y) {
		come_back_time = 30;
		take_damage(25);
	}

}

bool main_object::check_collision(SDL_Rect& rect1, SDL_Rect& rect2){
	if (rect1.x + rect1.w +1  <= rect2.x ||  // rect1 bên trái rect2
		rect1.x +1 >= rect2.x + rect2.w ||  // rect1 bên phải rect2
		rect1.y + rect1.h+1 <= rect2.y ||  // rect1 phía trên rect2
		rect1.y+1  >= rect2.y + rect2.h) {  // rect1 phía dưới rect2
		return false;
	}
	  return true;
}
void main_object::show_hit_effect(SDL_Renderer* screen) {
	loadImg("image//Shinobi//Hurt.png", screen);
	SDL_Rect* current_clip = &frame_clip[frame];
	SDL_Rect render_quad = { rect_.x, rect_.y, width_frame, height_frame };
	SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);

}
void main_object::checkThreats(SDL_Renderer* screen, std::vector<threats_object*> threats) {
	for (auto& threat : threats) {
		if (threat->checkCollision(x_real, y_real, width_frame, height_frame)) {
			x_real -= 10;
			take_damage(1);
		//	show_hit_effect(screen);  
		}
	}
}
void main_object::reset() {
	dead = false;  
	x_real = 0;  
	y_real = 0;
}