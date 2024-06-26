#pragma once

#include <vector>

#include "band_diagonal_matrix.h"
#include "utility.h"


// Finite difference representation of first order derivative operator.
namespace d1dx1 {

	// Finite difference representation on uniform grid.
	namespace uniform {

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 1st order accurary.
		TriDiagonal c2b1(const std::vector<double>& grid);

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		TriDiagonal c2b2(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		// Boundary 2nd row: Central difference, 2nd order accurary.
		PentaDiagonal c4b2(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Forward difference, 4th order accurary.
		// Boundary 2nd row: Forward difference, 4th order accurary.
		PentaDiagonal c4b4(const std::vector<double>& grid);

	}

	// Finite difference representation on non-uniform grid.
	namespace nonuniform {

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 1st order accurary.
		TriDiagonal c2b1(const std::vector<double>& grid);

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		TriDiagonal c2b2(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		// Boundary 2nd row: Central difference, 2nd order accurary.
		PentaDiagonal c4b2(const std::vector<double>& grid);

	}

}


// Finite difference representation of second order derivative operator.
namespace d2dx2 {

	// Finite difference representation on uniform grid.
	namespace uniform {

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Neumann boundary condition, d2dx2 = 0. TODO: Is this Neumann, or only the case for B with fist order derivatives.
		TriDiagonal c2b0(const std::vector<double>& grid);

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 1st order accurary.
		TriDiagonal c2b1(const std::vector<double>& grid);

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		TriDiagonal c2b2(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Neumann boundary condition, d2dx2 = 0.
		// Boundary 2nd row: Central difference, 2nd order accurary.
		PentaDiagonal c4b0(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Forward difference, 2nd order accurary.
		// Boundary 2nd row: Central difference, 2nd order accurary.
		PentaDiagonal c4b2(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Forward difference, 4th order accurary.
		// Boundary 2nd row: Forward difference, 4th order accurary.
		PentaDiagonal c4b4(const std::vector<double>& grid);

	}

	// Finite difference representation on non-uniform grid.
	namespace nonuniform {

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Neumann boundary condition, d2dx2 = 0.
		TriDiagonal c2b0(const std::vector<double>& grid);

		// Interior: Central difference, 2nd order accuracy.
		// Boundary 1st row: Forward difference, 1st order accurary.
		TriDiagonal c2b1(const std::vector<double>& grid);

		// Interior: Central difference, 4th order accuracy.
		// Boundary 1st row: Neumann boundary condition, d2dx2 = 0.
		// Boundary 2nd row: Central difference, 2nd order accurary.
		PentaDiagonal c4b0(const std::vector<double>& grid);

	}

}


// TODO: Class method for evaluating second order mixed derivative operator.
template <class T1, class T2>
class MixedDerivative {

private:

	T1 d1dx1;
	T2 d1dy1;
	std::vector<double> prefactors;

public:

	MixedDerivative(
		T1& d1dx1_, 
		T2& d1dy1_) {

		d1dx1 = d1dx1_;
		d1dy1 = d1dy1_;

		std::vector<double> tmp(d1dx1.order() * d1dy1.order(), 1.0);
		prefactors = tmp;

	}

	void set_prefactors(const double scalar) {
		for (int i = 0; i != prefactors.size(); ++i) {
			prefactors[i] = scalar;
		}
	}

	void set_prefactors(
		const std::vector<double>& coef_x,
		const std::vector<double>& coef_y) {
		int index = 0;
		for (int i = 0; i != coef_x.size(); ++i) {
			for (int j = 0; j != coef_y.size(); ++j) {
				prefactors[i] = coef_x[i] * coef_y[j];
				++index;
			}
		}
	}

	void set_prefactors(
		const std::vector<double>& factors) {
		int index = 0;
		for (int i = 0; i != factors.size(); ++i) {
			prefactors[i] = factors[i];
				++index;
		}
	}

	std::vector<double> d2dxdy(
		std::vector<double> func) {

		const int n_x = d1dx1.order();
		const int n_y = d1dy1.order();

		std::vector<double> vec_x(n_x, 0.0);
		std::vector<double> vec_y(n_y, 0.0);

		// Evaluate first order partial derivative wrt y.
		func = action_2d(n_y, n_x, 2, false, d1dy1, func);

		// Evaluate first order partial derivative wrt x.
		func = action_2d(n_x, n_y, 1, false, d1dx1, func);

		// Multiply prefactors.
		for (int i = 0; i != n_x * n_y; ++i) {
			func[i] *= prefactors[i];
		}

		return func;

	}

};
