
// hacks for NVidia Optimus
// 1 use NVidia, 0 don't unless maybe you set it as default
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

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

