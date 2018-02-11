#ifndef GFX_HPP
#define GFX_HPP

class physics;

class gfx
{
public:
	gfx(){}
	virtual ~gfx(){}
	
	virtual void init(int w, int h){}
	virtual void render(){}
	virtual void deinit(){}
	virtual void resize(int w, int h){}
	physics * get_physics(){return p;}

protected:
	physics *p;
};


#endif //GFX_HPP
