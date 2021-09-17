#include "sparsematrix.hpp"
#include <iostream>

// CLASS SPARSE MATRIX METHODS

SparseMatrix::SparseMatrix(size_t rows, size_t columns)
	: num_column(columns), num_row(rows)
{
	non_zero = (rows * columns + 10) / 5;
	row = (size_t *)malloc(sizeof(size_t) * (rows + 1));
	col = (size_t *)malloc(sizeof(size_t) * non_zero);
	data = (double *)malloc(sizeof(double) * non_zero);
	for (size_t i = 0; i <= num_row; i++)
		row[i] = 0;
};

SparseMatrix::SparseMatrix(SparseMatrix const &other)
{
	this->num_column = other.num_column;
	this->num_row = other.num_row;
	this->non_zero = other.non_zero;
	this->row = (size_t *)malloc(sizeof(size_t) * (num_row + 1));
	this->col = (size_t *)malloc(sizeof(size_t) * non_zero);
	this->data = (double *)malloc(sizeof(double) * non_zero);
	for (size_t i = 0; i < other.row[num_row]; i++)
	{
		this->data[i] = other.data[i];
		this->col[i] = other.col[i];
	}
	for (size_t i = 0; i <= num_row; i++)
		this->row[i] = other.row[i];
};

// regular matrix to csr
SparseMatrix::SparseMatrix(size_t rows, size_t columns, double **val)
	: num_column(columns), num_row(rows)
{
	non_zero = (rows * columns + 10) / 5;
	row = (size_t *)malloc(sizeof(size_t) * (rows + 1));
	col = (size_t *)malloc(sizeof(size_t) * non_zero);
	data = (double *)malloc(sizeof(double) * non_zero);
	size_t k = 0;
	row[0] = 0;
	for (size_t i = 0; i < rows; i++)
	{
		row[i + 1] = row[i];
		for (size_t j = 0; j < columns; j++)
		{
			if (k == non_zero - 1)
			{
				non_zero = non_zero + 10;
				size_t *col1 = (size_t *)realloc(col, sizeof(size_t) * non_zero);
				double *data1 = (double *)realloc(data, sizeof(double) * non_zero);
				if ((col1 == NULL) || (data1 == NULL))
				{
					perror("Realloc: ");
					exit(1);
				}
				else
				{
					data = data1;
					col = col1;
				}
			}
			if ((val[i][j] > eps) || (val[i][j] < -eps))
			{
				data[k] = val[i][j];
				col[k] = j;
				k++;
				row[i + 1]++;
			}
		}
	}
};

SparseMatrix::~SparseMatrix()
{
	free(row);
	free(col);
	free(data);
};

double &SparseMatrix::at(size_t x, size_t y)
{
	if ((x >= num_row) || (y >= num_column))
	{
		throw "SparseMatrix::at: index is out of range";
	}
	double &tmp = *GetElem(x, y);
	return tmp;
};

SparseMatrix &SparseMatrix::operator=(const SparseMatrix &other)
{
	if ((this->num_column != other.num_column) || (this->num_row != other.num_row))
	{
		throw "SparseMatrix::operator=():Matrices have different sizes";
		return *this;
	};
	if (this->non_zero < other.non_zero)
	{
		// expanding the second array
		this->non_zero = other.non_zero;
		size_t *col1 = (size_t *)realloc(col, sizeof(size_t) * non_zero);
		double *data1 = (double *)realloc(data, sizeof(double) * non_zero);
		if ((col1 == NULL) || (data1 == NULL))
		{
			throw "SparseMatrix::double* GetElem(): Realloc";
			return *this;
		}
		else
		{
			data = data1;
			col = col1;
		}
	}
	for (size_t i = 0; i <= num_row; i++)
		this->row[i] = other.row[i];
	for (size_t i = 0; i < other.row[num_row]; i++)
	{
		this->data[i] = other.data[i];
		this->col[i] = other.col[i];
	};
	return *this;
}

bool SparseMatrix::operator==(Matrix const &other) const
{ // OK
	if ((this->num_column != other.num_columns()) || (this->num_row != other.num_rows()))
	{
		throw "SparseMatrix::operator==(): Matrices have different sizes";
		return false;
	};
	for (size_t i = 0; i < num_row; i++)
	{
		for (size_t j = 0; j < num_column; j++)
		{
			if (((this->get(i, j) - other.get(i, j)) > eps) || ((this->get(i, j) - other.get(i, j)) < -eps))
				return false;
		}
	}
	return true;
};

bool SparseMatrix::operator!=(Matrix const &other) const
{
	return (!(*this == other));
};

void SparseMatrix::set(size_t r, size_t c, double val)
{
	if ((r > num_row) || (c > num_column))
	{
		throw "SparseMatrix::set(): Index is out of range";
		return;
	}
	*(this->GetElem(r, c)) = val;
	return;
};

