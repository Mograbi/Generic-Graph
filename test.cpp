/*
 * test.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: Moawiya Mograbi
 */

#include <iostream>
#include <fstream>
#include "graph.h"

using std::string;
using std::list;
using namespace mgg;

struct tempEdge {
	int v1;
	int v2;
	tempEdge(int v1, int v2) :
			v1(v1), v2(v2) {
	}
};
std::ostream& operator<<(std::ostream& os, const tempEdge& e) {
	os << e.v1 << "  <--->  " << e.v2;
	return os;
}

bool graphTest() {
	graph::Graph<int> graph;
	int d1 = 1, d2 = 2, d3 = 3;
	graph.addVertex(d1);
	graph.addVertex(d2);
	graph.addVertex(d3);

	graph.addEdge(d1, d2);
	graph.addEdge(d1, d3);

	list<int>* vertices;
	graph.getVertices(&vertices);
	typename list<int>::iterator iter = vertices->begin();
	for (; iter != vertices->end(); ++iter) {
		std::cout << (*iter) << " ";
	}
	delete vertices;
	std::cout << std::endl << "+++++++++++++++++ v1 +++++++++++++" << std::endl;
	list<tempEdge>* edges;
	graph.getEdgesOfVertex(&edges, d1);
	typename list<tempEdge>::iterator edgeIter = edges->begin();
	for (; edgeIter != edges->end(); ++edgeIter) {
		std::cout << (*edgeIter) << std::endl;
	}
	delete edges;
	std::cout << "+++++++++++++++++ v3 +++++++++++++" << std::endl;
	graph.getEdgesOfVertex(&edges, d3);
	edgeIter = edges->begin();
	for (; edgeIter != edges->end(); ++edgeIter) {
		std::cout << (*edgeIter) << std::endl;
	}
	delete edges;
	std::cout << "+++++++ reached from v1 +++++++" << std::endl;
	graph.returnReached(d1, &vertices);
	iter = vertices->begin();
	for (; iter != vertices->end(); ++iter) {
		std::cout << (*iter) << " ";
	}
	std::cout << std::endl;
	delete vertices;
	return true;
}

int main() {

	if (graphTest() == false) {
		std::cout << "graph test failed" << std::endl;
	} else {
		std::cout << "graph test succeded" << std::endl;
	}

	return 0;
}

