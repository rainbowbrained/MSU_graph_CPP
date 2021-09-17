#include "densematrix.hpp"
#include <iostream>
#include <vector>

// CLASS DENSE MATRIX METHODS

DenseMatrix::DenseMatrix(size_t rows, size_t columns)
	: num_column(columns), num_row(rows)
{
	data.resize(num_row);
	for (size_t i = 0; i < num_row; i++)
	{
		data[i].resize(num_column, 0);
	}
};

DenseMatrix::DenseMatrix(const Matrix &other)
{
	this->num_column = other.num_columns();
	this->num_row = other.num_rows();
	data.resize(this->num_row);
	for (size_t i = 0; i < this->num_row; i++)
	{
		this->data[i].resize(this->num_column);
		for (size_t j = 0; j < this->num_column; j++)
			this->data[i][j] = other.get(i, j);
	}
};

DenseMatrix::DenseMatrix(size_t rows, size_t columns, double **val)
	: num_column(columns), num_row(rows)
{
	data.resize(this->num_rows());
	for (size_t i = 0; i < this->num_rows(); i++)
	{
		data[i].resize(this->num_columns(), 0);
		for (size_t j = 0; j < num_columns(); j++)
			data[i][j] = val[i][j];
	}
};

DenseMatrix::~DenseMatrix(){};

double &DenseMatrix::at(size_t x, size_t y)
{
	if ((x >= num_row) || (y >= num_column))
	{
		throw "DenseMatrix::at: index is out of range";
	}
	double &tmp = data[x][y];
	return tmp;
};

DenseMatrix &DenseMatrix::operator=(const Matrix &other)
{
	/*
	for (size_t i = 0; i < this->num_row; i++) {
		this->data[i].clear;
	this->data.clear;*/

	this->num_column = other.num_columns();
	this->num_row = other.num_rows();
	this->data.resize(this->num_row);
	for (size_t i = 0; i < this->num_row; i++)
	{
		this->data[i].resize(this->num_column);
		for (size_t j = 0; j < this->num_column; j++)
			this->data[i][j] = other.get(i, j);
	}
	return *this;
}

bool DenseMatrix::operator==(Matrix const &other) const
{ // OK
	if ((this->num_column != other.num_columns()) || (this->num_row != other.num_rows()))
	{
		throw "DenseMatrix::operator==(): Matrices have different sizes";
		return false;
	};
	for (size_t i = 0; i < num_row; i++)
	{
		for (size_t j = 0; j < num_column; j++)
		{
			if ((this->data[i][j] - other.get(i, j) < -eps) || (this->data[i][j] - other.get(i, j) > eps))
				return false;
		}
	}
	return true;
};

bool DenseMatrix::operator!=(Matrix const &other) const
{ // OK
	return (!(*this == other));
};

void DenseMatrix::set(size_t r, size_t c, double val)
{ //OK
	if ((r > num_row) || (c > num_column))
	{
		throw "DenseMatrix::set(): Index is out of range";
		return;
	}
	data[r][c] = val;
	return;
};

double DenseMatrix::get(size_t r, size_t c) const
{ //OK
	if ((r > num_row) || (c > num_column))
	{
		throw "SparseMatrix::get(): Index is out of range";
		return 0;
	}
	return data[r][c];
};

size_t DenseMatrix::num_rows() const { return num_row; };

size_t DenseMatrix::num_columns() const { return num_column; };

// Carry ou in the base class
DenseMatrix operator+(DenseMatrix one, const DenseMatrix &other)
{
	if ((one.num_columns() != other.num_columns()) || (one.num_rows() != other.num_rows()))
	{
		throw "DenseMatrix::operator+(): Matrices have different sizes";
		return one;
	};
	DenseMatrix sum(one.num_rows(), one.num_columns());
	for (size_t i = 0; i < one.num_rows(); i++)
	{
		for (size_t j = 0; j < one.num_columns(); j++)
		{
			double first = one.get(i, j);
			double second = other.get(i, j);
			if ((first + second < -eps) || (first + second > eps))
			{
				sum.set(i, j, first + second);
			}
		}
	}
	return sum;
};

// Carry ou in the base class
DenseMatrix operator*(DenseMatrix one, const DenseMatrix &other)
{ //OK
	if (one.num_rows() != other.num_columns())
	{
		throw "DenseMatrix::operato*(): Matrices have different sizes";
		return one;
	};
	DenseMatrix product(one.num_rows(), other.num_columns());
	for (size_t i = 0; i < one.num_rows(); i++)
	{
		for (size_t j = 0; j < other.num_columns(); j++)
		{
			double tmp = 0;
			for (size_t k = 0; k < one.num_columns(); k++)
			{
				tmp += one.get(i, k) * other.get(k, j);
			}
			if ((tmp < -eps) || (tmp > eps))
			{
				product.set(i, j, tmp);
			}
		}
	}
	return product;
};

std::ostream &operator<<(std::ostream &out, const DenseMatrix &matrix)
{
	out << "  ";
	for (size_t i = 0; i < matrix.num_columns(); i++)
	{
		out << i << " ";
	}
	out << std::endl;
	for (size_t i = 0; i < matrix.num_rows(); i++)
	{
		out << i << " ";
		for (size_t j = 0; j < matrix.num_columns(); j++)
			out << matrix.get(i, j) << ' ';
		out << std::endl;
	}
	return out;
};
