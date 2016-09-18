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
    for (unsigned int i = 0; i < oldBlueprint.size(); i++) {
        Central* central = oldBlueprint.at(i);
        if(central->origin.compare(font->origin) == 0 ||
            central->origin.compare(destiny->origin) == 0) {
            for (unsigned int j = 0; j < central->destinations.size(); j++){
                Central* central2 = central->destinations.at(j);
                if(central2->origin.compare(font->origin) == 0 ||
                        central2->origin.compare(destiny->origin) == 0) {
                    central->destinations.erase(central->destinations.begin() + j);
                }
            }
        }
    }
}

void depthSearchVisit(Central* font, Central* vertex, vector<Central*> oldBlueprint) {
    vertex->visited = true;

    for(unsigned int i = 0; i < vertex->destinations.size(); i++) {
        Central* adjacentVertex = vertex->destinations.at(i);
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
    for(unsigned int i = 0; i < oldBlueprint.size(); i++) {
        Central* oldCentral = oldBlueprint.at(i);
        bool found = false;
        for(unsigned int j = 0; j < newBlueprint.size(); j++) {
            Central* newCentral = oldBlueprint.at(i);
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

    for(unsigned int i = 0; i < newBlueprint.size(); i++) {
        Central* vertex = newBlueprint.at(i);

        for (unsigned int h = 0; h < newBlueprint.size(); h++) {
                newBlueprint.at(h)->visited = false;
        }
        if(vertex->isPresentInTheOther) {
            depthSearchVisit(vertex, vertex, oldBlueprint);
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
        for (unsigned int i = 0; i < graph.size(); i++) {
            Central* central = graph.at(i);

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