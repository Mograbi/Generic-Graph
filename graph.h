/*
 * graph.h
 *
 *  Created on: Dec 20, 2015
 *      Author: Moawiya Mograbi
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "exceptions.h"
#include <list>

namespace mgg {

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

template<class Data>
class Graph {
	// Decelerations
	struct GraphElement;
	struct Vertex;
	struct Edge;

private:
	list<GraphElement> graph;

	/*===============================================*/
	/*=====  GraphElement  ==========================*/
	/*===============================================*/
	struct GraphElement {
		Vertex v;
		list<Edge> edges;
		GraphElement(Vertex v, list<Edge> l) :
				v(v), edges(l) {
		}
		bool operator==(const Vertex& vertex) {
			return v == vertex;
		}
		/*
		 * check if vertex contains edge
		 */
		bool containsEdge(const Edge& e) {
			CheckClass<Edge> check;
			return check.listcontains(&edges, e);
		}
		void addEdge(const Vertex& v1, const Vertex& v2) {
			Edge e(v1, v2);
			if (!containsEdge(e)) {
				edges.push_back(e);
			}
		}
	};
	/*===============================================*/
	/*=====  Vertex  ================================*/
	/*===============================================*/
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
	/*===============================================*/
	/*=====  Edge  ==================================*/
	/*===============================================*/
	struct Edge {
		Vertex vertex1;
		Vertex vertex2;

		Edge(Vertex v1, Vertex v2) :
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
		Vertex getOtherEnd(const Vertex& v) {
			if (!(vertex1 == v) && !(vertex2 == v)) {
				throw floorplan::EdgeDoesNotContainVertex();
			}
			return vertex1 == v ? vertex2 : vertex1;
		}
	};
	/**************************************************************/
	/*****************    Private Methods    **********************/
	/**************************************************************/

	/*
	 * private auxiliary function that receives vertex and edge
	 * add the given edge to the vertex in graph.
	 */
	void atPutEdge(const Vertex& v, const Edge& e) {
		GraphIterator iter = graph.begin();
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
	void returnConnectedAux(list<Data>* vertices, const Vertex& v) {
		CheckClass<Data> check;
		if (check.listcontains(vertices, v.data)) {
			return;
		}
		list<Edge>* edges;
		getEdgesOfVertex(&edges, v.data);
		typename list<Edge>::iterator edgeIter = edges->begin();
		for (; edgeIter != edges->end(); ++edgeIter) {
			if (!check.listcontains(vertices, v.data)) {
				(vertices)->push_back(v.data);
			}
			//recursion
			returnConnectedAux(vertices, edgeIter->getOtherEnd(v));
		}
		delete edges;
	}

	typedef typename list<GraphElement>::iterator GraphIterator;
	/**************************************************************/
	/*****************    Public Methods    ***********************/
	/**************************************************************/
public:
	/*
	 * add a new vertex to the graph
	 * throw VertexAlreadyExist in case the vertex is already in the graph
	 */
	void addVertex(const Data& data) {
		Vertex v(data);
		if (vertexExist(data)) {
			throw VertexAlreadyExist();
		}
		GraphIterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if ((*iter) == v) {
				// graph already contain vertex
				return;
			}
		}
		// graph does not contain Vertex
		GraphElement element(v, list<Edge>());
		graph.push_back(element);
	}

	/*
	 * add edge to the graph
	 * v1 <--> v2
	 * the edges that we have in this graph is "undirected graph" so we add the edge to
	 * both vertices v1 and v2
	 */
	void addEdge(const Data& d1, const Data& d2) {
		Vertex v1(d1);
		Vertex v2(d2);
		if ((!vertexExist(d1)) || (!vertexExist(d2))) {
			throw VertexDoesNotExist();
		}
		// graph add edge already check if edge exist
		Edge e(v1, v2);
		this->atPutEdge(v1, e);
		this->atPutEdge(v2, e);
	}

	/*
	 * return all vertices in the graph.
	 * vertices must allocate vertices in this function
	 * the caller function must free.
	 */
	void getVertices(list<Data>** vertices) {
		*vertices = new list<Data>;
		GraphIterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			(*vertices)->push_back(((GraphElement) (*iter)).v.data);
		}
	}

	/*
	 * edges must be allocated before using this function
	 * return in list (edges) all the edges that is connected to given vertex
	 *
	 * we assume that tempEdge has a constructor which take two parameters of Data
	 * tempEdge edge((*edgeIter).vertex1.data, (*edgeIter).vertex2.data);
	 */
	template<class tempEdge>
	void getEdgesOfVertex(list<tempEdge>** edges, const Data& d) {
		Vertex v(d);
		if (!vertexExist(d)) {
			throw VertexDoesNotExist();
		}
		*edges = new list<tempEdge>;
		GraphIterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if (*iter == v) {
				break;
			}
		}
		const list<Edge>& liste = ((GraphElement) (*iter)).edges;
		typename list<Edge>::const_iterator edgeIter = liste.begin();
		for (; edgeIter != liste.end(); ++edgeIter) {
			tempEdge edge((*edgeIter).vertex1.data, (*edgeIter).vertex2.data);
			(*edges)->push_back(edge);
		}
	}

	/*
	 * check if vertex exist in graph
	 */
	bool vertexExist(const Data& d) {
		Vertex v(d);
		GraphIterator iter = graph.begin();
		for (; iter != graph.end(); ++iter) {
			if ((*iter) == v) {
				if (((GraphElement) (*iter)) == v) {
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
	template<class tempEdge>
	void getAllEdges(list<tempEdge>** edges) {
		// currently empty
	}

	/*
	 * receive and allocate list of vertices which will contain the vertices that is reached
	 * from the received vertex
	 * example:
	 * edges in graph: 1 <--> 2  and 2 <--> 3
	 * if we send v=1 this function will return 1, 2, 3 (the same result for v=2 and v=3)
	 */
	void returnReached(const Data& d, list<Data>** vertices) {
		Vertex v(d);
		if (!vertexExist(d)) {
			throw VertexDoesNotExist();
		}
		*vertices = new list<Data>;
		this->returnConnectedAux(*vertices, v);
	}
};


}

#endif /* GRAPH_H_ */
