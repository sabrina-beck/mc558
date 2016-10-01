/*
 * Name: Sabrina Beck Angelini
 * RA: 157240
 */

#include <iostream>
#include <list>
using namespace std;

typedef enum { Green, Yellow, Red } Color;

/*
 * Represents a edge (origin, destiny)
 */
class Edge {
    public:
        Edge(unsigned int origin, unsigned int destiny, Color color) {
            this->origin = origin;
            this->destiny = destiny;
            this->color = color;
        }
        unsigned int getOrigin() {
            return this->origin;
        }
        unsigned int getDestiny() {
            return this->destiny;
        }
        Color getColor() {
            return this->color;
        }
    private:
        unsigned int origin;
        unsigned int destiny;
        Color color;
};

/*
 * Represents a vertex with its edges
 */
class Vertex {
    public:
        list<Edge*> edges; // incident edges
        // count possible paths starting on this vertex
        // when arriving to this vertex through a edge of a dertemined color
        unsigned int pathsAfterGreen; // from a green edge
        unsigned int pathsAfterYellow; // from a yellow edge
        unsigned int pathsAfterRed; // from a red edge
        bool visited; // indicates if this vertex was already visited
        Vertex(){
            this->pathsAfterGreen = 0;
            this->pathsAfterYellow = 0;
            this->pathsAfterRed = 0;
            this->visited = false;
        }
};

/*
 * Represents a graph with vertexes and each vertex with its edges
 */
class Graph {
    public:
        void addEdge(Edge *edge); // add a new edge to the graph
        int countPossiblePaths(); // count possible paths from the origin to the destiny

        Graph(unsigned int size, unsigned int origin, unsigned int destiny) {
            this->vertexes = new Vertex*[size];
            for(unsigned int i = 0; i < size; i++) {
                this->vertexes[i] = new Vertex();
            }
            this->size = size;
            this->origin = origin;
            this->destiny = destiny;
        }
        Vertex **getVertexes() {
            return this->vertexes;
        }
        unsigned int getSize() {
            return this->size;
        }
        unsigned int getOrigin() {
            return this->origin;
        }
        unsigned int getDestiny() {
            return this->destiny;
        }
    private:
        Vertex **vertexes;
        unsigned int origin;
        unsigned int destiny;
        unsigned int size;

        void countPossiblePaths(Vertex* vertex);
};

Graph* readGraph();

int main() {
    // reads the input graph
    Graph* graph = readGraph();

    // counts the possible paths
    int pathsCount = graph->countPossiblePaths();

    // prints the number of possible paths
    cout << pathsCount << "\n";

    return 0;
}

/*
 * Reads an input graph
 */
Graph* readGraph() {
    unsigned int vertexesNumber,
        edgesNumber,
        origin,
        destiny;
    
    // reads the number of vertexes (n) and the number of edges (m) in the graph
    cin >> vertexesNumber >> edgesNumber;
    // reads the paths origin and destiny
    cin >> origin >> destiny;
    
    // creates a graph with n vertexes (1, 2, ..., n-1) an with origin and destiny
    Graph *graph = new Graph(vertexesNumber, origin, destiny);

    // for each edge
    for(unsigned int i = 0; i < edgesNumber; i++) {
        unsigned int x, y, colorId;
        Color color;
        // reads the edge origin and destiny
        cin >> x >> y;
        // reads the edge color
        cin >> colorId;

        color = static_cast<Color>(colorId);

        // creates and add the read edge
        Edge *edge = new Edge(x, y, color);
        graph->addEdge(edge);
    }

    return graph;
}

/*
 * Add an edge to the graph
 */
void Graph::addEdge(Edge *edge) {
    // fetch the origin vertex
    Vertex* vertex = this->vertexes[edge->getOrigin()];
    // add the edge to the adjacency list of its origin vertex
    vertex->edges.push_back(edge);
}

/*
 * Counts all the possible paths that meets with the established conditions for this graph
 */
int Graph::countPossiblePaths() {
    // Initializes the destiny vertex
    Vertex* destinyVertex = this->vertexes[this->destiny];
    destinyVertex->pathsAfterGreen = 1;
    destinyVertex->pathsAfterYellow = 1;
    destinyVertex->pathsAfterRed = 1;
    // we mark the destiny vertex as visited because we already calculated
    // its possible paths
    destinyVertex->visited = true;

    Vertex* originVertex = this->vertexes[this->origin];
    // we count the possible paths beginning on the origin vertex
    this->countPossiblePaths(originVertex);
    // since a green edge allows us to catch any path independent of its edges color
    // we consider that the origin vertex has a previous edge of the green color
    return originVertex->pathsAfterGreen;
}

/*
 * Counts all the possible paths that meets whith the established conditions
 * for the subgraph that has vertex as its origin
 * This is the recursive function.
 */
void Graph::countPossiblePaths(Vertex* vertex) {
    // we check each edge for the current vertex
    for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        Edge* edge = *it;
        Vertex* destinyVertex = this->vertexes[edge->getDestiny()];

        // if the destiny vertex of the current edge hasn't been visited yet
        // we make the depth search on it to calculate its paths
        if(!destinyVertex->visited) {
            this->countPossiblePaths(destinyVertex);
        }

        if(edge->getColor() == Green) {
            // sum the destiny vertex possible paths that comes after a green edge
            // to all the possibilities of previous edge color,
            // since the current edge is green and any previous color allows the
            // next edge on the path to be green
            vertex->pathsAfterGreen += destinyVertex->pathsAfterGreen;
            vertex->pathsAfterYellow += destinyVertex->pathsAfterGreen;
            vertex->pathsAfterRed += destinyVertex->pathsAfterGreen;
        } else if(edge->getColor() == Yellow) {
            // sum the destiny vertex possible paths that comes after a yellow edge
            // to the counter of possible paths where the previous edge is green or yellow,
            // since the current edge is yellow and only green and yellow edges allows
            // the next edge on the path to be yellow
            vertex->pathsAfterGreen += destinyVertex->pathsAfterYellow;
            vertex->pathsAfterYellow += destinyVertex->pathsAfterYellow;
        } else { // edge->color == Red
            // sum the destiny vertex possible paths that comes after a red edge
            // to the counter of possible paths where the previous edge is green,
            // since the current edge is red and only green edges allows
            // the next edge on the path to be red
            vertex->pathsAfterGreen += destinyVertex->pathsAfterRed;
        }
    }

    // mark the current vertex as visited
    vertex->visited = true;
}