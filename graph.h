/*
 * graph.h
 *
 *  Created on: Dec 20, 2015
 *      Author: Moawiya Mograbi
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include <list>
#include "system.h"
#include "block.h"
#include "exceptions.h"
#include <map>

namespace floorplan {

template<class Data>
class CheckClass {
public:
	/*
	 * auxiliary contains function to check if element is in the list
	 * because <list> does not provide contains function
	 * please not that the class Data must implement operator==
	 */
	bool listcontains(list<Data>* l, const Data& element) {
		typename list<Data>::iterator iter = l->begin();
		for (; iter != l->end(); ++iter) {
			if ((*iter) == element) {
				return true;
			}
		}
		return false;
	}
};

/*==================================================================================*/
/*==========================       Vertex       ====================================*/
/*==================================================================================*/
template<class Data>
struct Vertex {
	Data data;
	bool operator==(const Vertex& v) {
		return data == v.data;
	}
	Vertex(const Data& d) :
			data(d) {
	}
	Vertex(const Vertex& v) :
			data(v.data) {
	}
};
template<class Data>
std::ostream& operator<<(std::ostream& os, const Vertex<Data>& v) {
	os << v.data;
	return os;
}
/*==================================================================================*/
/*==========================       Edge       ======================================*/
/*==================================================================================*/
template<class Data>
struct Edge {
	Vertex<Data> vertex1;
	Vertex<Data> vertex2;

	Edge(Vertex<Data> v1, Vertex<Data> v2) :
			vertex1(v1), vertex2(v2) {
	}
	bool operator==(const Edge& e) {
		return ((vertex1 == e.vertex1 && vertex2 == e.vertex2)
				|| (vertex1 == e.vertex2 && vertex2 == e.vertex1));
	}
	/*
	 * return the other end of the edge
	 * v <--> u
	 * if receive u return v and vice versa.
	 */
	Vertex<Data> getOtherEnd(const Vertex<Data>& v) {
		if (!(vertex1 == v) && !(vertex2 == v)) {
			throw EdgeDoesNotContainVertex();
		}
		return vertex1 == v ? vertex2 : vertex1;
	}
};
template<class Data>
std::ostream& operator<<(std::ostream& os, const Edge<Data>& e) {
	os << e.vertex1.data << "  <--->  " << e.vertex2.data;
	return os;
}
/*==================================================================================*/
/*==========================       GraphElement       ==============================*/
/*==================================================================================*/
/*
 * GraphElement contains Vertex v and all the edges that is connected to it
 */
template<class Data>
struct GraphElement {
	Vertex<Data> v;
	list<Edge<Data>> edges;
	GraphElement(Vertex<Data> v, list<Edge<Data>> l) :
			v(v), edges(l) {
	}
	bool operator==(const Vertex<Data>& vertex) {
		return v == vertex;
	}
	/*
	 * check if vertex contains edge
	 */
	bool containsEdge(const Edge<Data>& e) {
		CheckClass<Edge<Data>> check;
		return check.listcontains(&edges,e);
	}
	void addEdge(const Vertex<Data>& v1, const Vertex<Data>& v2) {
		Edge<Data> e(v1, v2);
		if (!containsEdge(e)) {
			edges.push_back(e);
		}
	}
};
/*==================================================================================*/
/*==========================       Graph       =====================================*/
/*==================================================================================*/
template<class Data>
class Graph {

	list<GraphElement<Data>> graph;

	/**************************************************************/
	/*****************    Private Methods    **********************/
	/**************************************************************/

