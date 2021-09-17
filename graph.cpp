#include "graph.hpp"
#include <iostream>

#include <unistd.h>

Graph::Graph(Matrix *mat)
{
	if (mat->num_columns() != mat->num_rows())
	{
		throw "Graph: matrix must be square";
	}
	AdjMat = mat;
	oriented = isOriented();
};

Graph::Graph(const Graph &other)
{
	this->oriented = other.oriented;
	this->AdjMat = other.AdjMat;
	this->oriented = other.oriented;
};

Graph &Graph::operator=(const Graph &other)
{
	this->oriented = other.oriented;
	this->AdjMat = other.AdjMat;
	this->oriented = other.oriented;
	return *this;
};

size_t Graph::Bonded_components() const
{
	if (AdjMat->num_rows() != AdjMat->num_columns())
	{
		throw "Bonded_components:: Matrix must be square";
	}
	std::vector<size_t> keys(AdjMat->num_rows(), 0);
	size_t key = 0;
	for (size_t i = 0; i < AdjMat->num_columns(); i++)
	{
		key++;
		if (keys[i] == 0)
			keys[i] = key;
		Matrix::RowIterator ending = AdjMat->end_rows(i);
		for (Matrix::RowIterator it = AdjMat->iterate_rows(i); it != ending; ++it)
		{
			if (keys[it.column()] == 0)
			{
				// bonded nodes, second node is not connected to other
				keys[it.column()] = keys[i];
			}
			else
			{
				// bonded nodes, second node is connected to other nodes
				size_t tmp_key = keys[it.column()];
				for (size_t k = 0; k < AdjMat->num_columns(); k++)
				{
					if (keys[k] == tmp_key)
						keys[k] = keys[i];
				}
			}
		}
	}
	size_t comp = 0;
	for (size_t i = key; i > 0; i--)
	{
		size_t j = 0;
		while ((j < AdjMat->num_columns()) && (keys[j] != i))
			j++;
		if ((j < AdjMat->num_columns()) && (keys[j] == i))
			comp++;
	}
	return comp;
};

bool Graph::isOriented() const
{
	if (AdjMat->num_rows() != AdjMat->num_columns())
	{
		return true;
	}
	for (size_t i = 0; i < AdjMat->num_columns(); i++)
	{
		Matrix::RowIterator ending = AdjMat->end_rows(i);
		Matrix::ColumnIterator ending_col = AdjMat->end_columns(i);
		Matrix::ColumnIterator it_col = AdjMat->iterate_columns(i);
		for (Matrix::RowIterator it = AdjMat->iterate_rows(i); it != ending; ++it)
		{
			if ((it_col == ending_col) || (it.column() != it_col.row()) || (*it - *it_col > eps) || (*it - *it_col < -eps))
				return true;
			++it_col;
		}
	}
	return false;
};

void Graph::make_not_orient()
{
	for (size_t i = 0; i < AdjMat->num_rows(); i++)
		for (size_t j = 0; j < AdjMat->num_rows(); j++)
		{
			if ((AdjMat->get(i, j) < eps) && (AdjMat->get(i, j) > -eps) && ((AdjMat->get(j, i) > eps) || (AdjMat->get(j, i) < -eps)))
			{
				// AdjMat[i][j] == 0, AdjMat[j][i] != 0
				AdjMat->set(i, j, AdjMat->get(j, i));
			}
			else if ((AdjMat->get(j, i) < eps) && (AdjMat->get(j, i) > -eps) && ((AdjMat->get(i, j) > eps) || (AdjMat->get(i, j) < -eps)))
			{
				// AdjMat[i][j] != 0, AdjMat[j][i] == 0
				AdjMat->set(j, i, AdjMat->get(i, j));
			}
			else if (((AdjMat->get(j, i) < -eps) || (AdjMat->get(j, i) > eps)) && ((AdjMat->get(i, j) > eps) || (AdjMat->get(i, j) < -eps)))
			{
				// AdjMat[i][j] != 0, AdjMat[j][i] != 0
				AdjMat->set(j, i, AdjMat->get(i, j));
			}
		}
	oriented = 0;
	return;
};

double Graph::Diameter() const
{
	if (this->oriented || (this->Bonded_components() > 1))
	{
		throw "Graph:Diameter: the graph must be oriented and bonded";
	}
	double max_way = 0;

	for (size_t i = 0; i < AdjMat->num_columns(); i++)
	{
		std::vector<double> keys(AdjMat->num_rows(), SIZE_MAX);
		std::vector<bool> visited(AdjMat->num_rows(), 0);
		keys[i] = 0;
		size_t cur_node = i;
		while (!isFilled(visited))
		{
			visited[cur_node] = 1;
			for (Matrix::RowIterator it = AdjMat->iterate_rows(cur_node); it != AdjMat->end_rows(cur_node); ++it)
			{
				if (*it < 0)
				{
					throw "Graph:Diameter(): the algorithm does not work negative way length";
				}
				if ((it.column() != i) && (keys[cur_node] + *it < keys[it.column()]))
				{
					keys[it.column()] = keys[cur_node] + *it;
				}
			}
			cur_node = min_unvis(keys, visited);
		}
		double tmp = max_w(keys, cur_node);
		if (tmp > max_way)
		{
			max_way = tmp;
		}
	}
	return max_way;
};

bool Graph::isFilled(std::vector<bool> vec) const
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == 0)
			return false;
	}
	return true;
};

size_t Graph::min_unvis(std::vector<double> vec, std::vector<bool> vis) const
{
	double m = __DBL_MAX__;
	size_t tmp = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if ((vec[i] < m) && (!vis[i]))
		{
			m = vec[i];
			tmp = i;
		}
	}
	return tmp;
};

double Graph::max_w(std::vector<double> vec, size_t cur) const
{
	double m = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if ((vec[i] > m) && (i != cur))
			m = vec[i];
	}
	return m;
};

void Graph::Print() const
{
	std::cout << "  ";
	for (size_t i = 0; i < AdjMat->num_columns(); i++)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < AdjMat->num_rows(); i++)
	{
		std::cout << i << " ";
		for (size_t j = 0; j < AdjMat->num_columns(); j++)
			std::cout << AdjMat->get(i, j) << ' ';
		std::cout << std::endl;
	}
	return;
};

void Graph::AddRib(size_t x, size_t y, double val)
{
	AdjMat->set(x, y, val);
	return;
};