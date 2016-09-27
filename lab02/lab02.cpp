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
        list<Edge*> edges;
};

/*
 * Represents a graph with vertexes and each vertex with its edges
 */
class Graph {
    public:
        void addEdge(Edge *edge);
        int countPossiblePaths();

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

        int countPossiblePaths(Color previousEdgeColor, Vertex* vertex);
        bool canFollowThisPath(Color previousEdgeColor, Edge* currentEdge);
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
    // fetch the edges origin vertex
    Vertex* vertex = this->vertexes[edge->getOrigin()];
    // add the edge to the adjacency list of its origin vertex
    vertex->edges.push_back(edge);
}

/*
 * Counts all the possible paths that meets whith the established conditions for this graph
 */
int Graph::countPossiblePaths() {
    // fetch the graphs origin vertex
    Vertex* originVertex = this->vertexes[this->origin];
    // how green is the color that allows us to choose any color as the next step
    // we can use it as the previous color to the origin vertex,
    // since we don't have a previous edge at this point
    // we count the number of possible paths starting on originVertex
    return this->countPossiblePaths(Green, originVertex);
}

/*
 * Counts all the possible paths that meets whith the established conditions
 * for the subgraph that has vertex as its origin
 * This is the recursive function. It receives the previous edge color to check
 * the conditions
 */
int Graph::countPossiblePaths(Color previousEdgeColor, Vertex* vertex) {
    // if the current vertex is the destiny vertexes, we found a complete path
    if(vertex == this->vertexes[this->destiny]) {
        return 1; // counting the found path
    }

    // otherwise, we have to keeping searching path the remaining vertex on the current path
    int count = 0;
    //for each edge of the current path
    for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        Edge* edge = *it;

        // if the previous edge and the current edge has colors that allows
        // the path to continue on the current edge
        if(this->canFollowThisPath(previousEdgeColor, edge)) {
            Vertex* destinyVertex = this->vertexes[edge->getDestiny()];
            // we analyze the possible paths by traveling through this edge
            // the paths count is the sum of all possible paths found by searching
            // on each edge of the current vertex 
            count = count + this->countPossiblePaths(edge->getColor(), destinyVertex);
        }
    }
    return count;
}

/*
 * Check if we can follow a certain path by analyzing the previous edge color and current one
 */
bool Graph::canFollowThisPath(Color previousEdgeColor, Edge* currentEdge) {
    // if the previous edge is yellow, we can't travel through a red edge
    if(previousEdgeColor == Yellow) {
        return currentEdge->getColor() != Red;
    }

    // if the previous edge is red, we can only travel through a green edge
    if(previousEdgeColor == Red) {
        return currentEdge->getColor() == Green;
    }

    // if the previous edge is green, we can travel through any edge
    return true;
}