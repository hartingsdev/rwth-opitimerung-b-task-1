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
     * Berechne den kürzesten Weg zwischen einem Terminal und allen anderen Terminals, um einen Graphen mit weniger
     * Kanten und Knoten zu erhalten. Hierfür wird der vorher implementierte Dijkstra Algorithmus genutzt. Dazu werden
     * die berechneten Vertices in eine Liste eingefügt.
     */
    std::vector<Vertex> unionPath;
    for (; vector_iterator != terminals.end(); vector_iterator++) {
        std::vector<Vertex> resultPath = my_shortest_path(g, startVertex, *vector_iterator);
        unionPath.insert(unionPath.end(), resultPath.begin(), resultPath.end());
    }

    /*
     * Im Anschluss werden die redundanten Vertices aus der Liste entfernet, damit im Anschluss eine kleiner Liste
     * überprüft werden muss.
     */
    std::sort(unionPath.begin(), unionPath.end());
    unionPath.erase(unique(unionPath.begin(), unionPath.end()), unionPath.end());

    /*
     * Danach wird der bestehende Graph kopiert und die nicht benötigten Vertices entfernet, damit auf den neuen Graph
     * der MST Algorithmus angewendet werden kann. Nur die Vertices in der vorher berechneten Liste bleiben dem Graph
     * erhalten.
     */
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

    /*
     * Zu letzt wird auf dem neuen Graphen der MST-Alogrithmus ausgeführt.
     */
    steiner_tree = my_spanning_tree(new_graph);

    // return steiner tree
    return steiner_tree;
}

/*
 * A)
 * Sei G''(V'', E'') ein neuer Graph mit V'' = T und E'' Teilmenge von E die Kanten (u,v) für die gilt u in V'' und
 * v in V''. Wähle beliebigen Knoten v in V'' und berechne alle kürzesten Wege zu Knoten in T. Nachdem der Graph sicher
 * zusammenhängend ist den minimalen Spannbaum von G''(V'', E'') berechnen und als G'(V', E')bennen.
 *
 * B)
 * Es handelt sich nicht immer um die optimale Lösung, da nur die küresten Wege von einem Terminalknoten zu allen anderen
 * betrachtet werden. Für den optimalen SteinerTree müssten von allen Terminalknoten die kürzesten Wege zu den anderen
 * Terminalknoten berechnet werden.
 *
 * Für den folgenden Graph wird nicht die optimale Lösung berechnet.
 * V1 --1-- V2 --1-- V3
 * |2
 * V4 /(Kante zwischen V3 und V4 mit Gewicht 1)
 * T = {1,3,4}
 * Kürzeste Wege von 1:
 * V3: {{V1,V2}, {V2,V3}} -> w:2
 * V4: {{V1,V4}} -> w:2
 *
 * Berchneter Steiner Tree: {{V1,V2}, {V2,V3}, {V1, V4}} -> Summe der Gewichte: 4
 * Optimaler Steiner Tree: {{V1,V4}, {V4, V3}} -> Summe der Gewichte: 3
 *  -> dieser wird aber nicht betrachtet, da die Kante {V3,V4} nicht im kürzesten Pfad von V1 liegt.
 *
 * D)
 * O(|Terminals| -1 * (|V|*|E| + |V|) + V + O(MST)
 */