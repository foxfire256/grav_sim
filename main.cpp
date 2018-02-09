
#include <QtGui/QGuiApplication>

#include "qt/qt_window.hpp"

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	
	qt_window window;
	window.show();
	
	window.setAnimating(true);
	
	return app.exec();
}

 
