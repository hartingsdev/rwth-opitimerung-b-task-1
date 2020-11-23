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
    /*
     * Initalisierung benötigter Variablen um durch die Liste der Edges zu laufen, die Edge und
     * den Source- und Targetvertex zwischen zu speichern. Aus dem Graphen die Gewicht der Kanten erhalten, um später
     * den MST berechnen zu können.
     */
    typedef boost::property_map<Graph, boost::edge_weight_t>::const_type EdgeWeightMap;
    const EdgeWeightMap weights = get(boost::edge_weight, g);
    EdgeIterator e_i, e_end;
    Edge e;
    Vertex source_vertex, target_vertex;

    /*
     * Um immer die Kante mit dem geringsten Gewicht auszuwählen werden diese im Verlauf in aufsteigenden Reihenfolge
     * in einer Prioirtätswarteschlange eingefügt, die dafür sorgt, dass das erste Element eine Kante mit minimalen
     * Restgewicht ist.
     */
    typedef std::pair<unsigned int, Edge> EdgeWeight;
    std::priority_queue<EdgeWeight, std::vector<EdgeWeight>, std::greater<EdgeWeight>> pq;

    /*
     * Die Edge Iteratoren werden genutzt, um über die Liste der Edges zu iterieren. Jede Edge wird anhand ihres Gewichts
     * in die Prioritätswarteschlange eingefügt.
     */
    tie(e_i, e_end) = edges(g);
    for (; e_i != e_end; e_i++) {
        pq.push(std::make_pair(weights[*e_i], *e_i));
    }

    /*
     * Erstellung eines Counter sowie Berechnung der maximalen Kanten im Spanning Tree. Für einen Minimalenspannbaum
     * dürfen nur die Anzahl der Vertices - 1 Kanten im SPT sein. Da der Vergleich mit < durchgeführt wird, wird auf
     * die maximale Anzahl der Vertices noch +1 addiert, sodass die Anzahl der Kanten im Spanning Tree < Anzahl der
     * Vertices sein muss.
     *
     * Danach wird ein Disjunktes Set erstellt, damit geprüft werden kann, ob das hinzufügen einer Kante einen Kreis
     * bilden würde.
     */
    int counter = 0;
    int max_edges = num_vertices(g);
    DisjointSet vertex_set(max_edges);

    /*
     * Solange die Abbruchbedingungen (Die Anzahl der benötigen Kanten ist erreicht und Es gibt keine weiteren Kanten)
     * nicht erreicht ist wird der folgende Programmcode ausgeführt.
     */
    while (counter < max_edges && !pq.empty()) {
        /*
         * Erstes Element / Kante aus der Prioritätswarteschlange abrufen und aus der Warteschlange entfernen.
         */
        e = pq.top().second;
        pq.pop();

        /*
         * Start- und Endknoten der Kante bestimmen, um die Kreisfreiheit zuprüfen.
         */
        source_vertex = source(e, g);
        target_vertex = target(e, g);

        /*
         * Wenn der Start- und Endknoten nicht im gleichen Set sind, kann die Kante dem SPT hinzugefügt werden.
         * Um eine weitere Kreisfreiheit zu grantieren, werden die beiden Sets vereinigt. Danach wird die Variable in
         * der die Anzahl an Kanten im SPT incrementiert.
         */
        if (vertex_set.find(source_vertex) !=  vertex_set.find(target_vertex)) {
            vertex_set.Union(source_vertex, target_vertex);
            spanning_tree.push_back(e);
            counter++;
        }
    }

    // return spanning tree
    return spanning_tree;
}

/*
 * Laufzeit:
 *  - Einfügen in PQ: E
 *  - Schleife wird |V|-1 mal durchlaufen
 *      - Suchen, ob im Set V (Da rekrusive Aufrufe möglich sind)
 *      - Union in 1
 *
 *  O(|V|-1 * V)
 */