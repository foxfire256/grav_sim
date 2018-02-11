//
// Created by foxfire on 1/20/18.
//

#ifndef GFX_OPENGL_HPP
#define GFX_OPENGL_HPP

#include "gfx.hpp"

#include <vector>
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
	std::vector<Eigen::Projective3f> MVP;
	std::vector<Eigen::Affine3f> M, MV;
	// TODO: should this be Affine3f ?
	std::vector<Eigen::Matrix3f> normal_matrix;
	// more shader uniforms
	Eigen::Vector4f light_pos, color;
	Eigen::Vector3f La, Ls, Ld;
	Eigen::Vector3f Ka, Ks, Kd;
	float shininess;
	
	GLuint shader_id, shader_vert_id, shader_frag_id;
	GLuint vertex_vbo, normal_vbo;
	
	fox::counter *phy_counter;
	fox::counter *fps_counter;
	fox::counter *perf_counter;

	const static uint8_t perf_array_size = 8;
	double phys_times[perf_array_size];
	double gfx_matrix_times[perf_array_size];
	double render_times[perf_array_size];
	double phys_time;
	double gfx_matrix_time;
	double render_time;
	uint8_t perf_index;
	double total_time;
	
	OBJ_MODEL *mesh;
	
	void print_info();
	void load_shaders();

	void update_matricies();
};


#endif //GFX_OPENGL_HPP
