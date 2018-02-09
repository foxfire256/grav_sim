//
// Created by foxfire on 1/20/18.
//

#ifndef GFX_HPP
#define GFX_HPP


class gfx
{
public:
	gfx(){}
	virtual ~gfx(){}
	
	virtual void init(int w, int h){}
	virtual void render(){}
	virtual void deinit(){}
	virtual void resize(int w, int h){}
};


#endif //GFX_HPP
