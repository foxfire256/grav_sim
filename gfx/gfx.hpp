#ifndef GFX_HPP
#define GFX_HPP

#include <cstdint>

class physics;
class physics_simd_test;

class gfx
{
public:
	gfx(){}
	virtual ~gfx(){}
	
	virtual void init(int w, int h){}
	virtual void render(){}
	virtual void deinit(){}
	virtual void resize(int w, int h){}
	virtual void set_simd_test_mode(bool on) { simd_test_mode = on; }

	//physics * get_physics(){return p;}

protected:
	physics* p;
	physics_simd_test* p2;
	bool simd_test_mode;
	uint16_t obj_count;
};


#endif //GFX_HPP
