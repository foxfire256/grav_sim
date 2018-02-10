

#include "qt_window.hpp"

qt_window::qt_window(physics *p) : opengl_window(p)
{

}

void qt_window::render()
{
	opengl_window::render();
}

void qt_window::initialize()
{
	opengl_window::initialize();
}
