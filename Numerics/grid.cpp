#include <cmath>
#include <vector>

#include <Eigen/Dense>

#include "grid.h"


std::vector<double> grid::uniform(
	const double x_min, 
	const double x_max, 
	const int n_points) {

	const double dx = (x_max - x_min) / (n_points - 1);

	std::vector<double> grid(n_points, 0.0);

	for (int i = 0; i != n_points; ++i) {
		grid[i] = dx * i + x_min;
	}

	return grid;

}


// Reference:
// - White (2013)
std::vector<double> grid::exponential_full(
	const double x_min,
	const double x_max,
	const int n_points,
	const double scaling) {

	// Scaling >> 0 -> grid is shifted towards x_min
	// Scaling << 0 -> grid is shifted towards x_max

	const double eta = (x_max - x_min) / (std::exp(scaling) - 1.0);

	std::vector<double> grid = grid::uniform(x_min, x_max, n_points);

	double z;

	for (int i = 0; i != n_points; ++i) {

		z = (double)i / (double)(n_points - 1);

		grid[i] = (x_min - eta) + eta * exp(scaling * z);

	}

	return grid;

}


std::vector<double> grid::exponential(
	const double x_min,
	const double x_max,
	const int n_points) {

	return grid::exponential_full(x_min, x_max, n_points);

}


// Reference:
// - White (2013)
std::vector<double> grid::hyperbolic_full(
	const double x_min,
	const double x_max,
	const int n_points,
	const double x_center,
	const double scaling) {

	const double beta = scaling * (x_max - x_min);

	const double delta = asinh((x_min - x_center) / beta);

	const double gamma = asinh((x_max - x_center) / beta) - delta;
	
	std::vector<double> grid = grid::uniform(x_min, x_max, n_points);

	double z;

	for (int i = 0; i != n_points; ++i) {

		z = (double)i / (double)(n_points - 1);

		grid[i] = x_center + beta * sinh(gamma * z + delta);

	}

	return grid;

}


std::vector<double> grid::hyperbolic(
	const double x_min,
	const double x_max,
	const int n_points) {

	return grid::hyperbolic_full(x_min, x_max, n_points);

}


Eigen::VectorXd grid::eigen::uniform(
	const double x_min, 
	const double x_max, 
	const int n_points) {

	const double dx = (x_max - x_min) / (n_points - 1);

	Eigen::VectorXd grid(n_points);

	for (int i = 0; i != n_points; ++i) {
		grid(i) = dx * i + x_min;
	}

	return grid;

}
