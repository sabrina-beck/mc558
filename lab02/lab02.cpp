/*
 * Name: Sabrina Beck Angelini
 * RA: 157240
 */

#include <iostream>
#include <vector>
#include <list>
using namespace std;

typedef enum { GREEN = 0, YELLOW = 1, RED = 2 } Color;

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

class Vertex {
    public:
        list<Edge*> edges;
};

class Graph {
    public:
        void addEdge(Edge *edge);
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
};

Graph* readGraph();

int main() {
    Graph* graph = readGraph();

    graph->print();

    return 0;
}

Graph* readGraph() {
    unsigned int vertexesNumber,
        edgesNumber,
        origin,
        destiny;
    
    cin >> vertexesNumber >> edgesNumber;
    cin >> origin >> destiny;
    
    Graph *graph = new Graph(vertexesNumber, origin, destiny);

    for(unsigned int i = 0; i < edgesNumber; i++) {
        unsigned int x, y, colorId;
        Color color;
        cin >> x >> y;
        cin >> colorId;

        color = static_cast<Color>(colorId);

        Edge *edge = new Edge(x, y, color);
        graph->addEdge(edge);
    }

    return graph;
}

void Graph::addEdge(Edge *edge) {
    Vertex* vertex = this->vertexes[edge->getOrigin()];
    vertex->edges.push_back(edge);
}

void Graph::print() {
    for(unsigned int i = 0; i < this->getSize(); i++) {
        Vertex* vertex = this->vertexes[i];
        cout << "Vertice no " << i << "\n";

        if(vertex->edges.size() > 0) {
            cout << "\t";
        }

        for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
            Edge* edge = *it;
            cout << "(" << edge->getOrigin() << ", " << edge->getDestiny() << ") ";
        }
        cout << "\n";
    }
}