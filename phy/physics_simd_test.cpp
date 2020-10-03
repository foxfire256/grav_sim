#include "physics_simd_test.hpp"

#include <cmath>


physics_simd_test::physics_simd_test()
{
	this->generator = std::mt19937_64(std::random_device{}());

}

physics_simd_test::~physics_simd_test()
{

}

void physics_simd_test::step(double delta_t)
{
	total_time += delta_t;

	// TODO: collision detection
	#pragma omp parallel for
	for(int i = 0; i < obj_count; i++)
	{
		// Euler
		//a[current][i] = accel(x[current][i], i);
		//v[next][i] = a[current][i] * delta_t + v[current][i];
		//x[next][i] = v[next][i] * delta_t + x[current][i];

		// RK4 integration, see doc/grav_sim.tex
		// this is about 4x worse than Euler
		/*
		double xk1[3];
		double xk2[3];
		double xk3[3];
		double xk4[3];

		double vk1[3];
		double vk2[3];
		double vk3[3];
		double vk4[3];

		double ak1[3];
		double ak2[3];
		double ak3[3];
		double ak4[3];
		*/
		std::array<double, 3> xk1;
		std::array<double, 3> xk2;
		std::array<double, 3> xk3;
		std::array<double, 3> xk4;

		std::array<double, 3> vk1;
		std::array<double, 3> vk2;
		std::array<double, 3> vk3;
		std::array<double, 3> vk4;

		std::array<double, 3> ak1;
		std::array<double, 3> ak2;
		std::array<double, 3> ak3;
		std::array<double, 3> ak4;

		xk1 = x[current][i];
		vk1 = v[current][i];
		ak1 = accel(xk1, i);

		xk2[0] = x[current][i][0] + 0.5 * vk1[0] * delta_t;
		xk2[1] = x[current][i][1] + 0.5 * vk1[1] * delta_t;
		xk2[2] = x[current][i][2] + 0.5 * vk1[2] * delta_t;
		vk2[0] = v[current][i][0] + 0.5 * ak1[0] * delta_t;
		vk2[1] = v[current][i][1] + 0.5 * ak1[1] * delta_t;
		vk2[2] = v[current][i][2] + 0.5 * ak1[2] * delta_t;
		ak2 = accel(xk2, i);

		xk3[0] = x[current][i][0] + 0.5 * vk2[0] * delta_t;
		xk3[1] = x[current][i][1] + 0.5 * vk2[1] * delta_t;
		xk3[2] = x[current][i][2] + 0.5 * vk2[2] * delta_t;
		vk3[0] = v[current][i][0] + 0.5 * ak2[0] * delta_t;
		vk3[1] = v[current][i][1] + 0.5 * ak2[1] * delta_t;
		vk3[2] = v[current][i][2] + 0.5 * ak2[2] * delta_t;
		ak3 = accel(xk3, i);

		xk4[0] = x[current][i][0] + vk3[0] * delta_t;
		xk4[1] = x[current][i][1] + vk3[1] * delta_t;
		xk4[2] = x[current][i][2] + vk3[2] * delta_t;
		vk4[0] = v[current][i][0] + ak3[0] * delta_t;
		vk4[1] = v[current][i][1] + ak3[1] * delta_t;
		vk4[2] = v[current][i][2] + ak3[2] * delta_t;
		ak4 = accel(xk4, i);

		v[next][i][0] = v[current][i][0] + (delta_t / 6.0) * (ak1[0] + 2 * ak2[0] + 2 * ak3[0] + ak4[0]);
		v[next][i][1] = v[current][i][1] + (delta_t / 6.0) * (ak1[1] + 2 * ak2[1] + 2 * ak3[1] + ak4[1]);
		v[next][i][2] = v[current][i][2] + (delta_t / 6.0) * (ak1[2] + 2 * ak2[2] + 2 * ak3[2] + ak4[2]);
		x[next][i][0] = x[current][i][0] + (delta_t / 6.0) * (vk1[0] + 2 * vk2[0] + 2 * vk3[0] + vk4[0]);
		x[next][i][1] = x[current][i][1] + (delta_t / 6.0) * (vk1[1] + 2 * vk2[1] + 2 * vk3[1] + vk4[1]);
		x[next][i][2] = x[current][i][2] + (delta_t / 6.0) * (vk1[2] + 2 * vk2[2] + 2 * vk3[2] + vk4[2]);
	}

	current = current ? 0 : 1;
	next = next ? 0 : 1;
}

