//
// Created by foxfire on 1/20/18.
//

#include "gfx_opengl.hpp"

#include <cstdio>
#include <string>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "fox/counter.hpp"
#include "fox/gfx/eigen_opengl.hpp"
#include "fox/obj_model_loader.h"

#ifdef __ANDROID__
std::string data_root = "/sdcard/opengl_testing1";
#elif __APPLE__
std::string data_root = "/Users/foxfire/dev/opengl_testing1";
#elif _WIN32
std::string data_root = "C:/dev/opengl_testing1";
#else // Linux
std::string data_root = "/home/foxfire/dev/opengl_testing1";
#endif

#define print_opengl_error() print_opengl_error2((char *)__FILE__, __LINE__)
int print_opengl_error2(char *file, int line);

gfx_opengl::gfx_opengl() : gfx()
{

}

gfx_opengl::~gfx_opengl()
{

}

void gfx_opengl::init(int w, int h)
{
	win_w = w;
	win_h = h;
	
	// init glew first
	glewExperimental = GL_TRUE; // Needed in core profile
	if(glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		exit(-1);
	}
	
	// HACK: to get around initial glew error with core profiles
	GLenum gl_err = glGetError();
	while(gl_err != GL_NO_ERROR)
	{
		//printf("glError in file %s @ line %d: %s (after glew init)\n",
		//	(char *)__FILE__, __LINE__, gluErrorString(gl_err));
		gl_err = glGetError();
	}
	
	print_opengl_error();
	
	print_info();
	
	// init basic OpenGL stuff
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND); // alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	// OpenGL 3.2 core requires a VAO to be bound to use a VBO
	// WARNING: GLES 2.0 does not support VAOs
	glGenVertexArrays(1, &default_vao);
	glBindVertexArray(default_vao);
	
	// initialize some defaults
	trans = {0.0f, 0.0f, 0.0f};
	rot = {0.0f, 0.0f, 0.0f};
	
	eye = Eigen::Vector3f(0.0f, 0.0f, 3.0f);
	target = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	
	fox::gfx::look_at(eye, target, up, V);
	
	color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	La = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ls = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ld = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	rot = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	trans = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	scale = 1.0f;
	light_pos = Eigen::Vector4f(eye[0], eye[1], eye[2], 1.0f);
	M = Eigen::Matrix4f::Identity();
	fox::gfx::perspective(65.0f, (float)w / (float)h, 0.01f, 40.0f, P);
	
	MV = V * M;
	
	normal_matrix(0, 0) = MV(0, 0);
	normal_matrix(0, 1) = MV(0, 1);
	normal_matrix(0, 2) = MV(0, 2);
	
	normal_matrix(1, 0) = MV(1, 0);
	normal_matrix(1, 1) = MV(1, 1);
	normal_matrix(1, 2) = MV(1, 2);
	
	normal_matrix(2, 0) = MV(2, 0);
	normal_matrix(2, 1) = MV(2, 1);
	normal_matrix(2, 2) = MV(2, 2);
	
	MVP = P * MV;
	
	load_shaders();
	
	// load a mesh
	mesh = (OBJ_MODEL *)malloc(sizeof(OBJ_MODEL));
	if(mesh == nullptr)
	{
		printf("Failed to allocate memory for obj model\n");
		exit(-1);
	}
	std::string mesh_file = data_root + "/meshes/icosphere.obj";
	obj_model_load(mesh_file.c_str(), mesh);
	if(obj_model_check_arrays(mesh))
		obj_model_gl_arrays(mesh);
	obj_model_print_info(mesh);
	printf("Model has %d verticies\n", mesh->vert_count);
	printf("Vertex data takes up %.3f MB\n", (float)mesh->vert_count * 12 /
											 (1024 * 1024));
	printf("Normal data takes up %.3f MB\n", (float)mesh->vert_count * 12 /
											 (1024 * 1024));
	
	// see if we have a material file to load uniforms from
	if(mesh->mtl)
	{
		Ka = Eigen::Vector3f(mesh->mtl->Ka[0], mesh->mtl->Ka[1], mesh->mtl->Ka[2]);
		Ks = Eigen::Vector3f(mesh->mtl->Ks[0], mesh->mtl->Ks[1], mesh->mtl->Ks[2]);
		Kd = Eigen::Vector3f(mesh->mtl->Kd[0], mesh->mtl->Kd[1], mesh->mtl->Kd[2]);
		// TODO: make sure Ns is what we want for shininess
		shininess = mesh->mtl->Ns;
		
		//this->texture_filename = m->mtl->texture_filename;
	}
		// otherwise create some defaults at least
	else
	{
		Ka = Eigen::Vector3f(0.3f, 0.3f, 0.3f);
		Ks = Eigen::Vector3f(0.1f, 0.1f, 0.1f);
		Kd = Eigen::Vector3f(0.6f, 0.6f, 0.6f);
		shininess = 5.0f;
	}
	
	// create VBOs
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vert_count * 3,
				 mesh->v, GL_STATIC_DRAW);
	print_opengl_error();
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vert_count * 3,
				 mesh->vn, GL_STATIC_DRAW);
	print_opengl_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// set shader uniforms and arrays
	int u;
	glUseProgram(shader_id);
	u = glGetUniformLocation(shader_id, "light_pos");
	glUniform4fv(u, 1, light_pos.data());
	u = glGetUniformLocation(shader_id, "La");
	glUniform3fv(u, 1, La.data());
	u = glGetUniformLocation(shader_id, "Ls");
	glUniform3fv(u, 1, Ls.data());
	u = glGetUniformLocation(shader_id, "Ld");
	glUniform3fv(u, 1, Ld.data());
	u = glGetUniformLocation(shader_id, "color");
	glUniform4fv(u, 1, color.data());
	u = glGetUniformLocation(shader_id, "MVP");
	glUniformMatrix4fv(u, 1, GL_FALSE, MVP.data());
	u = glGetUniformLocation(shader_id, "MV");
	glUniformMatrix4fv(u, 1, GL_FALSE, MV.data());
	u = glGetUniformLocation(shader_id, "normal_matrix");
	glUniformMatrix3fv(u, 1, GL_FALSE, normal_matrix.data());
	u = glGetUniformLocation(shader_id, "shininess");
	glUniform1f(u, shininess);
	u = glGetUniformLocation(shader_id, "Ka");
	glUniform3fv(u, 1, Ka.data());
	u = glGetUniformLocation(shader_id, "Ks");
	glUniform3fv(u, 1, Ks.data());
	u = glGetUniformLocation(shader_id, "Kd");
	glUniform3fv(u, 1, Kd.data());
	print_opengl_error();
	
	// start counters
	update_counter = new fox::counter();
	fps_counter = new fox::counter();
	
	rot_vel = 16.0f;
}

