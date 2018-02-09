//
// Created by foxfire on 1/20/18.
//

#ifndef GFX_OPENGL_HPP
#define GFX_OPENGL_HPP

#include "gfx.hpp"

#include <cstdint>

#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace fox
{
	class counter;
}
struct OBJ_MODEL;

class gfx_opengl : public gfx
{
public:
	gfx_opengl();
	
	~gfx_opengl() override;
	
	void init(int w, int h) override;
	
	void render() override;
	
	void resize(int w, int h) override;
	
	void deinit() override;

private:
	int win_w, win_h;
	// an empty vertex array object to bind to
	uint32_t default_vao;
	
	Eigen::Vector3f eye, target, up;
	Eigen::Affine3f V;
	Eigen::Projective3f P;
	// model matrix (specific to the model instance)
	Eigen::Projective3f MVP;
	Eigen::Affine3f M, MV;
	// TODO: should this be Affine3f ?
	Eigen::Matrix3f normal_matrix;
	// more shader uniforms
	Eigen::Vector4f light_pos, color;
	Eigen::Vector3f La, Ls, Ld;
	Eigen::Vector3f Ka, Ks, Kd;
	float shininess;
	Eigen::Vector3f rot, trans;
	float scale;
	
	GLuint shader_id, shader_vert_id, shader_frag_id;
	GLuint vertex_vbo, normal_vbo;
	
	fox::counter *update_counter;
	fox::counter *fps_counter;
	
	float rot_vel;
	
	OBJ_MODEL *mesh;
	
	void print_info();
	void load_shaders();
};


#endif //GFX_OPENGL_HPP
