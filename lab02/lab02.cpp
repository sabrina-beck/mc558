/*
 * Name: Sabrina Beck Angelini
 * RA: 157240
 */

#include <iostream>
#include <vector>
#include <list>
using namespace std;

typedef enum { Green, Yellow, Red } Color;

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
    Graph* graph = readGraph();

    int pathsCount = graph->countPossiblePaths();

    cout << pathsCount << "\n";

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

int Graph::countPossiblePaths() {
    Vertex* originVertex = this->vertexes[this->origin];
    return this->countPossiblePaths(Green, originVertex);
}

int Graph::countPossiblePaths(Color previousEdgeColor, Vertex* vertex) {
    if(vertex == this->vertexes[this->destiny]) {
        return 1;
    }

    int count = 0;
    for (list<Edge*>::iterator it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        Edge* edge = *it;

        if(this->canFollowThisPath(previousEdgeColor, edge)) {
            Vertex* destinyVertex = this->vertexes[edge->getDestiny()];
            count = count + this->countPossiblePaths(edge->getColor(), destinyVertex);
        }
    }
    return count;
}

bool Graph::canFollowThisPath(Color previousEdgeColor, Edge* currentEdge) {
    if(previousEdgeColor == Yellow) {
        return currentEdge->getColor() != Red;
    }

    if(previousEdgeColor == Red) {
        return currentEdge->getColor() == Green;
    }

    return true;
}