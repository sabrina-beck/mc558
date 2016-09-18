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
};

vector<Central*> buildGraph();
void printGraph(vector<Central*> graph);
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

void printVector(vector<Central*> dest) {
    cout << "      ";
    for (int i = 0; i < dest.size(); i++) {
        Central* d = dest.at(i);
        cout << d->origin << "  |  ";
    }
    cout << "\n";
}

void printGraph(vector<Central*> graph) {
    for (int i = 0; i < graph.size(); i++) {
        Central* c = graph.at(i);
        cout << c->origin << "\n";
        cout << "      childs: " << c->destinations.size() << "\n";
        printVector(c->destinations);
    }    
}

Central* find(vector<Central*> graph, string searched) {
    for (int k = 0; k < graph.size(); k++) {
        Central* vertex = graph.at(k);
        if(searched.compare(vertex->origin) == 0) {
            return vertex;
        }
    }
    return NULL;
}

bool isThereAPathWithOnlyNewCentrals(Central* vertex, string destiny, 
                                                vector<Central*> oldBlueprint) {
    vertex->visited = true;
    for(int i = 0; i < vertex->destinations.size(); i++) {
        Central* adjacentVertex = vertex->destinations.at(i);
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

bool areFromSameCity(vector<Central*> oldBlueprint, vector<Central*> newBlueprint) {
    for(int i = 0; i < oldBlueprint.size(); i++) {
        Central* oldOrigin = oldBlueprint.at(i);
        Central* newOrigin = find(newBlueprint, oldOrigin->origin);

        if(newOrigin == NULL) {
            return false;
        }

        for(int j = 0; j < oldOrigin->destinations.size(); j++) {
            Central* oldDestiny = oldOrigin->destinations.at(j);

            for (int h = 0; h < newBlueprint.size(); h++)
            {
                newBlueprint.at(h)->visited = false;
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

vector<Central*> buildGraph() {
    vector<Central*> graph;
    
    int m1;
    cin >> m1;
    for(int i = 0; i < m1; i++) {

        string vertex1, vertex2;
        cin >> vertex1 >> vertex2;

        Central *central1 = NULL, *central2 = NULL;
        for (int i = 0; i < graph.size(); i++) {
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