/*
 * Name: Sabrina Beck Angelini
 * RA: 157240
 */

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

        // indicate if the vertex was already visited during the depth search
        bool visited;
        // indicate if the vertex is present in the dead file
        bool isPresentInTheOther;
        Vertex():isPresentInTheOther(false) {};
};

list<Vertex*> buildGraph();
bool areFromSameCity(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint);

int main() {
    // builds the graph of the dead file
    list<Vertex*> oldBlueprint = buildGraph();
    // builds the graph of the damaged blueprint
    list<Vertex*> newBlueprint = buildGraph();

    // check if both graphs are from the same city
    bool sameCity = areFromSameCity(oldBlueprint, newBlueprint);
    if(sameCity) {
        cout << "SIM\n";
    } else {
        cout << "NAO\n";
    }

    return 0;
}

/*
 * Removes the edge (font, destiny) from the given graph,
 * considerates that the graph is not oriented
 */
void remove(Vertex* font, Vertex* destiny, list<Vertex*> graph) {
    // for each vertex in the graph
    for (list<Vertex*>::iterator it = graph.begin(); it != graph.end(); it++) {
        Vertex* vertex = *it;
        // searchs for the font vertex and the destiny vertex
        if(vertex->name.compare(font->name) == 0 ||
            vertex->name.compare(destiny->name) == 0) {
            // search the edge in the adjacency list of the font and the destiny
            for (list<Vertex*>::iterator it2 = vertex->adjacents.begin(); it2 != vertex->adjacents.end();) {
                Vertex* vertex2 = *it2;
                // when the edge is found
                if(vertex2->name.compare(font->name) == 0 ||
                        vertex2->name.compare(destiny->name) == 0) {
                    // removes it
                    it2 = vertex->adjacents.erase(it2);
                } else {
                    it2++;
                }
            }
        }
    }
}

/*
 * Modified depth search. Recursive function that makes a depth search
 * beginning in a determined font vertex. Searchs for paths between the font vertex
 * and others vertexes that belongs to the dead file. When a path with these requirements
 * is found, removes the equivalent edge from the dead file
 */
void depthSearchVisit(Vertex* font, Vertex* vertex, list<Vertex*> oldBlueprint) {
    // mark the vertex as visited, avoiding infinite loops when there are cycles
    vertex->visited = true;

    // check each adjacent vertex
    for (list<Vertex*>::iterator it = vertex->adjacents.begin(); it != vertex->adjacents.end(); it++) {
        Vertex* adjacentVertex = *it;
        // if the vertex wasn't visited before
        if(!adjacentVertex->visited) {
            // if the discovered vertex is present in the dead file,
            // then we found a path composed of only new vertexes between
            // the font vertex and the current vertex
            if(adjacentVertex->isPresentInTheOther) {
                // we remove the correpondent edge in the dead file
                remove(font, adjacentVertex, oldBlueprint);
            } else {
                // otherwise, we keep searching for a vertex in the dead file present
                // in this subgraph
                depthSearchVisit(font, adjacentVertex, oldBlueprint);
            }
        }
    }
}

/*
 * Check if the vertexes in the dead file are present in the damaged blueprint,
 * it also set a flag to true in the damaged bluprint graph's vertexes that are present
 * in the dead file
 */
bool checkVertexes(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    // for each vertex in the dead file
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* oldVertex = *it;
        bool found = false;
        // we check if there is a correspondent one in the damaged bluepriny
        for (list<Vertex*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
            Vertex* newVertex = *it2;

            // if the vertex is found, we set the flag to true, indicating that
            // this vertex is in the dead file
            if(newVertex->name.compare(oldVertex->name) == 0) {
                newVertex->isPresentInTheOther = true;
                found = true;
                break;
            }
        }
        // if the vertex was not found, then the blueprints are not from the samen city
        if(!found) {
            return false;
        }
    }
    return true;
}

/*
 * Check if the dead file (old blueprint) is from the same city of
 * the damaged blueprint (newBlueprint)
 */
bool areFromSameCity(list<Vertex*> oldBlueprint, list<Vertex*> newBlueprint) {
    // check if all the vertexes in the dead file are present in the damaged blueprint
    if(!checkVertexes(oldBlueprint, newBlueprint)) {
        return false;
    }

    // for each vertex in the damaged blueprint
    for (list<Vertex*>::iterator it = newBlueprint.begin(); it != newBlueprint.end(); it++) {
        Vertex* vertex = *it;

        // we proccess only the ones that are present in the dead file
        if(vertex->isPresentInTheOther) {
            // we must reset the visited flag before every depth search,
            // so the search will occur correcty
            for (list<Vertex*>::iterator it2 = newBlueprint.begin(); it2 != newBlueprint.end(); it2++) {
                    Vertex* v = *it2;
                    v->visited = false;
            }
            // makes the depth search, looking for paths between vertexes present
            // in the dead file and remove the correspodent edges from the dead file
            depthSearchVisit(vertex, vertex, oldBlueprint);
        }
    }

    // In the end the dead file must have all its edges removed,
    // if that happens, the two blueprints are from the city 
    for (list<Vertex*>::iterator it = oldBlueprint.begin(); it != oldBlueprint.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->adjacents.size() > 0) {
            return false;
        }
    }

    return true;
}

/*
 * Reads the input graph and build a graph represented by adjacency lists,
 * considerating that the graph is not oriented
 */
list<Vertex*> buildGraph() {
    list<Vertex*> graph;
    
    unsigned int numberOfEdges;
    // reads the number of edges
    cin >> numberOfEdges;

    // reads each edge
    for(unsigned int i = 0; i < numberOfEdges; i++) {

        string input1, input2;
        // each input line has two vertex
        cin >> input1 >> input2;

        Vertex *vertex1 = NULL, *vertex2 = NULL;
        
        // search for these vertexes in the graph, to check if they were
        // already read in a previous input
        for (list<Vertex*>::iterator it = graph.begin(); it != graph.end(); it++) {
            Vertex* vertex = *it;

            // check if input 1 vertex is present in the graph
            if(vertex->name.compare(input1) == 0) {
                vertex1 = vertex;
            }

            // check if input 2 vertex is present in the graph
            if(vertex->name.compare(input2) == 0) {
                vertex2 = vertex;
            }

            // if both vertexes were found, we don't have to search anymore
            if(vertex1 != NULL && vertex2 != NULL) {
                break;
            }
        }

        // if the vertex 1 was not found, we create it
        if(vertex1 == NULL) {
            vertex1 = new Vertex();
            vertex1->name = input1;
            // insert the recent created vertex on the graph
            graph.push_back(vertex1);
        }

        // if the vertex 2 was not found, we create it
        if(vertex2 == NULL) {
            vertex2 = new Vertex();
            vertex2->name = input2;
            // insert the recent created vertex on the graph
            graph.push_back(vertex2);
        }

        // inserts the read edge, considerating that the graph is not oriented
        // insert the edge (vertex1, vertex2)
        vertex1->adjacents.push_back(vertex2);
        // insert the edge (vertex2, vertex1)
        vertex2->adjacents.push_back(vertex1);
    }

    return graph;
}