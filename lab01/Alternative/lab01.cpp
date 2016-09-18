#include <iostream>
#include <list>
#include <stdlib.h>
using namespace std;

typedef enum { WHITE, GREY, BLACK } Color;

class Vertex;
class Vertex {
    public:
        string name;
        list<Vertex*> adjacents;

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

Vertex* find(list<Vertex*> graph, string searched) {
    for (list<Vertex*>::iterator it = graph.begin(); it != graph.end();  it++) {
        Vertex* vertex = *it;
        if(searched.compare(vertex->name) == 0) {
            return vertex;
        }
    }
    return NULL;
}

bool isThereAPathWithOnlyNewVertexes(Vertex* vertex, string destiny, 
                                                list<Vertex*> oldBlueprint) {
    vertex->visited = true;
    for (list<Vertex*>::iterator it = vertex->adjacents.begin(); it != vertex->adjacents.end();  it++) {
        Vertex* adjacentVertex = *it;
        if(!adjacentVertex->visited) {
            if(adjacentVertex->name.compare(destiny) == 0) {
                return true;
            }

            if(adjacentVertex->isPresentInTheOther) {
                continue;
            }

            bool result = isThereAPathWithOnlyNewVertexes(adjacentVertex, destiny, oldBlueprint);
            if(result) {
                return true;
            }
        }
    }
    return false;
}

bool checkVertexes(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* oldVertex = *it;
        bool found = false;
        for (list<Vertex*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
            Vertex* newVertex = *it2;
            if(newVertex->name.compare(oldVertex->name) == 0) {
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
    checkVertexes(oldBlueprint, newBlueprint);

    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end();  it++) {
        Vertex* oldVertexFont = *it;
        Vertex* newVertexFont = find(newBlueprint, oldVertexFont->name);

        if(newVertexFont == NULL) {
            return false;
        }

        for (list<Vertex*>::iterator it2 = oldVertexFont->adjacents.begin(); it2 != oldVertexFont->adjacents.end();  it2++) {
            Vertex* oldVertexDestiny = *it2;

            for (list<Vertex*>::iterator it3 = newBlueprint.begin(); it3 != newBlueprint.end();  it3++) {
                Vertex* v = *it3;
                v->visited = false;
            }
            bool isCorrectPathThere = isThereAPathWithOnlyNewVertexes(newVertexFont, 
                                        oldVertexDestiny->name, oldBlueprint);
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

        Vertex *vertexFont = NULL, *vertexDestiny = NULL;
        for (list<Vertex*>::iterator it = graph.begin(); it != graph.end();  it++) {
            Vertex* vertex = *it;

            if(vertex->name.compare(vertex1) == 0) {
                vertexFont = vertex;
            }

            if(vertex->name.compare(vertex2) == 0) {
                vertexDestiny = vertex;
            }

            if(vertexFont != NULL && vertexDestiny != NULL) {
                break;
            }
        }

        if(vertexFont == NULL) {
            vertexFont = new Vertex();
            vertexFont->name = vertex1;
            graph.push_back(vertexFont);
        }

        if(vertexDestiny == NULL) {
            vertexDestiny = new Vertex();
            vertexDestiny->name = vertex2;
            graph.push_back(vertexDestiny);
        }

        vertexFont->adjacents.push_back(vertexDestiny);
        vertexDestiny->adjacents.push_back(vertexFont);
    }

    return graph;
}