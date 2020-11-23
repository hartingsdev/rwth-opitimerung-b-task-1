#include "mySpanningTree.h"

#include <queue>

/*
 * Disjunkt Set idea copied from:
 * https://www.geeksforgeeks.org/disjoint-set-data-structures/
 */
class DisjointSet {
    int *rank, *parent, n;

public:
    // Constructor to create and
    // initialize sets of n items
    DisjointSet(int num_elements)
    {
        rank = new int[num_elements];
        parent = new int[num_elements];
        this->n = num_elements;
        makeSet();
    }

    // Creates n single item sets
    void makeSet()
    {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Finds set of given item x
    int find(int x)
    {
        // Finds the representative of the set
        // that x is an element of
        if (parent[x] != x) {

            // if x is not the parent of itself
            // Then x is not the representative of
            // his set,
            parent[x] = find(parent[x]);

            // so we recursively call Find on its parent
            // and move i's node directly under the
            // representative of this set
        }

        return parent[x];
    }

    // Do union of two sets represented
    // by x and y.
    void Union(int x, int y)
    {
        // Find current sets of x and y
        int xset = find(x);
        int yset = find(y);

        // If they are already in same set
        if (xset == yset)
            return;

        // Put smaller ranked item under
        // bigger ranked item if ranks are
        // different
        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
        }
        else if (rank[xset] > rank[yset]) {
            parent[yset] = xset;
        }

            // If ranks are same, then increment
            // rank.
        else {
            parent[yset] = xset;
            rank[xset] = rank[xset] + 1;
        }
    }
};

std::vector<Edge> my_spanning_tree(const Graph &g) {
    std::vector<Edge> spanning_tree;
    // compute a minimal spanning tree for g
    typedef boost::property_map<Graph, boost::edge_weight_t>::const_type EdgeWeightMap;
    const EdgeWeightMap weights = get(boost::edge_weight, g);
    EdgeIterator e_i, e_end;

    typedef std::pair<unsigned int, Edge> EdgeWeight;
    std::priority_queue<EdgeWeight, std::vector<EdgeWeight>, std::greater<EdgeWeight>> pq;

    tie(e_i, e_end) = edges(g);

    for (; e_i != e_end; e_i++) {
        pq.push(std::make_pair(weights[*e_i], *e_i));
    }


    int counter = 0;
    int max_edges = num_vertices(g);
    DisjointSet vertex_set(max_edges);

    while (counter < max_edges && !pq.empty()) {
        Edge e = pq.top().second;
        pq.pop();

        Vertex source_vertex = source(e, g);
        Vertex target_vertex = target(e, g);

        if (vertex_set.find(source_vertex) !=  vertex_set.find(target_vertex)) {
            vertex_set.Union(source_vertex, target_vertex);
            spanning_tree.push_back(e);
            counter++;
        }
    }

    // return spanning tree
    return spanning_tree;
}
