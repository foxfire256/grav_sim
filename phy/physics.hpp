#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define _USE_MATH_DEFINES
#include <vector>
#include <random>
#include <cstdint>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace fox
{
	class counter;
}

class physics
{
public:
	physics();
	virtual ~physics();

	void init(uint16_t obj_count);
	void deinit();
	void step(double delta_t);
	uint16_t get_obj_count(){return obj_count;}
	std::vector<Eigen::Vector3d> get_pos(){return x[current];}

private:
	/**
	 * @brief Current and next indicies
	 */
	uint16_t current, next;
	uint16_t obj_count;
	double total_time;
	double mass_range[2];
	double radius_range[2];
	double distance_range[2];
	/**
	 * @brief Position (x1, x2, x3), two vectors for new and old
	 */
	std::vector<Eigen::Vector3d> x[2];
	/**
	 * @brief Velocity, two vectors for new and old
	 */
	std::vector<Eigen::Vector3d> v[2];
	/**
	 * @brief Acceleration, two vectors for new and old
	 */
	std::vector<Eigen::Vector3d> a[2];
	/**
	 * @brief Ocject radii
	 */
	std::vector<double> r;
	/**
	 * @brief Object mass
	 */
	std::vector<double> m;
	/**
	 * @brief A random generator that is initialized in the constructor
	 */
	std::mt19937_64 generator;

	fox::counter *perf_counter;
	double step_time[8];
	double G = 6.67408e-11;
};

#endif
