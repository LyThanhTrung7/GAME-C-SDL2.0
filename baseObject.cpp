#include "baseObject.h"

baseObject::baseObject() {
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}
baseObject::~baseObject() {

}

bool baseObject::LoadImg(std::string path, SDL_Renderer* screen) {
	SDL_Texture* newText = NULL;
	SDL_Surface* loadSur = IMG_Load(path.c_str());

	if (loadSur != NULL) {
		SDL_SetColorKey(loadSur, SDL_TRUE, SDL_MapRGB(loadSur->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)); // delete font
		newText = SDL_CreateTextureFromSurface(screen, loadSur);
		if (newText != NULL) {
			rect_.w = loadSur->w;
			rect_.h = loadSur->h;
		}
		SDL_FreeSurface(loadSur);
	}
	p_object_ = newText;
	return p_object_ != NULL;
}

void baseObject::Render(SDL_Renderer* des, const SDL_Rect* clip, const SDL_Rect* destinationRect) {

	SDL_RenderCopy(des, p_object_, clip, destinationRect);
}

void baseObject::Free() {
	if (p_object_ != NULL) {
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}