std::array<double, 3> physics_simd_test::accel(const std::array<double, 3> &x_i, uint16_t skip_index)
{
	std::array<double, 3> a = { 0.0, 0.0, 0.0 };
	for(uint16_t j = 0; j < obj_count; j++)
	{
		if(skip_index == j)
			continue;

		double r[3];
		r[0] = x[current][j][0] - x_i[0];
		r[1] = x[current][j][1] - x_i[1];
		r[2] = x[current][j][2] - x_i[2];

		double r_i[3];
		r_i[0] = r[0] / abs(r[0]);
		r_i[1] = r[1] / abs(r[1]);
		r_i[2] = r[2] / abs(r[2]);

		double r2 = (r[0] * r[0]) + (r[1] * r[1]) + (r[2] * r[2]);

		a[0] += G * m[j] * r_i[0] / r2;
		a[1] += G * m[j] * r_i[1] / r2;
		a[2] += G * m[j] * r_i[2] / r2;
	}
	return a;
}

void physics_simd_test::init(uint16_t obj_count)
{
	this->obj_count = obj_count;

	current = 0;
	next = 1;
	total_time = 0.0;

	x[0].resize(obj_count);
	x[1].resize(obj_count);
	v[0].resize(obj_count);
	v[1].resize(obj_count);
	a[0].resize(obj_count);
	a[1].resize(obj_count);
	r.resize(obj_count);
	m.resize(obj_count);

	// for now hard code some values
	mass_range[0] = 5e7;
	mass_range[1] = 1e8;
	radius_range[0] = 0.05;
	radius_range[1] = 0.25;
	//distance_range[0] = -radius_range[0] * 15.0;
	//distance_range[1] = radius_range[0] * 15.0;
	distance_range[0] = -4.0;
	distance_range[1] = 4.0;

	// random init stuff
	std::uniform_real_distribution<double> dist_m(mass_range[0],
		mass_range[1]);
	std::uniform_real_distribution<double> dist_r(radius_range[0],
		radius_range[1]);
	std::uniform_real_distribution<double> dist_d(distance_range[0],
		distance_range[1]);

	for(uint16_t i = 0; i < obj_count; i++)
	{
		r[i] = dist_r(generator);
		m[i] = dist_m(generator);

		// TODO: detect and avoid collisions here
		bool collision = true;
		while(collision)
		{
			// generate a new point
			x[0][i] = { dist_d(generator),
						dist_d(generator),
						dist_d(generator) };

			// look for a collision
			collision = false;
			for(uint16_t j = 0; j < i; j++)
			{
				if(i == j)
					continue;

				// direction unit vector
				std::array<double, 3> ji_uv;
				ji_uv[0] = (x[0][i][0] - x[0][j][0]) / abs(x[0][i][0] - x[0][j][0]);
				ji_uv[1] = (x[0][i][1] - x[0][j][1]) / abs(x[0][i][1] - x[0][j][1]);
				ji_uv[2] = (x[0][i][2] - x[0][j][2]) / abs(x[0][i][2] - x[0][j][2]);

				// position plus radius in the direction of the other object
				std::array<double, 3> ji_r;
				ji_r[0] = x[0][j][0] + ji_uv[0] * r[j];
				ji_r[1] = x[0][j][1] + ji_uv[1] * r[j];
				ji_r[2] = x[0][j][2] + ji_uv[2] * r[j];

				double distance_j = sqrt((x[0][i][0] - ji_r[0]) * (x[0][i][0] - ji_r[0])
					+ (x[0][i][1] - ji_r[1]) * (x[0][i][1] - ji_r[1])
					+ (x[0][i][2] - ji_r[2]) * (x[0][i][2] - ji_r[2]));
				// if radius > distance to j object then collision
				if(r[i] > distance_j)
				{
					collision = true;
					break;
				}
			}
		}

		v[0][i] = { 0.0, 0.0, 0.0 };
		a[0][i] = { 0.0, 0.0, 0.0 };
	}
}

void physics_simd_test::deinit()
{
	// TODO: should we really use swap to force a deallocation and is this the
	// right way to do it?

	std::vector<std::array<double, 3>> n0, n1;
	x[0].clear();
	x[1].clear();
	x[0].swap(n0);
	x[1].swap(n1);

	std::vector<std::array<double, 3>> n2, n3;
	v[0].clear();
	v[1].clear();
	v[0].swap(n2);
	v[1].swap(n3);

	std::vector<std::array<double, 3>> n4, n5;
	a[0].clear();
	a[1].clear();
	a[0].swap(n4);
	a[1].swap(n5);

	std::vector<double> n6;
	r.clear();
	r.swap(n6);

	std::vector<double> n7;
	m.clear();
	m.swap(n7);
}


