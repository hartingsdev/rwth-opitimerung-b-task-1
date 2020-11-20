#include "myShortestPath.h"

#include <iostream>
#include <queue>

//class WTS {
//public:
//    int weight = 0;
//};
//
//class Compare {
//public:
//    bool operator()(WTS &a, WTS &b) {
//        return a.weight < b.weight;
//    }
//};


/*
 * Define datatype for outgoing edges of vertices like in types.h
 */

typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIterator;
typedef boost::property_map<Graph, boost::edge_weight_t>::const_type EdgeWeightMap;

std::vector<Vertex> my_shortest_path(const Graph &g, const Vertex &startVertex, const Vertex &endVertex) {
    std::vector<Vertex> path;
    // compute a shortest path between startVertex and endVertex and save it in
    // the vector >>path<<
    //TODO
    /*
     * Initalisierung der Entfernung
     * Ich verwende Integer für die Entfernung, da in den .lgf Dateien nur ganzzahlige Entfernungen hinterlegt sind.
     * In einem anderen Anwendungsfall wären float oder double wahrscheinlich besser.
     *
     * Da im Dijkstra Algorithmus nur positive Kanten verwendete werden, wird die Entefernung im Datentype 'unsigned int'
     * gespeichert. Die anfängliche Distanz von unendlich für alle Knoten, wird mit dem größtmöglichen unsigned int Wert
     * kodiert, da keine größeren Werte abgespeichert werden. Der Wert wird über die Standard Library abgerufen.
     * Die Entfernungen des startKnoten muss auf 0 zurückgesetzt werden.
     */
    //unsigned int dist[nodeCount] = {INT_MAX};
    //dist[startNode] = 0;

    typename boost::property_map<Graph, boost::vertex_index_t>::type index = get(boost::vertex_index, g);
    EdgeWeightMap weights = get(boost::edge_weight, g);
    std::cout << "out-edges: ";
    OutEdgeIterator out_i, out_end;
    Edge e;
    tie(out_i, out_end) = out_edges(startVertex, g);
    for (; out_i != out_end; ++out_i) {
        e = *out_i;
        Vertex src = source(e, g);
        Vertex targ = target(e, g);
        std::cout << "(" << index[src] << "," << index[targ] << " -- " << weights[e] << ") ";
    }
    std::cout << std::endl;

    //std::priority_queue<WTS, std::vector<WTS>, Compare> pq;
    // return path
    return path;
}
