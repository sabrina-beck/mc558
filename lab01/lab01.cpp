#include <iostream>
#include <list>
#include <stdlib.h>
using namespace std;

typedef enum { WHITE, GREY, BLACK } Color;

class Vertex;
class Vertex {
    public:
        string origin;
        list<Vertex*> destinations;

        bool visited;
};

list<Vertex*> buildGraph();
bool areFromSameCity(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint);

int main() {
    list<Vertex*> oldBlueprint = buildGraph();
    list<Vertex*> newBlueprint = buildGraph();

    bool yes = areFromSameCity(oldBlueprint, newBlueprint);
    if(yes) {
        cout << "SIM\n";
    } else {
        cout << "NAO\n";
    }

    return 0;
}

Vertex* find(list<Vertex*> graph, string searched) {
    for (list<Vertex*>::iterator it = graph.begin(); it != graph.end();  it++) {
        Vertex* vertex = *it;
        if(searched.compare(vertex->origin) == 0) {
            return vertex;
        }
    }
    return NULL;
}

bool isThereAPathWithOnlyNewCentrals(Vertex* vertex, string destiny, 
                                                list<Vertex*> oldBlueprint) {
    vertex->visited = true;
    for (list<Vertex*>::iterator it = vertex->destinations.begin(); it != vertex->destinations.end();  it++) {
        Vertex* adjacentVertex = *it;
        if(!adjacentVertex->visited) {
            
            if(adjacentVertex->origin.compare(destiny) == 0) {
                return true;
            }

            if(find(oldBlueprint, adjacentVertex->origin)) {
                continue;
            }

            bool result = isThereAPathWithOnlyNewCentrals(adjacentVertex, destiny, oldBlueprint);
            if(result) {
                return true;
            }
        }
    }
    return false;
}

bool areFromSameCity(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end();  it++) {
        Vertex* oldOrigin = *it;
        Vertex* newOrigin = find(newBlueprint, oldOrigin->origin);

        if(newOrigin == NULL) {
            return false;
        }

        for (list<Vertex*>::iterator it2 = oldOrigin->destinations.begin(); it2 != oldOrigin->destinations.end();  it2++) {
            Vertex* oldDestiny = *it2;

            for (list<Vertex*>::iterator it3 = newBlueprint.begin(); it3 != newBlueprint.end();  it3++) {
                Vertex* v = *it3;
                v->visited = false;
            }
            bool isCorrectPathThere = isThereAPathWithOnlyNewCentrals(newOrigin, 
                                        oldDestiny->origin, oldBlueprint);
            if(!isCorrectPathThere) {
                return false;
            }
        }
    }
    return true;
}

list<Vertex*> buildGraph() {
    list<Vertex*> graph;
    
    unsigned int m1;
    cin >> m1;
    for(unsigned int i = 0; i < m1; i++) {

        string vertex1, vertex2;
        cin >> vertex1 >> vertex2;

        Vertex *central1 = NULL, *central2 = NULL;
        for (list<Vertex*>::iterator it = graph.begin(); it != graph.end();  it++) {
            Vertex* central = *it;

            if(central->origin.compare(vertex1) == 0) {
                central1 = central;
            }

            if(central->origin.compare(vertex2) == 0) {
                central2 = central;
            }

            if(central1 != NULL && central2 != NULL) {
                break;
            }
        }

        if(central1 == NULL) {
            central1 = new Vertex();
            central1->origin = vertex1;
            graph.push_back(central1);
        }

        if(central2 == NULL) {
            central2 = new Vertex();
            central2->origin = vertex2;
            graph.push_back(central2);
        }

        central1->destinations.push_back(central2);
        central2->destinations.push_back(central1);
    }

    return graph;
}