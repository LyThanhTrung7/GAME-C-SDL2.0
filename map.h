#ifndef MAP_H_
#define MAP_H_

#include "function.h"
#include "baseObject.h"

#define MAX_TILES 20

class tile_map :public baseObject
{
public:
	tile_map() { ; }
	~tile_map() { ; }
};

class game_map {
public:
	game_map() { ; }
	~game_map() { ; }

	void load_map(const char* name);
	void load_tile(SDL_Renderer* screen);
	void draw_map(SDL_Renderer* screen);
	map getmap()const { return game_map_; }

	void set_map(map& mapdata) { game_map_ = mapdata; }

private:
	map game_map_;
	tile_map tile_mat[MAX_TILES];
};


#endif // MAP_H_
