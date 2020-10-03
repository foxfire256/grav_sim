#ifndef PHYSICS_SIMD_TEST_HPP
#define PHYSICS_SIMD_TEST_HPP

#define _USE_MATH_DEFINES
#include <vector>
#include <array>
#include <random>
#include <cstdint>

class physics_simd_test
{
public:
	physics_simd_test();
	virtual ~physics_simd_test();

	void init(uint16_t obj_count);
	void deinit();
	void step(double delta_t);
	uint16_t get_obj_count(){return obj_count;}
	std::vector<std::array<double, 3>>& get_pos(){return x[current];}
	std::vector<double> get_radii(){return r;}

private:
	/**
	 * @brief Finds acceleration due to gravity
	 * @param x_i Position
	 * @param skip_index The index of the object that acceleration is calc
	 * @return The acceleration vector
	 */
	std::array<double, 3> accel(const std::array<double, 3>& x_i, uint16_t skip_index);

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
	std::array<std::vector<std::array<double, 3>>, 2> x;
	/**
	 * @brief Velocity, two vectors for new and old
	 */
	std::array<std::vector<std::array<double, 3>>, 2> v;
	/**
	 * @brief Acceleration, two vectors for new and old
	 */
	std::array<std::vector<std::array<double, 3>>, 2> a;
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

	double G = 6.67408e-11;
};

#endif
