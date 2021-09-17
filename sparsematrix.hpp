#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_
#include <iostream>
#include "matrix.hpp"

class SparseMatrix : public Matrix
{
	size_t num_column;
	size_t num_row;
	double *data;
	size_t *row;
	size_t *col;
	size_t non_zero; // size of array data and col

	class Row // proxy class for iterating
	{
		size_t num_row;
		SparseMatrix &matr;

	public:
		class subRow
		{
			size_t num_row;
			size_t num_col;
			SparseMatrix &matr;

		public:
			subRow(SparseMatrix &m, size_t r) 
				: num_row(r), num_col(0), matr(m) {};
			double &operator*()
			{
				double &tmp = *(matr.GetElem(num_row, num_col));
				return tmp;
			};
			subRow operator+(size_t c)
			{
				num_col += c;
				return *this;
			};
		};

		Row(SparseMatrix &m, size_t r) : num_row(r), matr(m){};
		//second index
		double &operator[](size_t column_number)
		{
			double &tmp = *(matr.GetElem(num_row, column_number));
			return tmp;
		};
		Row operator+(size_t num)
		{
			num_row += num;
			return *this;
		};

		//*(m + row) + column
		subRow operator*()
		{
			return subRow(matr, num_row);
		};
	};

	class constRow  // proxy class for iterating in const objects
	{
		size_t num_row;
		const SparseMatrix &matr;

	public:
		class constsubRow
		{
			size_t num_row;
			size_t num_col;
			const SparseMatrix &matr;

		public:
			constsubRow(const SparseMatrix &m, size_t r) 
				: num_row(r), num_col(0), matr(m) {};
			double operator*()
			{
				double tmp = matr.get(num_row, num_col);
				num_col = 0;
				return tmp;
			};
			constsubRow operator+(size_t c)
			{
				num_col += c;
				return *this;
			};
		};

		constRow(SparseMatrix const &m, size_t r) 
			: num_row(r), matr(m){};
		//second index
		double operator[](size_t column_number)
		{
			double tmp = matr.get(num_row, column_number);
			return tmp;
		};

		constsubRow operator*()
		{
			return constsubRow(matr, num_row);
		};

		constRow operator+(size_t num)
		{
			num_row += num;
			return *this;
		};
	};

public:

	// default constructor is useless in this task
	SparseMatrix() = delete; 

	// constructor
	SparseMatrix(size_t, size_t );

	// copy constructor
	SparseMatrix(SparseMatrix const &);

	// constructor: from regular matrix to sparse
	SparseMatrix(size_t, size_t, double **);

	// destructor
	~SparseMatrix();

	// analog of the at function for vector
	double & at(size_t, size_t) override;

	// assignment operator
	SparseMatrix &operator=(const SparseMatrix &other);

	// checking matrices for equality
	bool operator==(Matrix const &other) const;
	bool operator!=(Matrix const &other) const;

	// getters and setters
	void set(size_t r, size_t c, double val) override;
	double get(size_t r, size_t c) const override;

	// returning matrix's parameters
	size_t num_rows() const override;
	size_t num_columns() const override;
	
	// creates a place and returns the pointer on the element in data array
	double *GetElem(size_t r, size_t c);
	
	// binary operations (for sparse matrix only)
	friend SparseMatrix operator+(SparseMatrix one, const Matrix &other);
	friend SparseMatrix operator*(SparseMatrix one, const Matrix &other);

	// overloading the output operation
	friend std::ostream &operator<<(std::ostream &out, const SparseMatrix &matrix);
	
	// method that deletes zero elements from arrays
	void CleanZero();

	// address ariphmetic and overloading square brackets
	Row operator[](const size_t x);
	Row::subRow operator*();
	Row operator+(const size_t x);
	constRow operator[](const size_t x) const;
	constRow operator+(const size_t x) const;
	constRow::constsubRow operator*() const;
};

#endif