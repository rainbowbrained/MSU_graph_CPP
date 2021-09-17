#ifndef MATRIX_H_
#define MATRIX_H_
#include <cstdlib>
#define eps 10e-5

class Matrix
{
public:
    virtual ~Matrix() = default;
    virtual bool operator==(Matrix const &) const = 0;
    virtual bool operator!=(Matrix const &) const = 0;
    virtual void set(size_t r, size_t c, double val) = 0;
    virtual double get(size_t r, size_t c) const = 0;
    virtual size_t num_rows() const = 0;
    virtual size_t num_columns() const = 0;
    virtual double &at(size_t, size_t) = 0;

    // Iterators
    class RowIterator
    {
    private:
        Matrix &mat;
        size_t num_row, cur_column;

    public:
        RowIterator() = delete;
        RowIterator(size_t, size_t, Matrix &);
        RowIterator &operator++();   //prefix increment
        RowIterator operator++(int); //postfix increment
        bool operator==(const RowIterator &) const;
        bool operator!=(const RowIterator &) const;
        const double &operator*();
        double operator*() const;
        size_t column() const;
    };

    class ColumnIterator
    {
    private:
        Matrix &mat;
        size_t cur_row, num_column;

    public:
        ColumnIterator() = delete;
        ColumnIterator(size_t, size_t, Matrix &);
        ColumnIterator &operator++();   //prefix increment
        ColumnIterator operator++(int); //postfix increment
        bool operator==(const ColumnIterator &) const;
        bool operator!=(const ColumnIterator &) const;
        const double &operator*();
        double operator*() const;
        size_t row() const;
    };

    RowIterator iterate_rows(size_t);
    RowIterator end_rows(size_t);
    ColumnIterator iterate_columns(size_t);
    ColumnIterator end_columns(size_t);
};

#endif