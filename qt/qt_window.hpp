 
#include "opengl_window.hpp"

class physics;

class qt_window : public opengl_window
{
public:
	qt_window(physics *p);
	
	void render() override;
	
	void initialize() override;
};
