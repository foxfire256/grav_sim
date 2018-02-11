//
// Created by foxfire on 1/20/18.
//

#ifndef OT1_SDL_WINDOW_HPP
#define OT1_SDL_WINDOW_HPP

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

class gfx;
class gfx_factory;

class sdl_window
{
public:
	sdl_window();
	
	virtual ~sdl_window();
	
	void init();
	void deinit();
	void render();
	void resize(int w, int h);
	int main_loop();
	
private:
	SDL_Window *window;
	SDL_GLContext context;
	int done;
	
	gfx *g;
	gfx_factory *gf;
};


#endif //OT1_SDL_WINDOW_HPP