void gfx_opengl::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float dt = update_counter->update();
	rot[1] = rot_vel * dt;
	M = M * Eigen::AngleAxisf(rot[0] / 180.0f * (float)M_PI,
							  Eigen::Vector3f::UnitX());
	M = M * Eigen::AngleAxisf(rot[1] / 180.0f * (float)M_PI,
							  Eigen::Vector3f::UnitY());
	M = M * Eigen::AngleAxisf(rot[2] / 180.0f * (float)M_PI,
							  Eigen::Vector3f::UnitZ());
	
	MV = V * M;
	
	normal_matrix(0, 0) = MV(0, 0);
	normal_matrix(0, 1) = MV(0, 1);
	normal_matrix(0, 2) = MV(0, 2);
	
	normal_matrix(1, 0) = MV(1, 0);
	normal_matrix(1, 1) = MV(1, 1);
	normal_matrix(1, 2) = MV(1, 2);
	
	normal_matrix(2, 0) = MV(2, 0);
	normal_matrix(2, 1) = MV(2, 1);
	normal_matrix(2, 2) = MV(2, 2);
	
	MVP = P * MV;
	
	glUseProgram(shader_id);
	int u;
	u = glGetUniformLocation(shader_id, "MVP");
	glUniformMatrix4fv(u, 1, GL_FALSE, MVP.data());
	u = glGetUniformLocation(shader_id, "MV");
	glUniformMatrix4fv(u, 1, GL_FALSE, MV.data());
	u = glGetUniformLocation(shader_id, "normal_matrix");
	glUniformMatrix3fv(u, 1, GL_FALSE, normal_matrix.data());
	GLint vertex_loc, normal_loc;
	vertex_loc = glGetAttribLocation(shader_id, "vertex");
	normal_loc = glGetAttribLocation(shader_id, "normal");
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glEnableVertexAttribArray(vertex_loc);
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glEnableVertexAttribArray(normal_loc);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, mesh->vert_count);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gfx_opengl::resize(int w, int h)
{
	win_w = w;
	win_h = h;
	
	glViewport(0, 0, win_w, win_h);
	fox::gfx::perspective(65.0f, (float)win_w / (float)win_h, 0.01f, 40.0f, P);
	//fox::gfx::ortho(0.0f, (float)win_w, 0.0f, (float)win_h, -10.0f, 10.0f, P);
	
	MVP = P * MV;
	
	glUseProgram(shader_id);
	int u;
	u = glGetUniformLocation(shader_id, "MVP");
	glUniformMatrix4fv(u, 1, GL_FALSE, MVP.data());
}

