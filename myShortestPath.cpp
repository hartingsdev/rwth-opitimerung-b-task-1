#include "myShortestPath.h"
#include <queue>

/*
 * Define datatype for outgoing edges of vertices like in types.h
 */

typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIterator;
typedef boost::property_map<Graph, boost::edge_weight_t>::const_type EdgeWeightMap;
typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndex;


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

    const int number_of_vertices = num_vertices(g);

    std::vector<Vertex> prev_vertex(number_of_vertices, startVertex);
    std::vector<unsigned int> dist(number_of_vertices, UINT_MAX);


    typedef std::pair<unsigned int, Vertex> VertexDistance;
    std::priority_queue<VertexDistance, std::vector<VertexDistance>, std::greater<VertexDistance>> pq;

    pq.push(std::make_pair(0, startVertex));
    dist[startVertex] = 0;

    std::vector<int> completed_vertices;
    std::vector<int>::iterator completed_vertices_iterator;

    int test = 0;
    while (!pq.empty()) {
        test++;
        Vertex working_vertex = pq.top().second;
        pq.pop();
        completed_vertices_iterator = std::find(completed_vertices.begin(), completed_vertices.end(), working_vertex);
        if (completed_vertices_iterator != completed_vertices.end()) {
            // Element wurde schon bearbeitet
            continue;
        }

        completed_vertices.push_back(working_vertex);

        const EdgeWeightMap weights = get(boost::edge_weight, g);
        OutEdgeIterator out_i, out_end;
        Edge e;
        Vertex target_vertex;
        tie(out_i, out_end) = out_edges(working_vertex, g);
        for(; out_i != out_end; out_i++) {
            e = *out_i;
            target_vertex = target(e, g);

            if (dist[working_vertex] + weights[e] < dist[target_vertex]){
                dist[target_vertex] = dist[working_vertex] + weights[e];
                prev_vertex[target_vertex] = working_vertex;
            }

            completed_vertices_iterator = std::find(completed_vertices.begin(), completed_vertices.end(), target_vertex);
            if (completed_vertices_iterator == completed_vertices.end()) {
                pq.push(std::make_pair(dist[target_vertex], target_vertex));
            }
        }
    }

    path.insert(path.begin(), endVertex);
    Vertex working_vertex = prev_vertex[endVertex];
    while(working_vertex != startVertex) {
        path.insert(path.begin(), working_vertex);
        working_vertex = prev_vertex[working_vertex];
    }
    path.insert(path.begin(), startVertex);

    // return path
    return path;
}
