#include "sparsematrix.hpp"
#include "densematrix.hpp"
#include <iostream>

class Graph
{
	Matrix *AdjMat;
	bool oriented; //filled by the method

	//Utility functions
	bool isFilled(std::vector<bool> vec) const;
	size_t min_unvis(std::vector<double> vec, std::vector<bool> vis) const;
	double max_w(std::vector<double> vec, size_t cur) const;

public:
	// default constructor is useless in this task
	Graph() = delete;

	// constructor using matrix
	Graph(Matrix *);

	// copy constructor
	Graph(const Graph &);

	// assignment operator
	Graph &operator=(const Graph &);

	// changes the AdjMat and makes it not oriented
	void make_not_orient();

	// return s the number of bonded components in the graph
	size_t Bonded_components() const;

	// checks if the graph is oriented
	bool isOriented() const;

	// return the diameter of the graph
	double Diameter() const;

	// prints the AdjMat
	void Print() const;

	// adds a rib to a graph
	void AddRib(size_t, size_t, double);
};