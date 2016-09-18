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
        bool isPresentInTheOther;
        Vertex():isPresentInTheOther(false) {};
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

void remove(Vertex* font, Vertex* destiny, list<Vertex*> oldBlueprint) {
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->origin.compare(font->origin) == 0 ||
            vertex->origin.compare(destiny->origin) == 0) {
            for (list<Vertex*>::iterator it2 = vertex->destinations.begin(); it2 != vertex->destinations.end();) {
                Vertex* vertex2 = *it2;
                if(vertex2->origin.compare(font->origin) == 0 ||
                        vertex2->origin.compare(destiny->origin) == 0) {
                    it2 = vertex->destinations.erase(it2);
                } else {
                    it2++;
                }
            }
        }
    }
}

void depthSearchVisit(Vertex* font, Vertex* vertex, list<Vertex*> oldBlueprint) {
    vertex->visited = true;

    for (list<Vertex*>::iterator it = vertex->destinations.begin(); it != vertex->destinations.end(); it++) {
        Vertex* adjacentVertex = *it;
        if(!adjacentVertex->visited) {
            if(adjacentVertex->isPresentInTheOther) {
                remove(font, adjacentVertex, oldBlueprint);
            } else {
                depthSearchVisit(font, adjacentVertex, oldBlueprint);
            }
        }
    }
}

bool checkVertexes(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* oldVertex = *it;
        bool found = false;
        for (list<Vertex*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
            Vertex* newVertex = *it2;

            if(newVertex->origin.compare(oldVertex->origin) == 0) {
                newVertex->isPresentInTheOther = true;
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

bool areFromSameCity(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    if(!checkVertexes(oldBlueprint, newBlueprint)) {
        return false;
    }

    for (list<Vertex*>::iterator it = newBlueprint.begin(); it != newBlueprint.end(); it++) {
        Vertex* vertex = *it;

        if(vertex->isPresentInTheOther) {
            for (list<Vertex*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
                    Vertex* v = *it2;
                    v->visited = false;
            }
            depthSearchVisit(vertex, vertex, oldBlueprint);
        }
    }

    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->destinations.size() > 0) {
            return false;
        }
    }

    return true;
}

list<Vertex*> buildGraph() {
    list<Vertex*> graph;
    
    unsigned int m1;
    cin >> m1;
    for(unsigned int i = 0; i < m1; i++) {

        string input1, input2;
        cin >> input1 >> input2;

        Vertex *vertex1 = NULL, *vertex2 = NULL;
        for (list<Vertex*>::iterator it = graph.begin(); it != graph.end(); it++) {
            Vertex* vertex = *it;

            if(vertex->origin.compare(input1) == 0) {
                vertex1 = vertex;
            }

            if(vertex->origin.compare(input2) == 0) {
                vertex2 = vertex;
            }

            if(vertex1 != NULL && vertex2 != NULL) {
                break;
            }
        }

        if(vertex1 == NULL) {
            vertex1 = new Vertex();
            vertex1->origin = input1;
            graph.push_back(vertex1);
        }

        if(vertex2 == NULL) {
            vertex2 = new Vertex();
            vertex2->origin = input2;
            graph.push_back(vertex2);
        }

        vertex1->destinations.push_back(vertex2);
        vertex2->destinations.push_back(vertex1);
    }

    return graph;
}