
#include <QtGui/QGuiApplication>

#include "qt/qt_window.hpp"
#include "phy/physics.hpp"

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	
	physics ph;
	ph.init(8);

	qt_window window;
	window.show();
	
	window.setAnimating(true);

	fflush(stdout);

	return app.exec();
}

 
