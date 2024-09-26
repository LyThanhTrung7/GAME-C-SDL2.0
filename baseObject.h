#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "function.h"

class baseObject {
public: 
	baseObject();
	~baseObject();

	void set_rec(const int &x, const int &y){
		rect_.x = x;
		rect_.y = y;
	}
	SDL_Rect getRect() const { return rect_; }
	SDL_Texture* getObject() const { return p_object_; }

	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip , const SDL_Rect* destinationRect);
	void Free();

protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;

};


#endif // !BASE_OBJECT_H_