double SparseMatrix::get(size_t r, size_t c) const
{
	if ((r > num_row) || (c > num_column))
	{
		throw "SparseMatrix::get(): Index is out of range";
		return 0;
	}
	if (row[r] == row[r + 1])
		return 0;
	size_t i = row[r];
	while ((i < row[r + 1]) && (col[i] < c))
		i++;
	if ((col[i] == c) && (i < row[r + 1]))
		return data[i];
	else
		return 0;
};

size_t SparseMatrix::num_rows() const { return num_row; };

size_t SparseMatrix::num_columns() const { return num_column; };

double *SparseMatrix::GetElem(size_t r, size_t c)
{
	size_t i = row[r];
	while ((i < row[r + 1]) && (c > col[i]))
		i++;
	// i is an index for a new value
	// now we need to shift other elements to the right or return addres in a data array
	if ((i < row[r + 1]) && (c == col[i]))
	{
		return data + i;
	}
	else
	{
		if (non_zero - 1 == row[num_row])
		{
			// allocate extra memory
			non_zero = non_zero + 10;
			size_t *col1 = (size_t *)realloc(col, sizeof(size_t) * non_zero);
			double *data1 = (double *)realloc(data, sizeof(double) * non_zero);
			if ((col1 == NULL) || (data1 == NULL))
			{
				throw "SparseMatrix::double* GetElem(): Realloc";
				return NULL;
			}
			else
			{
				data = data1;
				col = col1;
			}
		}
		if ((i > 0) && (c < col[i - 1]) && (i <= row[r + 1]) && (i - 1 >= row[r]))
			i--;
		int total_nonzero = row[num_row];
		//shift elements to the right
		if (total_nonzero > 0)
		{
			for (size_t m = total_nonzero; m > i; m--)
			{
				data[m + 1] = data[m];
				col[m + 1] = col[m];
			}
		}
		col[i] = c;
		data[i] = 0;
		for (size_t m = r + 1; m <= num_row; m++)
			row[m]++;
		return data + i;
	}
};

// Matrix addition
SparseMatrix operator+(SparseMatrix one, const Matrix &other)
{
	if ((one.num_columns() != other.num_columns()) || (one.num_rows() != other.num_rows()))
	{
		throw "SparseMatrix::operator+(): Matrices have different sizes";
		return one;
	};
	SparseMatrix sum(one.num_rows(), one.num_columns());
	for (size_t i = 0; i < one.num_rows(); i++)
	{
		for (size_t j = 0; j < one.num_columns(); j++)
		{
			double first = one.get(i, j);
			double second = other.get(i, j);
			if ((first + second < -eps) || (first + second > eps))
			{
				*(sum.GetElem(i, j)) = first + second;
			}
		}
	}
	return sum;
};

SparseMatrix operator*(SparseMatrix one, const Matrix &other)
{
	if (one.num_rows() != other.num_columns())
	{
		throw "SparseMatrix::operator*(): Matrices have different sizes";
		return one;
	};
	SparseMatrix product(one.num_rows(), other.num_columns());
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
				*(product.GetElem(i, j)) = tmp;
			}
		}
	}
	return product;
};

void SparseMatrix::CleanZero()
{
	for (int i = row[num_row] - 1; i >= 0; i--)
	{
		if ((data[i] < eps) && (data[i] > -eps))
		{
			size_t cur_row = 1;
			while ((row[cur_row] <= (size_t)i) && (cur_row < num_row))
				cur_row++;
			for (size_t m = num_row; m >= cur_row; m--)
				row[m]--;
			for (size_t m = i; m < row[num_row]; m++)
			{
				data[m] = data[m + 1];
				col[m] = col[m + 1];
			}
		}
	}
	return;
};

SparseMatrix::Row SparseMatrix::operator[](const size_t x)
{
	SparseMatrix &tmp = *this;
	return Row(tmp, x);
};

// Address addition
SparseMatrix::Row SparseMatrix::operator+(size_t const x)
{
	SparseMatrix &tmp = *this;
	return Row(tmp, x);
};
SparseMatrix::Row::subRow SparseMatrix::operator*()
{
	SparseMatrix &tmp = *this;
	return Row::subRow(tmp, 0);
};

// Address addition for const objects
SparseMatrix::constRow SparseMatrix::operator[](size_t const x) const
{
	const SparseMatrix &tmp = *this;
	return constRow(tmp, x);
};

SparseMatrix::constRow SparseMatrix::operator+(size_t const x) const
{
	const SparseMatrix &tmp = *this;
	return constRow(tmp, x);
};

SparseMatrix::constRow::constsubRow SparseMatrix::operator*() const
{
	const SparseMatrix &tmp = *this;
	return constRow::constsubRow(tmp, 0);
};

std::ostream &operator<<(std::ostream &out, const SparseMatrix &matrix)
{
	out << "  ";
	for (size_t j = 0; j < matrix.num_column; j++)
	{
		out << j << ' ';
	}
	out << std::endl;
	for (size_t i = 0; i < matrix.num_row; i++)
	{
		out << i << ' ';
		for (size_t j = 0; j < matrix.num_row; j++)
		{
			out << matrix.get(i, j) << " ";
		}
		out << std::endl;
	}
	return out;
};