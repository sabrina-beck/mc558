#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <cstdlib>
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

class Element {
    public:
        virtual double getKey() = 0;
        virtual void setKey(double key) = 0;
};

class Vertex : public Element {
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
        virtual double getKey() {
            return this->key;
        }
        virtual void setKey(double key) {
            this->key = key;
        }
        double isOnHeap() {
            return this->onHeap;
        }
        void setOnHeap(bool onHeap) {
            this->onHeap = onHeap;
        }
        int getIndex() {
            return this->index;
        }
        void setIndex(int index) {
            this->index = index;
        }
    private:
        Coordinate* coordinate;
        Vertex* father;
        double key;
        bool onHeap;
        int index;
};

class Case {
    public:
        Case(int numberOfConnectionPoints, int twistedPairMaxDistance) {
            this->numberOfConnectionPoints = numberOfConnectionPoints;
            this->twistedPairMaxDistance = twistedPairMaxDistance;
            this->vertexes = (Vertex**) malloc(numberOfConnectionPoints * sizeof(Vertex*));
            this->currentIndex = 0;
        }
        ~Case() {
            free(this->vertexes);
        }
        int getNumberOfConnectionPoints() {
            return this->numberOfConnectionPoints;
        }
        int getTwistedPairMaxDistance() {
            return this->twistedPairMaxDistance;
        }
        void addVertex(Vertex* vertex) {
            this->vertexes[this->currentIndex++] = vertex;
        }
        Vertex** getVertexes() {
            return this->vertexes;
        }
    private:
        int numberOfConnectionPoints;
        int twistedPairMaxDistance;
        Vertex** vertexes;
        int currentIndex;
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

class MinHeap {
    public:
        MinHeap(Vertex** vertexes, int size);
        ~MinHeap();
        bool isEmpty();
        Vertex* extractMin();
        void decreaseKey(Vertex* vertex, double key);
    private:
        void minHeapify(int index);
        int father(int i);
        int right(int i);
        int left(int i);
        int size;
        Vertex** vertexes;
};

vector<Case*> readInput();
void printInput(vector<Case*> cases);
NetworkStructure minimumCableConfiguration(Case* networkCase);
void minimumSpawningTree(Case* graph, Vertex* root);

int main() {
    vector<Case*> cases = readInput();

    for (vector<Case*>::iterator it = cases.begin(); it != cases.end(); it++) {
        Case* currentCase = *it;
        NetworkStructure ns = minimumCableConfiguration(currentCase);
        cout << round(ns.getTwistedPairSize()) << " " 
                        << round(ns.getOpticalFiberSize()) << "\n";
    }

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

        Vertex** vertexes = currentCase->getVertexes();
        for (int i = 0; i < currentCase->getNumberOfConnectionPoints(); i++) {
            Vertex* vertex = vertexes[i];
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
    Vertex** vertexes = networkCase->getVertexes();
    Vertex* root = vertexes[0];
    minimumSpawningTree(networkCase, root);

    NetworkStructure structure;
    for (int i = 0; i < networkCase->getNumberOfConnectionPoints(); i++) {
        double edgeWeight = vertexes[i]->getKey();

        if(edgeWeight > networkCase->getTwistedPairMaxDistance()) {
            structure.addOpticalFiber(edgeWeight);
        } else {
            structure.addTwistedPair(edgeWeight);
        }
    }

    return structure;
}

void minimumSpawningTree(Case* networkCase, Vertex* root) {
    root->setKey(0);
    MinHeap heap(networkCase->getVertexes(), networkCase->getNumberOfConnectionPoints());
    while(!heap.isEmpty()) {
        Vertex* u = heap.extractMin();
        u->setOnHeap(false);
        
        Vertex** vertexes = networkCase->getVertexes();
        for (int i = 0; i < networkCase->getNumberOfConnectionPoints(); i++) {
            Vertex* vertex = vertexes[i];
            
            double edgeWeight = weight(u, vertex);
            if(vertex->isOnHeap() && edgeWeight < vertex->getKey()) {
                vertex->setFather(u);
                heap.decreaseKey(vertex, edgeWeight);
            }
        }
    }
}

void swap(Vertex** a, Vertex** b) {
    Vertex* tmp = *a;
    *a = *b;
    *b = tmp;
}

MinHeap::MinHeap(Vertex** vertexes, int size) {
    this->size = size;
    this->vertexes = (Vertex**) malloc(size * sizeof(Vertex*));

    for(int i = 0; i < size; i++) {
        this->vertexes[i] = vertexes[i];
        this->vertexes[i]->setIndex(i);
    }

    for (int i = this->size/2; i >= 0; i--) {
        this->minHeapify(i);
    }

}

MinHeap::~MinHeap() {
    free(this->vertexes);
}

int MinHeap::father(int i) {
    return (i + 1)/2 - 1;
}

int MinHeap::left(int i) {
    return 2*(i + 1) - 1;
}

int MinHeap::right(int i) {
    return 2*(i + 1);
}

void MinHeap::minHeapify(int i) {
    int left = this->left(i);
    int right = this->right(i);
    int lowest;

    if (left < this->size && this->vertexes[left]->getKey() < this->vertexes[i]->getKey()) {
        lowest = left;
    } else {
        lowest = i;
    }

    if (right < this->size && this->vertexes[right]->getKey() < this->vertexes[lowest]->getKey()) {
        lowest = right;
    }

    if (lowest != i) {
        swap(this->vertexes + i, this->vertexes + lowest);

        this->vertexes[i]->setIndex(i);
        this->vertexes[lowest]->setIndex(lowest);

        this->minHeapify(lowest);
    }
}

bool MinHeap::isEmpty() {
    return this->size == 0;
}

Vertex* MinHeap::extractMin() {
    if (this->size < 1) {
        return NULL; // heap underflow
    }

    Vertex* min = this->vertexes[0];

    this->vertexes[0] = this->vertexes[this->size - 1];
    this->vertexes[0]->setIndex(0);
    this->size = this->size - 1;
    
    this->minHeapify(0);

    return min;
}

void MinHeap::decreaseKey(Vertex* vertex, double key) {
    int i = vertex->getIndex();

    if (key > this->vertexes[i]->getKey()) {
        return; // error “nova chave maior que a atual”
    }

    this->vertexes[i]->setKey(key);
    while (i > 0 && this->vertexes[this->father(i)]->getKey() > this->vertexes[i]->getKey()) {
        int father = this->father(i);

        swap(this->vertexes[i], this->vertexes[father]);

        this->vertexes[i]->setIndex(i);
        this->vertexes[father]->setIndex(father);

        i = this->father(i);
    }

}