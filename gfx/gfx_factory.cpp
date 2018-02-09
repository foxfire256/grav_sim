//
// Created by foxfire on 1/20/18.
//

#include "gfx_factory.hpp"

#include "gfx_opengl.hpp"

gfx * gfx_factory::get_gfx()
{
	return new gfx_opengl();
}
