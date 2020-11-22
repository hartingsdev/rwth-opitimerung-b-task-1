#include <iostream>
#include "mySteinerTree.h"
#include "myShortestPath.h"
#include "mySpanningTree.h"

#include <boost/graph/copy.hpp>

std::vector<Edge> my_steiner_tree(const Graph &g,
                                  const std::vector<Vertex> &terminals) {
    std::vector<Edge> steiner_tree;
    // compute a steiner tree for g with terminals g

    auto vector_iterator = terminals.begin();
    Vertex startVertex = *vector_iterator++;

    /*
     *
     */
    std::vector<Vertex> unionPath;
    for (; vector_iterator != terminals.end(); vector_iterator++) {
        std::vector<Vertex> resultPath = my_shortest_path(g, startVertex, *vector_iterator);
        unionPath.insert(unionPath.end(), resultPath.begin(), resultPath.end());
    }

    std::sort(unionPath.begin(), unionPath.end());
    unionPath.erase(unique(unionPath.begin(), unionPath.end()), unionPath.end());

    Graph new_graph;
    boost::copy_graph(g, new_graph);

    VertexIterator v_i, v_end, next;
    boost::tie(v_i, v_end) = vertices(new_graph);


    for (next = v_i; v_i != v_end; v_i = next) {
        ++next;
        auto completed_vertices_iterator = std::find(unionPath.begin(), unionPath.end(), *v_i);
        if (completed_vertices_iterator == unionPath.end()) {
            remove_vertex(*v_i, new_graph);
        }
    }

    steiner_tree = my_spanning_tree(new_graph);

    // return steiner tree
    return steiner_tree;
}
