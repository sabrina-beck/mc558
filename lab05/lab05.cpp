#include <iostream>
#include <cstdlib>
#include <list>
#include <queue>
#include <climits>
using namespace std;

typedef enum Color {
    WHITE, BLUE, RED
} Color;

typedef struct Coordinate {
    int line;
    int column;
} Coordinate;

/*
 * Represents a graph vertex
 */
typedef struct Vertex {
    Coordinate coordinate;
    Color color;
    list<struct Edge*>* adjacencies;

    int distance;       // the distance of the vertex from the origin vertex, used in dijkstra
    Vertex* father;     // the vertex predecessor in the minimum path from the origin vertex, used in dijkstra
    bool visited;

    int index;          // the vertex index in the heap
} Vertex;

/*
 * Represents a graph edge
 */
typedef struct Edge {
    int weight;         // the edge weight
    int flux;
    bool inverted;
    Vertex* origin;     // the edge origin vertex
    Vertex* destiny;    // the edge destiny vertex
    struct Edge* original;
} Edge;

/*
 * Represents a Graph
 */
class Graph {
    public:
        /*
         * Add a new vertex to the graph
         */
        void addVertex(Vertex* vertex) {
            this->vertexes.push_back(vertex);
        }

        /*
         * Add a new edge to the graph that goes from originId to destinyId
         */
        void addEdge(Coordinate originCoord, Coordinate destinyCoord, int weight,
                     int flux, bool inverted, Edge* originalEdge) {
            Vertex* origin = NULL;
            Vertex* destiny = NULL;

            for(list<Vertex*>::iterator it = this->vertexes.begin(); it != this->vertexes.end(); it++) {
                Vertex* vertex = *it;
                if(vertex->coordinate.line == originCoord.line &&
                    vertex->coordinate.column == originCoord.column) {
                    origin = vertex;
                }

                if(vertex->coordinate.line == destinyCoord.line &&
                    vertex->coordinate.column == destinyCoord.column) {
                    destiny = vertex;
                }                
            }

            if(origin == NULL) {
                origin = new Vertex();
                origin->coordinate = originCoord;
                origin->distance = INT_MAX;
                origin->father = NULL;
                origin->adjacencies = new list<Edge*>();
                this->vertexes.push_back(origin);
            }

            if(destiny == NULL) {
                destiny = new Vertex();
                destiny->coordinate = destinyCoord;
                destiny->distance = INT_MAX;
                destiny->father = NULL;
                destiny->adjacencies = new list<Edge*>();
                this->vertexes.push_back(destiny);
            }

            Edge* edge = new Edge;
            edge->weight = weight;
            edge->flux = flux;
            edge->inverted = inverted;
            edge->original = originalEdge;
            edge->origin = origin;
            edge->destiny = destiny;

            // add the edge to its origin vertex adjacencies
            origin->adjacencies->push_back(edge);
        }

        int getNumberOfVertexes() {
            return this->vertexes.size();
        }

        list<Vertex*> getVertexes() {
            return this->vertexes;
        }

    private:
        list<Vertex*> vertexes;
};

typedef struct FluxNetwork {
    Graph* graph;
    Vertex* source;
    Vertex* terminal;
} FluxNetwork;

typedef struct Gallery {
    int** map;
    int lines;
    int columns;
} Gallery;

typedef struct Instancies {
    Gallery* galleries;
    int size;
} Instancies;

Instancies readGalleries();
void print(Instancies instancies);
int galleryProblem(Gallery gallery);


int main() {
    Instancies instancies = readGalleries();

    //for (int i = 0; i < instancies.size; i++) {
        Gallery gallery = instancies.galleries[0];
        int numberOfGuards = galleryProblem(gallery);
        cout << numberOfGuards << "\n";
    //}
    return 0;
}

Instancies readGalleries() {
    Instancies instancies;
    cin >> instancies.size;

    instancies.galleries = (Gallery*) malloc(instancies.size * sizeof(Gallery));

    for(int i = 0; i < instancies.size; i++) {
        int lines, columns;
        cin >> lines >> columns;

        instancies.galleries[i].lines = lines;
        instancies.galleries[i].columns = columns;
        
        int** gallery = (int**) malloc(lines * sizeof(int*));
        for(int line = 0; line < lines; line++) {
            gallery[line] = (int*) malloc(columns * sizeof(int));

            string galleryLine;
            cin >> galleryLine;
            
            for(int k = 0; k < columns; k++) {
                char sectorType = galleryLine[k];
                if(sectorType == '#') {
                    gallery[line][k] = 1;
                } else {
                    gallery[line][k] = 0;
                }
            }
        }
        instancies.galleries[i].map = gallery;
    }

    return instancies;
}

