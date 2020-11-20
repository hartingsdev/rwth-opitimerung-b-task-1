#include "myShortestPath.h"
#include <queue>

/*
 * Define datatype for outgoing edges of vertices like in types.h
 */

typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIterator;
typedef boost::property_map<Graph, boost::edge_weight_t>::const_type EdgeWeightMap;
typedef std::pair<unsigned int, Vertex> VertexDistance;

std::vector<Vertex> my_shortest_path(const Graph &g, const Vertex &startVertex, const Vertex &endVertex) {
    std::vector<Vertex> path;
    // compute a shortest path between startVertex and endVertex and save it in
    // the vector >>path<<

    /*
     * Zu erst wird die Anzahl an Vertices aus dem Graphen gelesen, dies wird benötigt, damit die Vectoren dist
     * (Distanz zum StartVertx) und prev_vertex (Vorgängervertex auf dem kürzesten S->T Pfad) initalisiert werden
     * können. Hierbei wird die Distanz für alle Vertices auf unendlich - hier durch die größtmögliche (unsigned int)
     * Zahl kodiert - gesetzt und der Vorgänger ist für alle der startVertex.
     *
     * Die größtmögliche Zahl wird hierbei mithilfe des Macros UINT_MAX aus dem climits header bezogen.
     */
    const int number_of_vertices = num_vertices(g);

    std::vector<Vertex> prev_vertex(number_of_vertices, startVertex);
    std::vector<unsigned int> dist(number_of_vertices, UINT_MAX);

    /*
     * Um den nächsten abzuarbeiten Knoten zu bestimmen, werden die Knoten inklusiver ihrer Gewichte in eine
     * Prioritätswarteschlange gespeichert. Das erste Eintrag ist in einer "normalen" Prioritätswarteschlange der
     * Knoten mit dem größten Gewicht / der größten Distanz. Um dies zu ändern, kann die Sortierung bei der Erstellung
     * angegeben werden. Es wird std::greater<> genutzt, um die Sortierung auf aufsteigend zu änderen. Der nächste
     * abzuarbeitende Knoten muss das geringste Gewicht der noch abzuarbeitenden Knoten haben.
     */
    std::priority_queue<VertexDistance, std::vector<VertexDistance>, std::greater<VertexDistance>> pq;

    /*
     * Damit Vertices nicht zweimal abgearbeitet werden, werden alle bearbeitenden Vertices in einem Vector gespeichert.
     * Um zu prüfen, ob der Eintrag bereits im Vector ist, wird ein Iterator benötigt, welcher über alle Elemente im
     * Vector iteriert.
     */
    std::vector<int> completed_vertices;
    std::vector<int>::iterator completed_vertices_iterator;

    /*
     * Die Gewichte der Kanten werden aus dem edge_weight Property ausgelesen.
     * Für die Iteration aller Kanten eines Vertex werden zwei Iteratoren benötigt, der erste welcher über die
     * Kantenliste läuft und der zweiter der das Ende der Kantenliste anzeigt.
     *
     * Für die Einfachheit wird eine Variable des Typs Edge genutzt, um die *dereferenzierte* Kantenitertor gesprichert
     * wird für den einfacheren Zugriff.
     *
     * Die Variable target_vertex ist das Ziel einer Kante ausgehen vom derzeitig betrachteten Vertex.
     */
    const EdgeWeightMap weights = get(boost::edge_weight, g);
    OutEdgeIterator out_edge_i, out_edge_end;
    Edge working_edge;
    Vertex target_vertex;

    /*
     * Nach der Initalisierung wird zuerst - wie im Dijkstra Algorithmus vorgesehen - die Distanz vom startVertex auf
     * die richtige Distanz 0 gesetzt. Danach wird der Startknoten in die abzuarbeitende Prioritätswarteschlange mit
     * dem Gewicht der Distanz eingefügt.
     */
    dist[startVertex] = 0;
    pq.push(std::make_pair(0, startVertex));

    /*
     * Solange die Prioritätswarteschlange mit den zu abarbeiteten Knoten nicht leer ist, müssen diese Knoten bearbeitet
     * werden.
     */
    while (!pq.empty()) {

        /*
         * Aus der Prioritätswarteschlange wird das erste Element - mit der geringsten Distanz -, genaur der
         * erste Knoten, ausgelesen und im Anschluss aus der Warteschlange gelöscht.
         */
        Vertex working_vertex = pq.top().second;
        pq.pop();

        /*
         * Im Anschluss wird geprüft, ob der Knoten bereits abgearbeitet worden ist, indem über den Vector iteriert wird.
         * Sollte der Knoten bereits abgearbeitet worden sein, wird der nächste Eintrag der Prioritätswarteschlange
         * abgerabeitet.
         *
         * Sollte der Knoten noch nicht abgearbeitet worden sein, wird dieser im Anschluss abgearbeitet und in die Liste
         * der abgearbeiteten Knoten aufgenommen, damit dieser nicht erneut bearbeitet wird.
         */
        completed_vertices_iterator = std::find(completed_vertices.begin(), completed_vertices.end(), working_vertex);
        if (completed_vertices_iterator != completed_vertices.end()) {
            // Das Element wurde schon bearbeitet und wird nicht weiter betrachtet.
            continue;
        }
        completed_vertices.push_back(working_vertex);

        /*
         * Für die Iterierung über die Kanten eines Vertex werden zwei Iteratoren benötigt. Der zweiter Iterator wird
         * benötigt, um zu prüfen, ob bei der Iterierung bereits das Ende erreicht worden ist. Der erste Iterator wird
         * genutzt, um die verschiedenen Kanten zu adressieren.
         */
        tie(out_edge_i, out_edge_end) = out_edges(working_vertex, g);

        /*
         * Im Folgendem werden über alle verfügbaren Kanten des Vertex iteiert und die Distanz der Nachbarn bei Bedarf
         * anzupassen.
         */
        for (; out_edge_i != out_edge_end; out_edge_i++) {
            /*
             * Für die Übersicht wird der Kanteniterator derefernziert und das Objekt in die Variable working_edge.
             * target_vertex ist hierbei der Endkoten der betrachteten Kante.
             */
            working_edge = *out_edge_i;
            target_vertex = target(working_edge, g);

            /*
             * Es wird geprüft, ob die Entfernung des Startknoten (der Kante) plus Kantengewicht echt kleiner als
             * die derzeitig hinterlegte Entfernung des Endknoten ist.
             *
             * Sollte die Distanz echt kleiner sein, wird die Distanz des Endknoten der Kange sowie der Vorgänger
             * aktuallisiert.
             */
            if (dist[working_vertex] + weights[working_edge] < dist[target_vertex]) {
                dist[target_vertex] = dist[working_vertex] + weights[working_edge];
                prev_vertex[target_vertex] = working_vertex;
            }

            /*
             * Im Anschluss wird der Endknoten zur Abarbeitung mit seinem neuen Kantengewicht in die
             * Prioritätswarteschlange eingefügt. Sollte der Knoten bereits abgearbeitet sein, wird dieser nicht erneut
             * in die Prioritätswarteschlange eingeüfgt.
             */
            completed_vertices_iterator = std::find(completed_vertices.begin(), completed_vertices.end(),
                                                    target_vertex);
            if (completed_vertices_iterator == completed_vertices.end()) {
                pq.push(std::make_pair(dist[target_vertex], target_vertex));
            }
        }
    }

    /*
     * Nach dem für alle Knoten der kürzeste Weg zum startVertex bestimmt worden ist, muss nun nur noch der kürzeste
     * Weg vom Startknoten zum Endknoten in der return Variable path abgespeichert werden.
     *
     * Dafür wird vom endVertex rückwärts immer der Vorgänger an die erste Position des vectors eingefügt.
     */
    path.insert(path.begin(), endVertex);
    Vertex working_vertex = prev_vertex[endVertex];
    while (working_vertex != startVertex) {
        path.insert(path.begin(), working_vertex);
        working_vertex = prev_vertex[working_vertex];
    }
    path.insert(path.begin(), startVertex);

    // return path
    return path;
}

/*
 * In der Prioritätswarteschlange können Knoten zu beginn doppelt vorkommen also ist die Laufzeit beschränkt mit O(2|V|).
 * Pro Knoten können maximal |E| Kanten betrachtet werden. Damit ergibt sich für die Erstellung aller kürzesten S-T Wege
 * (festes S) eine Laufzeit von O(2|V|*|E|) = O(|V|*|E|)
 * Die Erstellung des Ergebnispfads geschieht in O(|V|),
 * da der Pfad maximal die Länge der Anzahl der Vertices besitzt.
 *
 * Die theoretische Laufzeit ist hiermit O(|V|*|E| + |V|).
 */