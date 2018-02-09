 
#include "opengl_window.hpp"

class qt_window : public opengl_window
{
public:
	qt_window();
	
	void render() override;
	
	void initialize() override;
};