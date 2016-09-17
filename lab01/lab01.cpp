#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

class Central;
class Central {
    public:
        string origin;
        vector<Central*> destinations;
};
vector<Central*> buildGraph();
void printGraph(vector<Central*> graph);

int main() {
    vector<Central*> oldBlueprint = buildGraph();
    vector<Central*> newBlueprint = buildGraph();

    cout << "\nFINISH READING\n";
    cout << "\n------------------------\n";

    printGraph(oldBlueprint);
    cout << "\n------------------\n";
    printGraph(newBlueprint);
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