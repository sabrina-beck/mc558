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
        unsigned int pathsAfterGreen;
        unsigned int pathsAfterYellow;
        unsigned int pathsAfterRed;
        bool visited;
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
        void addEdge(Edge *edge);
        int countPossiblePaths();
        void print();

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

        void countPossiblePaths(Color previousEdgeColor, Vertex* vertex);
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
    Vertex* destinyVertex = this->vertexes[this->destiny];
    destinyVertex->pathsAfterGreen = 1;
    destinyVertex->pathsAfterYellow = 1;
    destinyVertex->pathsAfterRed = 1;
    destinyVertex->visited = 1;

    Vertex* originVertex = this->vertexes[this->origin];
    this->countPossiblePaths(Green, originVertex);
    return originVertex->pathsAfterGreen;
}

/*
 * Counts all the possible paths that meets whith the established conditions
 * for the subgraph that has vertex as its origin
 * This is the recursive function. It receives the previous edge color to check
 * the conditions
 */
void Graph::countPossiblePaths(Color previousEdgeColor, Vertex* vertex) {
    if(vertex->visited) {
        return;
    }

    for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        Edge* edge = *it;
        Vertex* destinyVertex = this->vertexes[edge->getDestiny()];
        if(!destinyVertex->visited) {
            this->countPossiblePaths(edge->getColor(), destinyVertex);
        }

        if(edge->getColor() == Green) {
            vertex->pathsAfterGreen += destinyVertex->pathsAfterGreen;
            vertex->pathsAfterYellow += destinyVertex->pathsAfterGreen;
            vertex->pathsAfterRed += destinyVertex->pathsAfterGreen;
        } else if(edge->getColor() == Yellow) {
            vertex->pathsAfterGreen += destinyVertex->pathsAfterYellow;
            vertex->pathsAfterYellow += destinyVertex->pathsAfterYellow;
        } else {
            vertex->pathsAfterGreen += destinyVertex->pathsAfterRed;
        }
    }

    vertex->visited = true;
}

string colorName(int i) {
    if(i == 0) {
        return "VERDE";
    }

    if(i == 1) {
        return "AMARELO";
    }
    
    return "VERMELHO";
}

void Graph::print() {
    for(unsigned int i = 0; i < this->getSize(); i++) {
        Vertex* vertex = this->vertexes[i];
        cout << "Vertice no " << i << "\n";
        cout << "\t Caminhos depois de aresta VERDE " << vertex->pathsAfterGreen << "\n";
        cout << "\t Caminhos depois de aresta AMARELA " << vertex->pathsAfterYellow << "\n";
        cout << "\t Caminhos depois de aresta VERMELHA " << vertex->pathsAfterRed << "\n";
        cout << "\t Visitado? " << vertex->visited << "\n";

        for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
            Edge* edge = *it;
            cout << "\t(" << edge->getOrigin() << ", " << edge->getDestiny() << ") ";
            cout << colorName(edge->getColor()) << "\n";
        }
        cout << "\n";
    }
}