#include "physics.hpp"

#include "fox/counter.hpp"

physics::physics()
{
	this->generator = std::mt19937_64(std::random_device{}());
	perf_counter = new fox::counter();
}

physics::~physics()
{
	delete perf_counter;
}

void physics::init(uint16_t obj_count)
{
	this->obj_count = obj_count;

	current = 0;
	next = 1;

	xm[0].resize(obj_count);
	xm[1].resize(obj_count);
	v[0].resize(obj_count);
	v[1].resize(obj_count);
	a[0].resize(obj_count);
	a[1].resize(obj_count);
	r.resize(obj_count);

	// for now hard code some values
	mass_range[0] = 1e6;
	mass_range[1] = 1e7;
	radius_range[0] = 1e4;
	radius_range[1] = 1.2e4;
	distance_range[0] = -radius_range[0] * 15.0;
	distance_range[0] = radius_range[0] * 15.0;

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

		// TODO: detect and avoid collisions here
		xm[0][i] = Eigen::Vector4d(dist_d(generator),
								dist_d(generator),
								dist_d(generator),
								dist_m(generator));

		v[0][i] = Eigen::Vector3d(0.0, 0.0, 0.0);
		a[0][i] = Eigen::Vector3d(0.0, 0.0, 0.0);
	}
}

void physics::deinit()
{
	std::vector<Eigen::Vector4d> n0, n1;
	xm[0].clear();
	xm[1].clear();
	xm[0].swap(n0);
	xm[1].swap(n1);

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
}

void physics::step()
{

}

