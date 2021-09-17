#ifndef DENSEMATRIX_H_
#define DENSEMATRIX_H_

#include <iostream>
#include <vector>
#include "matrix.hpp"

class DenseMatrix : public Matrix
{
	size_t num_column;
	size_t num_row;
	std::vector<std::vector<double>> data;

public:
	// default constructor is useless in this task
	DenseMatrix() = delete;

	// constructor
	DenseMatrix(size_t, size_t);

	// copy constructor
	DenseMatrix(Matrix const &);

	// constructor: from regular matrix to dense
	DenseMatrix(size_t, size_t, double **);

	// destructor
	~DenseMatrix();

	// analog of the at function for vector
	double &at(size_t, size_t) override;

	// assignment operator
	DenseMatrix &operator=(const Matrix &);

	// checking matrices for equality
	bool operator==(Matrix const &other) const;
	bool operator!=(Matrix const &other) const;

	// getters and setters
	void set(size_t, size_t, double) override;
	double get(size_t, size_t) const override;

	// returnin matrix's parameters
	size_t num_rows() const override;
	size_t num_columns() const override;

	// binary operations (for dense matrix only)
	friend DenseMatrix operator+(DenseMatrix one, const DenseMatrix &other);
	friend DenseMatrix operator*(DenseMatrix one, const DenseMatrix &other);

	//overloading the output operation
	friend std::ostream &operator<<(std::ostream &, const DenseMatrix &);
};

#endif