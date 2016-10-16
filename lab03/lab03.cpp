#include <iostream>
#include <list>
#include <cmath>
#include <cfloat>
#include <cstdlib>
using namespace std;

/*
 * Represents a pair (x, y), a point in the plane
 */
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

/*
 * Represents a vertex, in this problem is a point of connection
 */
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
        int getIndex() {
            return this->index;
        }
        void setIndex(int index) {
            this->index = index;
        }
    private:
        Coordinate* coordinate; // the vertex position in the plane
        Vertex* father;         // the vertex father in the minimum spanning tree
        double key;             // the vertex key, the minimum weight of an edge that connects this vertex to the minimum spanning tree 
        bool onHeap;            // indicates if this vertex is on the heap, used to calculate the minimum spanning tree
        int index;              // the current index of the vertex on the heap, used to calculate the minimum spanning tree
};

/*
 * Represents a case of connection points that we must calculate the minimum cable configuration
 * to connect all the connection points. It has a the connection points and the maximum length of
 * twisted pair cable. 
 */
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
        int currentIndex; // current index, used to add new vertex to this case
};

/*
 * Represents a network structure indicating the amount of twisted pair cabel and optical fiber
 * to connect all the connection points given for a case
 */
class NetworkStructure {
    public:
        NetworkStructure() {
            this->twistedPairSize = 0.0;
            this->opticalFiberSize = 0.0;
        }
        /*
         * Add an amount of twisted pair cable to this configuration
         */
        void addTwistedPair(double twistedPairSize) {
            this->twistedPairSize += twistedPairSize;
        }
        double getTwistedPairSize() {
            return this->twistedPairSize;
        }
        /*
         * Add an amount of optical fiber cable to this configuration
         */
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

/*
 * A minimum heap specification. I had to implement a heap, because none of the available
 * heap implementations in C++ stantard library offered a decreaseKey method
 * Represents a heap of vertexes.
 */
class MinHeap {
    public:
        /* constructor */
        MinHeap(Vertex** vertexes, int size);
        /* destructor */
        ~MinHeap();
        /* check if the heap is empty */
        bool isEmpty();
        /* extract the vertex with minimum weight (cable length) of the heap */
        Vertex* extractMin();
        /* decrease the vertex key (the key is the minimum weight of an edge that connects the vertex to the minimum spanning tree) */
        void decreaseKey(Vertex* vertex, double key);
    private:
        /* put the heap element (in the index position) on the correct place, keeping the minimum heap property  */
        void minHeapify(int index);
        /* considering the node in the index i, calculates the index of the node's father, considering that the array is a tree */
        int father(int i);
        /* considering the node in the index i, calculates the index of the node's right child, considering that the array is a tree */
        int right(int i);
        /* considering the node in the index i, calculates the index of the node's left child, considering that the array is a tree */
        int left(int i);

