#include <iostream>
#include "matrix.hpp"

// ---------- ROW ITERATOR ----------

Matrix::RowIterator::RowIterator(size_t row, size_t col, Matrix &m) : mat(m)
{
    num_row = row;
    cur_column = col;
};

Matrix::RowIterator &Matrix::RowIterator::operator++()
{
    cur_column++;
    if (cur_column >= mat.num_columns())
    {
        cur_column = mat.num_columns();
        return *this;
    }
    while ((cur_column < mat.num_columns()) &&
           (mat.get(num_row, cur_column) < eps) && (mat.get(num_row, cur_column) > -eps))
        cur_column++;
    if (cur_column == mat.num_columns())
    {
        return *this;
    }
    return *this;
};

Matrix::RowIterator Matrix::RowIterator::operator++(int)
{
    RowIterator prev = *this;
    ++(*this);
    return prev;
};

bool Matrix::RowIterator::operator==(const Matrix::RowIterator &other) const
{
    return ((mat == other.mat) && (num_row == other.num_row) && (cur_column == other.cur_column));
};

bool Matrix::RowIterator::operator!=(const Matrix::RowIterator &other) const
{
    return !(*this == other);
};

const double &Matrix::RowIterator::operator*()
{
    return mat.at(num_row, cur_column);
};

double Matrix::RowIterator::operator*() const
{
    return mat.get(num_row, cur_column);
};

Matrix::RowIterator Matrix::iterate_rows(size_t row)
{
    size_t col = 0;
    while ((col < num_columns()) &&
           (get(row, col) < eps) && (get(row, col) > -eps))
    {
        col++;
    }
    return RowIterator(row, col, *this);
};

Matrix::RowIterator Matrix::end_rows(size_t row)
{
    return RowIterator(row, num_columns(), *this);
};

size_t Matrix::RowIterator::column() const
{
    return cur_column;
};

// ---------- COLUMN ITERATOR ----------

Matrix::ColumnIterator::ColumnIterator(size_t row, size_t col, Matrix &m) : mat(m)
{
    num_column = col;
    cur_row = row;
};

Matrix::ColumnIterator &Matrix::ColumnIterator::operator++()
{
    cur_row++;
    if (cur_row >= mat.num_rows())
    {
        cur_row = mat.num_rows();
        return *this;
    }
    while ((cur_row < mat.num_rows()) &&
           (mat.get(cur_row, num_column) < eps) && (mat.get(cur_row, num_column) > -eps))
        cur_row++;
    if (cur_row == mat.num_rows())
    {
        return *this;
    }
    return *this;
};

Matrix::ColumnIterator Matrix::ColumnIterator::operator++(int)
{
    ColumnIterator prev = *this;
    ++(*this);
    return prev;
};

bool Matrix::ColumnIterator::operator==(const Matrix::ColumnIterator &other) const
{
    return ((mat == other.mat) && (cur_row == other.cur_row) && (num_column == other.num_column));
};

bool Matrix::ColumnIterator::operator!=(const Matrix::ColumnIterator &other) const
{
    return !(*this == other);
};

const double &Matrix::ColumnIterator::operator*()
{
    return mat.at(cur_row, num_column);
};

double Matrix::ColumnIterator::operator*() const
{
    return mat.get(cur_row, num_column);
};

Matrix::ColumnIterator Matrix::iterate_columns(size_t col)
{
    size_t row = 0;
    while ((row < num_rows()) &&
           (get(row, col) < eps) && (get(row, col) > -eps))
        row++;
    return ColumnIterator(row, col, *this);
};

Matrix::ColumnIterator Matrix::end_columns(size_t col)
{
    return ColumnIterator(num_rows(), col, *this);
};

size_t Matrix::ColumnIterator::row() const
{
    return cur_row;
};