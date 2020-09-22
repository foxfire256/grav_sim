
// hacks for NVidia Optimus
// 1 use NVidia, 0 don't unless maybe you set it as default
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

#include <omp.h>

#include "sdl/sdl_window.hpp"

int main(int argc, char **argv)
{
	omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(6);
	
	sdl_window w;
	
	w.init();
	while(!w.main_loop())
		w.render();
	
	w.deinit();
	return 0;
}

