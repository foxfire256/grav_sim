
#include <QtGui/QGuiApplication>
#include <QSurfaceFormat>

#include "qt/qt_window.hpp"

int main(int argc, char **argv)
{
	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	
	QGuiApplication app(argc, argv);
	
	// https://doc.qt.io/qt-5/qsurfaceformat.html
	QSurfaceFormat format;
	// set the OpenGL version
	//format.setRenderableType(QSurfaceFormat::OpenGL);
	//format.setMajorVersion(4);
	//format.setMinorVersion(1);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	//format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	// 0 = no vertical sync, 1 = vertical sync
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);

	qt_window window;
	window.show();

	window.setAnimating(true);

	return app.exec();
}

 