void print(Instancies instancies) {
    cout << instancies.size << "\n";
    for(int i = 0; i < instancies.size; i++) {
        Gallery gallery = instancies.galleries[i];
        cout << gallery.lines << " " << gallery.columns << "\n";

        for(int i = 0; i < gallery.lines; i++) {
            for(int k = 0; k < gallery.columns; k++) {
                if(gallery.map[i][k] == 1) {
                    cout << "#";
                } else if(gallery.map[i][k] == 0) {
                    cout << ".";
                }
            }
            cout << "\n";
        }
    }
}

Graph* createGraphFrom(Gallery gallery) {
    Graph* graph = new Graph();
    for(int i = 0; i < gallery.lines; i++) {
        for(int j = 0; j < gallery.columns; j++) {
            if(gallery.map[i][j] == 1) {
                Coordinate coord1;
                coord1.line = i;
                coord1.column = j;

                if(i+1 < gallery.lines && gallery.map[i+1][j] == 1) {
                    Coordinate coord2;
                    coord2.line = i + 1;
                    coord2.column = j;

                    graph->addEdge(coord1, coord2, 1, 0, false, NULL);
                    graph->addEdge(coord2, coord1, 1, 0, false, NULL);
                }
                if(i-1 >= 0 && gallery.map[i-1][j] == 1) {
                    Coordinate coord2;
                    coord2.line = i - 1;
                    coord2.column = j;

                    graph->addEdge(coord1, coord2, 1, 0, false, NULL);
                    graph->addEdge(coord2, coord1, 1, 0, false, NULL);
                }
                if(j+1 < gallery.columns && gallery.map[i][j+1] == 1) {
                    Coordinate coord2;
                    coord2.line = i;
                    coord2.column = j + 1;

                    graph->addEdge(coord1, coord2, 1, 0, false, NULL);
                    graph->addEdge(coord2, coord1, 1, 0, false, NULL);
                }
                if(j-1 >= 0 && gallery.map[i][j-1] == 1) {
                    Coordinate coord2;
                    coord2.line = i;
                    coord2.column = j - 1;

                    graph->addEdge(coord1, coord2, 1, 0, false, NULL);
                    graph->addEdge(coord2, coord1, 1, 0, false, NULL);
                }

                gallery.map[i][j] = 0;
            }
        }
    }
    return graph;
}

void remove(Vertex* origin, Vertex* destiny) {
    list<Edge*>* edges = origin->adjacencies;
    for(list<Edge*>::iterator it = edges->begin(); it != edges->end();) {
        Edge* oppositeEdge = *it;
        if(oppositeEdge->destiny == destiny) {
            it = edges->erase(it);
        } else {
            it++;
        }
    }
}

void partitionGraph(Graph* graph, Vertex* source) {
    source->color = BLUE;

    queue<Vertex*> queue;
    queue.push(source);

    while(!queue.empty()) {
        Vertex* u = queue.front();
        queue.pop();

        for(list<Edge*>::iterator it = u->adjacencies->begin(); it != u->adjacencies->end(); it++) {
            Edge* edge = *it;
            Vertex* v = edge->destiny;
            if(v->color == WHITE) {
                if(u->color == BLUE) {
                    v->color = RED;
                    //remove (v,u) edge
                    remove(v, u);
                } else {
                    v->color = BLUE;
                    //remove (u, v) edge
                    it = u->adjacencies->erase(it);
                }
                queue.push(v);
            } else if(v->color == u->color) {
                cout << "We have a problem, the graph is not bipartite.\n";
                exit(1);
            }
        }
    }
}


void partitionGraph(Graph* graph) {
    list<Vertex*> vertexes = graph->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        vertex->color = WHITE;
    }

    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->color == WHITE) {
            partitionGraph(graph, vertex);
        }
    }
}

FluxNetwork createFluxNetwork(Graph* graph) {
    Vertex* source = new Vertex();
    source->coordinate.line = -1;
    source->coordinate.column = -1;
    source->adjacencies = new list<Edge*>();

    Vertex* terminal = new Vertex();
    terminal->coordinate.line = -1;
    terminal->coordinate.column = -1;
    terminal->adjacencies = new list<Edge*>();

    list<Vertex*> vertexes = graph->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->color == BLUE) {
            Edge* edge = new Edge();
            edge->weight = 1;
            edge->origin = source;
            edge->destiny = vertex;

            source->adjacencies->push_back(edge);
        } else {
            Edge* edge = new Edge();
            edge->weight = 1;
            edge->origin = vertex;
            edge->destiny = terminal;

            terminal->adjacencies->push_back(edge);
        }
    }

    graph->addVertex(source);
    graph->addVertex(terminal);

    FluxNetwork fluxNetwork;
    fluxNetwork.graph = graph;
    fluxNetwork.source = source;
    fluxNetwork.terminal = terminal;
    return fluxNetwork;
}