	/*
	 * private auxiliary function that receives vertex and edge
	 * add the given edge to the vertex in graph.
	 */
	void atPutEdge(const Vertex<Data>& v, const Edge<Data>& e) {
		typename list<GraphElement<Data>>::iterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if ((*iter) == v) {
				break;
			}
		}
		iter->addEdge(e.vertex1, e.vertex2);
	}
	/*
	 * auxiliary recursion function that receives vertex and list of vertices
	 * check if the vertex is in the list if not put it
	 * run on all the vertices that is connected to the given vertex and run the
	 * recursion on them.
	 */
	void returnConnectedAux(list<Vertex<Data>>* vertices,
			const Vertex<Data>& v) {
		CheckClass<Vertex<Data>> check;
		if (check.listcontains(vertices,v)) {
			return;
		}
		list<Edge<int>>* edges;
		this->getEdgesOfVertex(&edges, v);
		typename list<Edge<int>>::iterator edgeIter = edges->begin();
		for (; edgeIter != edges->end(); ++edgeIter) {
			if (!check.listcontains(vertices,v)) {
				(vertices)->push_back(v);
			}
			//recursion
			returnConnectedAux(vertices, edgeIter->getOtherEnd(v));
		}
		delete edges;
	}
	/**************************************************************/
	/*****************    Public Methods    ***********************/
	/**************************************************************/
public:
	/*
	 * add a new vertex to the graph
	 * throw VertexAlreadyExist in case the vertex is already in the graph
	 */
	void addVertex(const Vertex<Data>& v) {
		if (vertexExist(v)) {
			throw VertexAlreadyExist();
		}
		typename list<GraphElement<Data>>::iterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if ((*iter) == v) {
				// graph already contain vertex
				return;
			}
		}
		// graph does not contain Vertex
		GraphElement<Data> element(v, list<Edge<Data>>());
		graph.push_back(element);
	}

	/*
	 * add edge to the graph
	 * v1 <--> v2
	 * the edges that we have in this graph is "undirected graph" so we add the edge to
	 * both vertices v1 and v2
	 */
	void addEdge(const Vertex<Data>& v1, const Vertex<Data>& v2) {
		if ((!vertexExist(v1)) || (!vertexExist(v2))) {
			throw VertexDoesNotExist();
		}
		// graph add edge already check if edge exist
		Edge<Data> e(v1, v2);
		this->atPutEdge(v1, e);
		this->atPutEdge(v2, e);
	}

	/*
	 * return all vertices in the graph.
	 * vertices must allocate vertices in this function
	 * the caller function must free.
	 */
	void getVertices(list<Vertex<Data>>** vertices) {
		*vertices = new list<Vertex<Data>>;
		typename list<GraphElement<Data>>::iterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			(*vertices)->push_back(((GraphElement<Data> ) (*iter)).v);
		}
	}

	/*
	 * edges must be allocated before using this function
	 * return in list (edges) all the edges that is connected to given vertex
	 */
	void getEdgesOfVertex(list<Edge<Data>>** edges, const Vertex<Data>& v) {
		if (!vertexExist(v)) {
			throw VertexDoesNotExist();
		}
		*edges = new list<Edge<Data>>;
		typename list<GraphElement<Data>>::iterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if (*iter == v) {
				break;
			}
		}
		const list<Edge<Data>>& liste = ((GraphElement<Data> ) (*iter)).edges;
		typename list<Edge<Data>>::const_iterator edgeIter = liste.begin();
		for (; edgeIter != liste.end(); ++edgeIter) {
			(*edges)->push_back((*edgeIter));
		}
	}

	/*
	 * check if vertex exist in graph
	 */
	bool vertexExist(const Vertex<Data>& v) {
		typename list<GraphElement<Data>>::iterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if ((*iter) == v) {
				if (((GraphElement<Data> ) (*iter)) == v) {
					// vertex already contains this edge
					return true;
				}
			}
		}
		return false;
	}

	/*
	 * edges must be allocated before using this function
	 */
	void getAllEdges(list<Edge<Data>>** edges) {
		// currently empty
	}

	/*
	 * receive and allocate list of vertices which will contain the vertices that is reached
	 * from the received vertex
	 * example:
	 * edges in graph: 1 <--> 2  and 2 <--> 3
	 * if we send v=1 this function will return 1, 2, 3 (the same result for v=2 and v=3)
	 */
	void returnReached(const Vertex<Data>& v, list<Vertex<Data>>** vertices) {
		if (!vertexExist(v)) {
			throw VertexDoesNotExist();
		}
		*vertices = new list<Vertex<Data>>;
		this->returnConnectedAux(*vertices, v);
	}
};

}

#endif /* GRAPH_H_ */
