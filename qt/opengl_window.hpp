/**
 * This is adapted from the Qt5 OpenGL Window Example
 * http://doc.qt.io/qt-5/qtgui-openglwindow-example.html
 */

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class gfx;

class opengl_window : public QWindow, protected QOpenGLFunctions
{
Q_OBJECT
public:
	explicit opengl_window(QWindow *parent = 0);
	~opengl_window();
	
	virtual void render(QPainter *painter);
	virtual void render();
	
	virtual void resizeEvent(QResizeEvent *ev) override;
	
	virtual void initialize();
	
	void setAnimating(bool animating);

public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event) override;
	
	void exposeEvent(QExposeEvent *event) override;
	
private:
	bool animating;
	
	QOpenGLContext *context;
	QOpenGLPaintDevice *device;
	
	gfx *g;
};
