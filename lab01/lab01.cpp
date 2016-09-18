#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

typedef enum { WHITE, GREY, BLACK } Color;

class Central;
class Central {
    public:
        string origin;
        vector<Central*> destinations;

        bool visited;
        bool isPresentInTheOther;
        Central():isPresentInTheOther(false) {};
};

vector<Central*> buildGraph();
bool areFromSameCity(vector<Central*> oldBlueprint, vector<Central*> newBlueprint);

int main() {
    vector<Central*> oldBlueprint = buildGraph();
    vector<Central*> newBlueprint = buildGraph();

    bool yes = areFromSameCity(oldBlueprint, newBlueprint);
    if(yes) {
        cout << "SIM\n";
    } else {
        cout << "NAO\n";
    }

    return 0;
}

void remove(Central* font, Central* destiny, vector<Central*> oldBlueprint) {
    for (vector<Central*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Central* central = *it;
        if(central->origin.compare(font->origin) == 0 ||
            central->origin.compare(destiny->origin) == 0) {
            
            for (vector<Central*>::iterator it2 = central->destinations.begin(); it2 != central->destinations.end(); it2++) {
                Central* central2 = *it2;
                if(central2->origin.compare(font->origin) == 0 ||
                        central2->origin.compare(destiny->origin) == 0) {
                    central->destinations.erase(it2);
                }
            }
        }
    }
}

void depthSearchVisit(Central* font, Central* vertex, vector<Central*> oldBlueprint) {
    vertex->visited = true;

for (vector<Central*>::iterator it = vertex->destinations.begin(); it != vertex->destinations.end(); it++) {
        Central* adjacentVertex = *it;
        if(!adjacentVertex->visited) {
            if(adjacentVertex->isPresentInTheOther) {
                remove(font, vertex, oldBlueprint);
            } else {
                depthSearchVisit(font, adjacentVertex, oldBlueprint);
            }
        }
    }
}

bool checkVertexes(vector<Central*> oldBlueprint, vector<Central*> newBlueprint) {
    for (vector<Central*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Central* oldCentral = *it;
        bool found = false;
        for (vector<Central*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
            Central* newCentral = *it2;
            if(newCentral->origin.compare(oldCentral->origin) == 0) {
                newCentral->isPresentInTheOther = true;
                found = true;
                break;
            }
        }
        if(!found) {
            return false;
        }
    }
    return true;
}

bool areFromSameCity(vector<Central*> oldBlueprint, vector<Central*> newBlueprint) {
    checkVertexes(oldBlueprint, newBlueprint);

    for (vector<Central*>::iterator it = newBlueprint.begin(); it != newBlueprint.end(); it++) {
        Central* vertex = *it;

        for (vector<Central*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
                Central* v = *it2;
                v->visited = false;
        }
        if(vertex->isPresentInTheOther) {
            depthSearchVisit(vertex, vertex, oldBlueprint);
        }
    }

    for (vector<Central*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Central* vertex = *it;
        if(vertex->destinations.size() > 0) {
            return false;
        }
    }

    return true;
}

vector<Central*> buildGraph() {
    vector<Central*> graph;
    
    unsigned int m1;
    cin >> m1;
    for(unsigned int i = 0; i < m1; i++) {

        string vertex1, vertex2;
        cin >> vertex1 >> vertex2;

        Central *central1 = NULL, *central2 = NULL;
        for (vector<Central*>::iterator it = graph.begin(); it != graph.end(); it++) {
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