FluxNetwork buildResidualNetwork(FluxNetwork fluxNetwork) {
    Graph* residual = new Graph();
 
    list<Vertex*> vertexes = fluxNetwork.graph->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;

        if(vertex == fluxNetwork.source || vertex == fluxNetwork.terminal) {
            residual->addVertex(vertex);
        }

        for(list<Edge*>::iterator it2 = vertex->adjacencies->begin(); it2 != vertex->adjacencies->end(); it2++) {
            Edge* originalEdge = *it2;

            int capacity = originalEdge->weight;
            int flux = originalEdge->flux;

            if(flux < capacity) {
                int weight = capacity - flux;
                int newFlux = 0;
                bool inverted = false;
                Vertex* origin = vertex;
                Vertex* destiny = originalEdge->destiny;

                residual->addEdge(origin->coordinate, destiny->coordinate,
                    weight, newFlux, inverted, originalEdge);
            }
 
            if(flux > 0) {
                int weight = flux;
                int newFlux = 0;
                bool inverted = true;
                Vertex* origin = originalEdge->destiny;
                Vertex* destiny = vertex;

                residual->addEdge(origin->coordinate, destiny->coordinate,
                    weight, newFlux, inverted, originalEdge);
            }
        }
    }
 
    FluxNetwork residualNetwork;
    residualNetwork.graph = residual;
    residualNetwork.source = fluxNetwork.source;
    residualNetwork.terminal = fluxNetwork.terminal;
    return residualNetwork;
}

bool hasPathBFS(Graph* residual, Vertex* source, Vertex* terminal) {
    list<Vertex*> vertexes = residual->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        vertex->visited = false;
        vertex->father = NULL;
    }

    source->visited = true;
    source->distance = 0;
    source->father = NULL;

    queue<Vertex*> queue;
    queue.push(source);

    while(!queue.empty()) {
        Vertex* vertex = queue.front();
        queue.pop();

        for(list<Edge*>::iterator it = vertex->adjacencies->begin(); it != vertex->adjacencies->end(); it++) {
            Edge* edge = *it;
            if(!edge->destiny->visited) {
                edge->destiny->visited = true;
                edge->destiny->father = vertex;
                queue.push(edge->destiny);
            }
        }
    }

    if(terminal->father != NULL) {
        list<Edge*> path;
        int cf = INT_MAX; //cf(p) = min{cf(u, v) : (u, v) está em p}
        Vertex* current = terminal;
        while(current != NULL) {
            Vertex* father = current->father;
            for(list<Edge*>::iterator it = father->adjacencies->begin(); it != father->adjacencies->end(); it++) {
                Edge* edge = *it;
                if(edge->destiny == current) {
                    path.push_back(edge);
                    if(edge->weight < cf) {
                        cf = edge->weight;
                    }
                    break;
                }
            }
            current = father;
        }
        
        for(list<Edge*>::iterator it = path.begin(); it != path.end(); it++) {
            Edge* edge = *it;
            if(!edge->inverted) {
                edge->original->flux = edge->original->flux + cf;
            } else {
                edge->original->flux = edge->original->flux - cf;
            }
        }

        return true;
    }

    return false;
}

int fordFulkersonMaxFlow(FluxNetwork fluxNetwork) {
    list<Vertex*> vertexes = fluxNetwork.graph->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        for(list<Edge*>::iterator it2 = vertex->adjacencies->begin(); it2 != vertex->adjacencies->end(); it2++) {
            Edge* edge = *it2;
            edge->flux = 0;
        }
    }

    FluxNetwork residualNetwork = buildResidualNetwork(fluxNetwork);
    while(hasPathBFS(residualNetwork.graph, residualNetwork.source, residualNetwork.terminal)) {
        residualNetwork = buildResidualNetwork(fluxNetwork);
    }

    Vertex* source = fluxNetwork.source;
    int maxFlow = 0;
    for(list<Edge*>::iterator it = source->adjacencies->begin(); it != source->adjacencies->end(); it++) {    
        Edge* edge = *it;
        maxFlow += edge->flux;
    }

    return maxFlow;
}

void print(Coordinate coord) {
    cout << "(" << coord.line << ", " << coord.column << ")";
}

void print(Graph* graph) {
    list<Vertex*> vertexes = graph->getVertexes();
    for(list<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        for(list<Edge*>::iterator it2 = vertex->adjacencies->begin(); it2 != vertex->adjacencies->end(); it2++) {
            Edge* edge = *it2;
            print(edge->origin->coordinate);
            cout << " <> ";
            print(edge->destiny->coordinate);
            cout << "\n";
        }
    }
}

int galleryProblem(Gallery gallery) {
    Graph* graph = createGraphFrom(gallery);
    partitionGraph(graph);
    FluxNetwork fluxNetwork = createFluxNetwork(graph);
    return fordFulkersonMaxFlow(fluxNetwork);
}
