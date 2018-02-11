#include "physics.hpp"

#include "fox/counter.hpp"

physics::physics()
{
	this->generator = std::mt19937_64(std::random_device{}());

}

physics::~physics()
{

}

void physics::step(double delta_t)
{
	total_time += delta_t;

	// TODO: make this RK4
	// TODO: collision detection
	#pragma omp parallel for
	for(uint16_t i = 0; i < obj_count; i++)
	{
		// find the force due to gravity first
		Eigen::Vector3d f(0.0, 0.0, 0.0);
		for(uint16_t j = 0; j < obj_count; j++)
		{
			if(i == j)
				continue;

			Eigen::Vector3d r = x[current][j] - x[current][i];
			f += G * m[i] * m[j] * r.normalized() / r.squaredNorm();
		}

		//a[next][i] = f / m[i] + a[current][i];
		a[next][i] = f / m[i];

		// TODO: RK4 here
		v[next][i] = a[next][i] * delta_t + v[current][i];
		x[next][i] = v[next][i] * delta_t + x[current][i];
	}

	current = current ? 0 : 1;
	next = next ? 0 : 1;
}

void physics::init(uint16_t obj_count)
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
		x[0][i] = Eigen::Vector3d(dist_d(generator),
								dist_d(generator),
								dist_d(generator));

		v[0][i] = Eigen::Vector3d(0.0, 0.0, 0.0);
		a[0][i] = Eigen::Vector3d(0.0, 0.0, 0.0);
	}
}

void physics::deinit()
{
	// TODO: should we really use swap to force a deallocation and is this the
	// right way to do it?

	std::vector<Eigen::Vector3d> n0, n1;
	x[0].clear();
	x[1].clear();
	x[0].swap(n0);
	x[1].swap(n1);

	std::vector<Eigen::Vector3d> n2, n3;
	v[0].clear();
	v[1].clear();
	v[0].swap(n2);
	v[1].swap(n3);

	std::vector<Eigen::Vector3d> n4, n5;
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


