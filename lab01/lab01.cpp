#include <iostream>
#include <list>
#include <stdlib.h>
using namespace std;

typedef enum { WHITE, GREY, BLACK } Color;

class Central;
class Central {
    public:
        string origin;
        list<Central*> destinations;

        bool visited;
};

list<Central*> buildGraph();
bool areFromSameCity(list<Central*> oldBlueprint, list<Central*> newBlueprint);

int main() {
    list<Central*> oldBlueprint = buildGraph();
    list<Central*> newBlueprint = buildGraph();

    bool yes = areFromSameCity(oldBlueprint, newBlueprint);
    if(yes) {
        cout << "SIM\n";
    } else {
        cout << "NAO\n";
    }

    return 0;
}

Central* find(list<Central*> graph, string searched) {
    for (list<Central*>::iterator it = graph.begin(); it != graph.end();  it++) {
        Central* vertex = *it;
        if(searched.compare(vertex->origin) == 0) {
            return vertex;
        }
    }
    return NULL;
}

bool isThereAPathWithOnlyNewCentrals(Central* vertex, string destiny, 
                                                list<Central*> oldBlueprint) {
    vertex->visited = true;
    for (list<Central*>::iterator it = vertex->destinations.begin(); it != vertex->destinations.end();  it++) {
        Central* adjacentVertex = *it;
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

bool areFromSameCity(list<Central*> oldBlueprint, list<Central*> newBlueprint) {
    for (list<Central*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end();  it++) {
        Central* oldOrigin = *it;
        Central* newOrigin = find(newBlueprint, oldOrigin->origin);

        if(newOrigin == NULL) {
            return false;
        }

        for (list<Central*>::iterator it2 = oldOrigin->destinations.begin(); it2 != oldOrigin->destinations.end();  it2++) {
            Central* oldDestiny = *it2;

            for (list<Central*>::iterator it3 = newBlueprint.begin(); it3 != newBlueprint.end();  it3++) {
                Central* v = *it3;
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

list<Central*> buildGraph() {
    list<Central*> graph;
    
    unsigned int m1;
    cin >> m1;
    for(unsigned int i = 0; i < m1; i++) {

        string vertex1, vertex2;
        cin >> vertex1 >> vertex2;

        Central *central1 = NULL, *central2 = NULL;
        for (list<Central*>::iterator it = graph.begin(); it != graph.end();  it++) {
            Central* central = *it;

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
            central1 = new Central();
            central1->origin = vertex1;
            graph.push_back(central1);
        }

        if(central2 == NULL) {
            central2 = new Central();
            central2->origin = vertex2;
            graph.push_back(central2);
        }

        central1->destinations.push_back(central2);
        central2->destinations.push_back(central1);
    }

    return graph;
}