void gfx_opengl::deinit()
{
	// TODO: can this be freed earlier?
	if(shader_vert_id != 0)
		glDeleteShader(shader_vert_id);
	// TODO: can this be freed earlier?
	if(shader_frag_id != 0)
		glDeleteShader(shader_frag_id);
	if(shader_id != 0)
		glDeleteProgram(shader_id);
	
	glDeleteBuffers(1, &vertex_vbo);
	glDeleteBuffers(1, &normal_vbo);
	
	// TODO: this probably can be done after the VBOs are created
	obj_model_free(mesh);
	
	delete update_counter;
	delete fps_counter;
}

void gfx_opengl::load_shaders()
{
	print_opengl_error();
	
	// load shaders
	std::string fname = data_root + "/shaders/smooth_ads_v150.vert";
	FILE *f;
	uint8_t *vert_data, *frag_data;
	long fsize, vsize;
	long ret, result;
	f = fopen(fname.c_str(), "rt");
	if(f == NULL)
	{
		printf("ERROR couldn't open shader file %s\n", fname.c_str());
		exit(-1);
	}
	// find the file size
	fseek(f, 0, SEEK_END);
	vsize = ftell(f);
	rewind(f);
	
	vert_data = (uint8_t *)malloc(sizeof(uint8_t) * vsize);
	if(vert_data == nullptr)
	{
		printf("Failed to allocate vertex shader memory\n");
		exit(-1);
	}
	result = fread(vert_data, sizeof(uint8_t), vsize, f);
	if(result != vsize)
	{
		printf("ERROR: loading shader: %s\n", fname.c_str());
		printf("Expected %d bytes but only read %d\n", vsize, result);
		
		fclose(f);
		free(vert_data);
		exit(-1);
	}
	// TODO: is this really a good idea?
	vert_data[vsize - 1] = '\0';
	fclose(f);
	
	fname = data_root + "/shaders/smooth_ads_v150.frag";
	f = fopen(fname.c_str(), "rt");
	if(f == NULL)
	{
		printf("ERROR couldn't open shader file %s\n", fname.c_str());
		exit(-1);
	}
	// find the file size
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);
	
	frag_data = (uint8_t *)malloc(sizeof(uint8_t) * fsize);
	if(frag_data == nullptr)
	{
		printf("Failed to allocate vertex shader memory\n");
		exit(-1);
	}
	result = fread(frag_data, sizeof(uint8_t), fsize, f);
	if(result != fsize)
	{
		printf("ERROR: loading shader: %s\n", fname.c_str());
		printf("Expected %d bytes but only read %d\n", fsize, result);
		
		fclose(f);
		free(frag_data);
		exit(-1);
	}
	// TODO: is this really a good idea?
	frag_data[fsize - 1] = '\0';
	fclose(f);
	
	// actually create the shader
	shader_vert_id = glCreateShader(GL_VERTEX_SHADER);
	if(shader_vert_id == 0)
	{
		printf("Failed to create GL_VERTEX_SHADER!\n");
		exit(-1);
	}
	glShaderSource(shader_vert_id, 1, (GLchar **)&vert_data, NULL);
	glCompileShader(shader_vert_id);
	free(vert_data);
	
	// print shader info log
	int length = 0, chars_written = 0;
	char *info_log;
	glGetShaderiv(shader_vert_id, GL_INFO_LOG_LENGTH, &length);
	// WTF? so why was 4 used?
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader info log!\n",
				   length);
			exit(-1);
		}
		
		glGetShaderInfoLog(shader_vert_id, length, &chars_written, info_log);
		
		printf("Shader info log: %s\n", info_log);
		
		free(info_log);
	}
	
	// actually create the shader
	shader_frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	if(shader_frag_id == 0)
	{
		printf("Failed to create GL_VERTEX_SHADER!\n");
		exit(-1);
	}
	glShaderSource(shader_frag_id, 1, (GLchar **)&frag_data, NULL);
	glCompileShader(shader_frag_id);
	free(frag_data);
	
	// print shader info log
	length = 0;
	chars_written = 0;
	glGetShaderiv(shader_frag_id, GL_INFO_LOG_LENGTH, &length);
	// WTF? so why was 4 used?
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader info log!\n",
				   length);
			exit(-1);
		}
		
		glGetShaderInfoLog(shader_frag_id, length, &chars_written, info_log);
		
		printf("Shader info log: %s\n", info_log);
		
		free(info_log);
	}
	
	// create the shader program
	shader_id = glCreateProgram();
	if(shader_id == 0)
	{
		printf("Failed at glCreateProgram()!\n");
		exit(-1);
	}
	
	glAttachShader(shader_id, shader_vert_id);
	glAttachShader(shader_id, shader_frag_id);
	
	glLinkProgram(shader_id);
	
	glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &length);
	
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader program info log!\n",
				   length);
			exit(-1);
		}
		else
		{
			printf("Shader program info log:\n");
		}
		
		glGetProgramInfoLog(shader_id, length, &chars_written, info_log);
		
		printf("%s\n", info_log);
		
		free(info_log);
	}
	
	print_opengl_error();
}