        int size;           // the heap size
        Vertex** vertexes;  // the vertexes array
};

/* read the input cases */
list<Case*> readInput();
/* calculates the minimum cable configuration for a case */
NetworkStructure minimumCableConfiguration(Case* networkCase);

int main() {
    // read the input cases
    list<Case*> cases = readInput();

    // for each input case
    for (list<Case*>::iterator it = cases.begin(); it != cases.end(); it++) {
        Case* currentCase = *it;

        // calculates the minimum cable configuration using a spanning tree
        // considerating that each vertex is connected to all the other vertexes in the graph
        // of connection points
        NetworkStructure ns = minimumCableConfiguration(currentCase);
        
        // and prints the result
        cout << round(ns.getTwistedPairSize()) << " " 
                        << round(ns.getOpticalFiberSize()) << "\n";
    }

    return 0;
}

/* read the input cases */
list<Case*> readInput() {
    int numberOfCases;
    
    list<Case*> cases;

    // reads the number of cases
    cin >> numberOfCases;

    // for eache case
    for(int i = 0; i < numberOfCases; i++) {
        int numberOfConnectionPoints, twistedPairMaxDistance;

        // reads the number of connection points and the twisted pair cable maximum distance
        cin >> numberOfConnectionPoints >> twistedPairMaxDistance;

        // instantiate a case
        Case* currentCase = new Case(numberOfConnectionPoints, twistedPairMaxDistance);

        // for each connection point
        for(int j = 0; j < numberOfConnectionPoints; j++) {
            int x, y;

            // reads the connection point position in the plane
            cin >> x >> y;
            Coordinate* coordinate = new Coordinate(x, y);

            // instantiate a vertex for this connection point
            Vertex* vertex = new Vertex(coordinate);
            // add the vertex to the case being read
            currentCase->addVertex(vertex);
        }

        // add the current case to the list of cases
        cases.push_back(currentCase);
    }

    return cases;
}

/*
 * Calculates an edge weight. An edge weight is the distance of its vertex
 * this weight indicates the amount of cable to connect the vertexes through this edge
 */
double weight(Vertex* u, Vertex* v) {
    Coordinate* coordinateU = u->getCoordinate();
    Coordinate* coordinateV = v->getCoordinate();

    int cat1 = abs(coordinateU->getX() - coordinateV->getX());
    int cat2 = abs(coordinateU->getY() - coordinateV->getY());

    // calculates the distance between the vertexes u and v using pythagoras theorem
    return sqrt(cat1*cat1 + cat2*cat2);
}

/*
 * Calculates the minimum spawning using the Prim's algorithm, because the input graph is dense
 * It considerates that the input graph is complete, all the vertexes are adjacents to all the
 * other vertexes and each edge weight is calculated by the weight(u, v) function.
 */
void minimumSpanningTree(Case* networkCase, Vertex* root) {
    // the root vertex, is the root of the calculated minimum spanning tree
    // it starts with weight 0, because we haven't pass to any edge to arraive to
    // the tree root
    root->setKey(0);

    // creates a minimum heap with all the vertexes.
    // the heap property is kept by using each vertex key
    // we keep a control boolean to indicates if the vertex is on the heap, on each vertex
    // constructor we initialize the boolean with true, because this operation puts all the vertexes
    // on the heap. This flag avoid complexity.
    MinHeap heap(networkCase->getVertexes(), networkCase->getNumberOfConnectionPoints());
    
    // the heap has all the vertex that aren't in the mininum spanning tree yet
    // when the heap is empty, we calculated the entire mininum spanning tree
    while(!heap.isEmpty()) {
        // first, we extract the minimum value from the heap
        // on the first time, this value will be the root
        // because all the vertexes are initialized with DBL_MAX key (equivalent to infinity)
        Vertex* u = heap.extractMin();
        // u is not on the heap anymore 
        u->setOnHeap(false);
        
        Vertex** vertexes = networkCase->getVertexes();
        // all the other vertexes are adjacents to u, so here we iterate over each edge leaving u
        for (int i = 0; i < networkCase->getNumberOfConnectionPoints(); i++) {
            Vertex* vertex = vertexes[i];
            
            double edgeWeight = weight(u, vertex);
            // if the adjacent vertex is on the heap (is not on the minimum spanning tree yet)
            // an its key (minimum adjacent edge weight to connect to the tree) is higher than
            // the current edge, so the current edge is a better option for the minimum spanning tree
            // u is avoided here because its not on the heap. 
            if(vertex->isOnHeap() && edgeWeight < vertex->getKey()) {
                // if the current edge is a better option, we just set the vertex father on the
                // minimum spanning tree and decreases it on the heap
                vertex->setFather(u);
                heap.decreaseKey(vertex, edgeWeight);
            }
        }
    }
}

/*
 * Calculates the minimum cable configuration for a case using a minimum spanning tree
 * considerating that each vertex is connected to all the other vertexes in the graph
 * of connection points
 */
NetworkStructure minimumCableConfiguration(Case* networkCase) {
    Vertex** vertexes = networkCase->getVertexes();
    // choose an arbitrary root for the minimum spanning tree
    Vertex* root = vertexes[0];
    // calculates he minimum spanning tree
    minimumSpanningTree(networkCase, root);

    NetworkStructure structure;
    // for each connection point (vertex) in this case
    for (int i = 0; i < networkCase->getNumberOfConnectionPoints(); i++) {
        // the minimum spanning tree algorithm has calculated the minimum
        // amount of cable to connect each vertex to another vertex
        // in the minimum spanning tree.
        double edgeWeight = vertexes[i]->getKey();

        // this amount indicates the amount of cable needed to construct
        // the minimum network configuration to the given connection points

        // checks if the current amount need optical fiber or if twisted pair cable is enough
        if(edgeWeight > networkCase->getTwistedPairMaxDistance()) {
            structure.addOpticalFiber(edgeWeight);
        } else {
            structure.addTwistedPair(edgeWeight);
        }
    }

    return structure;
}

/*
 * Simple swap function, swaps two pointers to vertex
 */
void swap(Vertex** a, Vertex** b) {
    Vertex* tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * Constructs a heap given a array of vertexes and its size
 */
MinHeap::MinHeap(Vertex** vertexes, int size) {
    this->size = size; // the array size is the initial heap size
    this->vertexes = (Vertex**) malloc(size * sizeof(Vertex*));

    // create a copy of the array, so it wont destroy the input array
    for(int i = 0; i < size; i++) {
        this->vertexes[i] = vertexes[i];
        // keeps the vertex index in an atributte
        // to reduce the decreaseKey complexity
        this->vertexes[i]->setIndex(i);
    }

    // apply the min heapify algorithm to reorganize each element
    // to follow the minimum heap property
    for (int i = this->size/2; i >= 0; i--) {
        this->minHeapify(i);
    }

}

/*
 * The heap destructor, it deallocate the array
 */
MinHeap::~MinHeap() {
    free(this->vertexes);
}

/*
 * The heap is a tree that is represented using an array
 * this method calculates the index of the father of the node in the i index
 */
int MinHeap::father(int i) {
    return (i + 1)/2 - 1;
}

/*
 * The heap is a tree that is represented using an array
 * this method calculates the index of the left child of the node in the i index
 */
int MinHeap::left(int i) {
    return 2*(i + 1) - 1;
}

/*
 * The heap is a tree that is represented using an array
 * this method calculates the index of the right child of the node in the i index
 */
int MinHeap::right(int i) {
    return 2*(i + 1);
}

/*
 * Maintain the minimum heap property
 */
void MinHeap::minHeapify(int i) {
    int left = this->left(i);
    int right = this->right(i);
    int lowest;

    // if the left child is less than the current node, so the heap is in a wrong configuration
    // and the current node must be changed with its lowest child
    if (left < this->size && this->vertexes[left]->getKey() < this->vertexes[i]->getKey()) {
        lowest = left;
    } else {
        lowest = i;
    }

    // if the right child is less than the current node, so the heap is in a wrong configuration
    // and the current node must be changed with its lowest child
    if (right < this->size && this->vertexes[right]->getKey() < this->vertexes[lowest]->getKey()) {
        lowest = right;
    }

    if (lowest != i) {
        // if the heap is in the wrong configuration because of the current node, than
        // we swaps the node with its lowest child
        swap(this->vertexes + i, this->vertexes + lowest);

        // updates the swapped vertexes index
        this->vertexes[i]->setIndex(i);
        this->vertexes[lowest]->setIndex(lowest);

        // apply the minHeapify to the swapped node, until the node be in the right place
        this->minHeapify(lowest);
    }
}

/*
 * Check if the heap is empty
 */
bool MinHeap::isEmpty() {
    return this->size == 0;
}

/*
 * Extract the vertex with minimum weight (cable length) of the heap
 */
Vertex* MinHeap::extractMin() {
    if (this->size < 1) {
        return NULL; // heap underflow
    }

    // the vertex with the minimum weight is the tree root, the first element in vertexes array
    Vertex* min = this->vertexes[0];

    // removes the minimum element by changing it with the last element in the array 
    this->vertexes[0] = this->vertexes[this->size - 1];
    // update the changed vertex index
    this->vertexes[0]->setIndex(0);
    // update the heap size
    this->size = this->size - 1;
    
    // reorganize the heap to the correct configuration
    this->minHeapify(0);

    return min;
}

/*
 * Decrease the vertex key (the key is the minimum weight of an edge that connects the vertex
 * to the minimum spanning tree) and reorganize the heap
 */
void MinHeap::decreaseKey(Vertex* vertex, double key) {
    int i = vertex->getIndex();

    // if the key is bigger than the vertex current key, so the operation is invalid 
    if (key > this->vertexes[i]->getKey()) {
        return;
    }

    // changes the vertex key
    this->vertexes[i]->setKey(key);
    // while its key is less than its father on the heap tree, we swap the vertex with its father
    // until it be in the right position
    while (i > 0 && this->vertexes[this->father(i)]->getKey() > this->vertexes[i]->getKey()) {
        int father = this->father(i);

        swap(this->vertexes[i], this->vertexes[father]);

        this->vertexes[i]->setIndex(i);
        this->vertexes[father]->setIndex(father);

        i = this->father(i);
    }

}