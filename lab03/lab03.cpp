#include <iostream>
#include <vector>
#include <cmath>
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
        }
        Coordinate* getCoordinate() {
            return this->coordinate;
        }
    private:
        Coordinate* coordinate;
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
            this->twistedPairSize = 0;
            this->opticalFiberSize = 0;
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

int main() {
    vector<Case*> cases = readInput();

    //NetworkStructure minimum-spanning-tree(Case* networkCase);
    //for (vector<Case*>::iterator it = cases.begin(); it != cases.end(); it++) {
    //    Case* currentCase = *it;
    //    NetworkStructure ns = minimum-spanning-tree(currentCase);
    //}

    printInput(cases);

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