void gfx_opengl::print_info()
{
	printf("GLEW library version %s\n", glewGetString(GLEW_VERSION));
	
	if(glewIsSupported("GL_VERSION_5_1"))
	{
		printf("GLEW supported GL_VERSION_5_1\n");
	}
	else if(glewIsSupported("GL_VERSION_5_0"))
	{
		printf("GLEW supported GL_VERSION_5_0\n");
	}
	else if(glewIsSupported("GL_VERSION_4_8"))
	{
		printf("GLEW supported GL_VERSION_4_8\n");
	}
	else if(glewIsSupported("GL_VERSION_4_7"))
	{
		printf("GLEW supported GL_VERSION_4_7\n");
	}
	else if(glewIsSupported("GL_VERSION_4_6"))
	{
		printf("GLEW supported GL_VERSION_4_6\n");
	}
	else if(glewIsSupported("GL_VERSION_4_5"))
	{
		printf("GLEW supported GL_VERSION_4_5\n");
	}
	else if(glewIsSupported("GL_VERSION_4_4"))
	{
		printf("GLEW supported GL_VERSION_4_4\n");
	}
	else if(glewIsSupported("GL_VERSION_4_3"))
	{
		printf("GLEW supported GL_VERSION_4_3\n");
	}
	else if(glewIsSupported("GL_VERSION_4_2"))
	{
		printf("GLEW supported GL_VERSION_4_2\n");
	}
	else if(glewIsSupported("GL_VERSION_4_1"))
	{
		printf("GLEW supported GL_VERSION_4_1\n");
	}
	else if(glewIsSupported("GL_VERSION_4_0"))
	{
		printf("GLEW supported GL_VERSION_4_0\n");
	}
	else if(glewIsSupported("GL_VERSION_3_2"))
	{
		printf("GLEW supported GL_VERSION_3_2\n");
	}
	else if(glewIsSupported("GL_VERSION_3_1"))
	{
		printf("GLEW supported GL_VERSION_3_1\n");
	}
	else if(glewIsSupported("GL_VERSION_3_0"))
	{
		printf("GLEW supported GL_VERSION_3_0\n");
	}
	else if(glewIsSupported("GL_VERSION_2_1"))
	{
		printf("GLEW supported GL_VERSION_2_1\n");
	}
	else if(glewIsSupported("GL_VERSION_2_0"))
	{
		printf("GLEW supported GL_VERSION_2_0\n");
	}
	else
	{
		printf("NO GLEW GL_VERSION seems to be supported!\n");
	}
	
	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
		   glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	printf("Eigen version: %d.%d.%d\n", EIGEN_WORLD_VERSION,
		   EIGEN_MAJOR_VERSION,  EIGEN_MINOR_VERSION);
	
	
}

//------------------------------------------------------------------------------
// Returns 1 if an OpenGL error occurred, 0 otherwise.
int print_opengl_error2(char *file, int line)
{
	GLenum gl_err;
	int	ret_code = 0;
	
	gl_err = glGetError();
	while(gl_err != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line,
			gluErrorString(gl_err));
		
		ret_code = 1;
		gl_err = glGetError();
	}
	return ret_code;
}
