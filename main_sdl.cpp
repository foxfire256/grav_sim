
#include "sdl/sdl_window.hpp"

int main(int argc, char **argv)
{
	sdl_window w;
	
	w.init();
	while(!w.main_loop())
		w.render();
	
	w.deinit();
	return 0;
}

