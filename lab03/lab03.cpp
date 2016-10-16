#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
using namespace std;

class Coordinate {
    public:
        Coordinate(int x, int y) {
            this->x = x;
            this->y = y;
        }
        int getX() {
            return this->x;
        }
        int getY() {
            return this->y;
        }
    private:
        int x;
        int y;
};

class Vertex {
    public:
        Vertex(Coordinate* coordinate) {
            this->coordinate = coordinate;
            this->father = NULL;
            this->key = DBL_MAX;
            this->onHeap = true;
        }
        Coordinate* getCoordinate() {
            return this->coordinate;
        }
        Vertex* getFather() {
            return this->father;
        }
        void setFather(Vertex* newFather) {
            this->father = newFather;
        }
        double getKey() {
            return this->key;
        }
        void setKey(double key) {
            this->key = key;
        }
        double isOnHeap() {
            return this->onHeap;
        }
        void setOnHeap(bool onHeap) {
            this->onHeap = onHeap;
        }
    private:
        Coordinate* coordinate;
        Vertex* father;
        double key;
        bool onHeap;
};

class Case {
    public:
        Case(int numberOfConnectionPoints, int twistedPairMaxDistance) {
            this->numberOfConnectionPoints = numberOfConnectionPoints;
            this->twistedPairMaxDistance = twistedPairMaxDistance;
        }
        int getNumberOfConnectionPoints() {
            return this->numberOfConnectionPoints;
        }
        int getTwistedPairMaxDistance() {
            return this->twistedPairMaxDistance;
        }
        void addVertex(Vertex* vertex) {
            this->vertexes.push_back(vertex);
        }
        vector<Vertex*> getVertexes() {
            return this->vertexes;
        }
    private:
        int numberOfConnectionPoints;
        int twistedPairMaxDistance;
        vector<Vertex*> vertexes;
};

class NetworkStructure {
    public:
        NetworkStructure() {
            this->twistedPairSize = 0.0;
            this->opticalFiberSize = 0.0;
        }
        void addTwistedPair(double twistedPairSize) {
            this->twistedPairSize += twistedPairSize;
        }
        double getTwistedPairSize() {
            return this->twistedPairSize;
        }
        void addOpticalFiber(double opticalFiberSize) {
            this->opticalFiberSize += opticalFiberSize;
        }
        double getOpticalFiberSize() {
            return this->opticalFiberSize;
        }
    private:
        double twistedPairSize;
        double opticalFiberSize;
};

vector<Case*> readInput();
void printInput(vector<Case*> cases);
NetworkStructure minimumCableConfiguration(Case* networkCase);

int main() {
    vector<Case*> cases = readInput();

    for (vector<Case*>::iterator it = cases.begin(); it != cases.end(); it++) {
        Case* currentCase = *it;
        NetworkStructure ns = minimumCableConfiguration(currentCase);
        cout << round(ns.getTwistedPairSize()) << " " 
                        << round(ns.getOpticalFiberSize()) << "\n";
    }

    //printInput(cases);

    return 0;
}

vector<Case*> readInput() {
    int numberOfCases;
    
    vector<Case*> cases;

    cin >> numberOfCases;

    for(int i = 0; i < numberOfCases; i++) {
        int numberOfConnectionPoints, twistedPairMaxDistance;

        cin >> numberOfConnectionPoints >> twistedPairMaxDistance;

        Case* currentCase = new Case(numberOfConnectionPoints, twistedPairMaxDistance);

        for(int j = 0; j < numberOfConnectionPoints; j++) {
            int x, y;
            cin >> x >> y;

            Coordinate* coordinate = new Coordinate(x, y);
            Vertex* vertex = new Vertex(coordinate);
            currentCase->addVertex(vertex);
        }

        cases.push_back(currentCase);
    }

    return cases;
}

void printInput(vector<Case*> cases) {
    cout << cases.size() << "\n";
    for (vector<Case*>::iterator it = cases.begin(); it != cases.end(); it++) {
        Case* currentCase = *it;
        cout << currentCase->getNumberOfConnectionPoints() << " " 
                            << currentCase->getTwistedPairMaxDistance() << "\n";

        vector<Vertex*> vertexes = currentCase->getVertexes();
        for (vector<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
            Vertex* vertex = *it;
            Coordinate* coordinate = vertex->getCoordinate();
            cout << coordinate->getX() << " "
                            << coordinate->getY() << "\n";
        }

    }
}

double weight(Vertex* u, Vertex* v) {
    Coordinate* coordinateU = u->getCoordinate();
    Coordinate* coordinateV = v->getCoordinate();

    int cat1 = abs(coordinateU->getX() - coordinateV->getX());
    int cat2 = abs(coordinateU->getY() - coordinateV->getY());

    return sqrt(cat1*cat1 + cat2*cat2);
}

NetworkStructure minimumCableConfiguration(Case* networkCase) {
    vector<Vertex*> vertexes = networkCase->getVertexes();
    //Vertex* root = vertexes.at(0);
    //minimumSpawningTree(graph, root);

    NetworkStructure structure;
    for (vector<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
        Vertex* vertex = *it;
        if(vertex->getFather() != NULL) {
            double edgeWeight = vertex->getKey();
            if(edgeWeight > networkCase->getTwistedPairMaxDistance()) {
                structure.addOpticalFiber(edgeWeight);
            } else {
                structure.addTwistedPair(edgeWeight);
            }
        }
    }

    return structure;
}

//void minimumSpawningTree(Case* graph, Vertex* root) {
//    root->setKey(0);
//    Heap heap = new FibonacciHeap(graph->getVertexes());
//    while(!heap.isEmpty()) {
//        Vertex* u = heap.extractMin();
//        u->setOnHeap(false);
//
//        for (vector<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++) {
//            Vertex* vertex = *it;
//
//            double edgeWeight = weight(u, vertex);
//            if(vertex != u && vertex->isOnHeap() && edgeWeight < vertex->getKey()) {
//                vertex->setFather(u);
//                vertex->setKey(edgeWeight);
//                heap.reorganize(vertex);
//            }
//        }
//    }
//}

