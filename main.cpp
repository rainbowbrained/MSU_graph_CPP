#include <iostream>
#include "matrix.hpp"
#include "densematrix.hpp"
#include "sparsematrix.hpp"
#include "graph.hpp"

int main()
{
	try
	{
		SparseMatrix m1(5, 5);
		m1.set(0, 1, 1);
		m1.set(1, 2, 2);
		m1.set(1, 4, 3);
		std::cout << "SparseMatrix m1" << std::endl
				  << m1 << std::endl;

		DenseMatrix m2 = m1 + m1;
		std::cout << "DenseMatrix m2 = m1 + m1" << std::endl
				  << m2 << std::endl;

		Matrix::RowIterator ending = m1.end_rows(1);
		std::cout << "testing row iterator" << std::endl;
		for (Matrix::RowIterator it = m1.iterate_rows(1); it != ending; ++it)
		{
			std::cout << "1 --- " << *it << " --- " << it.column() << std::endl;
		}
		Matrix::ColumnIterator ending_col = m1.end_columns(4);
		std::cout << "testing column iterator" << std::endl;
		for (Matrix::ColumnIterator it = m1.iterate_columns(4); it != ending_col; ++it)
		{
			std::cout << "1 --- " << *it << " --- " << it.row() << std::endl;
		}
		SparseMatrix *tmp = &m1;
		Graph mygraph(tmp);
		std::cout << std::endl
				  << "Number of bonded components in mygraph (m1): " << mygraph.Bonded_components() << std::endl;

		DenseMatrix m_nor(10, 10);
		m_nor.set(0, 1, 9);
		m_nor.set(1, 0, 9);
		m_nor.set(0, 9, 1);
		m_nor.set(9, 0, 1);
		for (size_t i = 1; i < 9; i++)
		{
			m_nor.set(i + 1, i, i);
			m_nor.set(i, i + 1, i);
		}
		std::cout << "DenseMatrix m1" << std::endl
				  << m_nor << std::endl;
		DenseMatrix *tmp_or = &m_nor;
		Graph norgraph(tmp_or);
		std::cout << "Is mygraph (m1) oriented? : " << mygraph.isOriented() << std::endl;
		std::cout << "Is norgraph (tmp_or) oriented? : " << norgraph.isOriented() << std::endl;
		std::cout << "Not oriented norgraph's diameter : " << norgraph.Diameter() << std::endl;
		std::cout << "Oriented mygraph's diameter : " << mygraph.Diameter() << std::endl;
	}
	catch (const char *exception) // обработчик исключений типа const char*
	{
		std::cerr << "Error: " << exception << '\n';
	}

	try
	{
		DenseMatrix m3(5, 5);
		m3.set(0, 1, 1);
		m3.set(1, 2, 2);
		m3.set(1, 4, 3);
		std::cout << "DenseMatrix m3" << std::endl
				  << m3 << std::endl;
		DenseMatrix *tmp = &m3;
		Graph mygraph(tmp);
		std::cout << "Making mygraph (m3) not oriented" << std::endl;
		mygraph.make_not_orient();
		mygraph.Print();
		std::cout << "Number of bonded components in mygraph (m3): " << mygraph.Bonded_components() << std::endl;
		std::cout << "Mygraph's diameter : " << mygraph.Diameter() << std::endl;
	}
	catch (const char *exception) // обработчик исключений типа const char*
	{
		std::cerr << "Error: " << exception << '\n';
	}
	try
	{
		DenseMatrix m3(5, 5);
		DenseMatrix *tmp = &m3;
		Graph mygraph(tmp);
		mygraph.AddRib(1, 3, 4);
		mygraph.AddRib(0, 1, 1);
		mygraph.AddRib(1, 2, 2);
		mygraph.AddRib(1, 4, 3);
		mygraph.make_not_orient();
		mygraph.Print();
		std::cout << "Number of bonded components in mygraph (m3): " << mygraph.Bonded_components() << std::endl;
		std::cout << "Mygraph's diameter : " << mygraph.Diameter() << std::endl
				  << std::endl;
		Graph const newgraph(mygraph);
		std::cout << "Const newgraph = mygraph: " << std::endl;
		newgraph.Print();
	}
	catch (const char *exception) // обработчик исключений типа const char*
	{
		std::cerr << "Error: " << exception << '\n';
	}
	return 0;
}
