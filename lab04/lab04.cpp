#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct Edge;

/*
 * Represents a graph vertex
 */
typedef struct Vertex {
    int id;             // the vertex identification
    vector<struct Edge*> adjacencies;

    int distance;       // the distance of the vertex from the origin vertex, used in dijkstra
    Vertex* father;     // the vertex predecessor in the minimum path from the origin vertex, used in dijkstra

    int index;          // the vertex index in the dijkstra heap
} Vertex;

/*
 * Represents a graph edge
 */
typedef struct Edge {
    int weight;         // the edge weight
    Vertex* origin;     // the edge origin vertex
    Vertex* destiny;    // the edge destiny vertex
} Edge;

/*
 * Represents a Graph
 */
class Graph {
    public:
        /*
         * Builds a graph with the number of vertexes
         */
        Graph(int numberOfVertexes){
            // initialize the graph with the number of vertexes (n) identified from 0 to n-1
            for(int vertexId = 0; vertexId < numberOfVertexes; vertexId++) {
                Vertex* vertex = new Vertex;
                vertex->id = vertexId;
                vertex->distance = INT_MAX;

                // the vertexes final position is in order 0, 1, 2, 3, .., n-1
                this->vertexes.push_back(vertex);
            }
        }

        /*
         * Add a new edge to the graph that goes from originId to destinyId
         * with the given weight
         */
        void addEdge(int originId, int destinyId, int weight) {
            Vertex* origin = this->vertexes.at(originId);
            Vertex* destiny = this->vertexes.at(destinyId);

            Edge* edge = new Edge;
            edge->weight = weight;
            edge->origin = origin;
            edge->destiny = destiny;

            // add the edge to its origin vertex adjacencies
            origin->adjacencies.push_back(edge);
            // add the edge to the graph edges vector
            // important to search the heavier edge
            this->edges.push_back(edge);
        }

        int getNumberOfVertexes() {
            return this->vertexes.size();
        }

        int getNumberOfEdges() {
            return this->edges.size();
        }

        /*
         * Returns the vertex with the given id
         */
        Vertex* getVertex(int id) {
            return this->vertexes.at(id);
        }

        vector<Vertex*> getVertexes() {
            return this->vertexes;
        }

        vector<Edge*> getEdges() {
            return this->edges;
        }
    private:
        vector<Vertex*> vertexes;
        vector<Edge*> edges;    
};

/*
 * Represents a race with its origin and destiny
 * its limit of points and the supermarket halls and places
 * represented by a graph
 */
typedef struct {
    Graph* graph;
    int origin;
    int destiny;
    int limitOfPoints;
} Race;

/*
 * A minimum heap specification. I had to implement a heap, because none of the available
 * heap implementations in C++ stantard library offered a decreaseKey method
 * Represents a heap of vertexes.
 */
class MinHeap {
    public:
        /* constructor */
        MinHeap(vector<Vertex*> vertexes);
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

        int size;                  // the heap size
        vector<Vertex*> vertexes;  // the vertexes array
};

vector<Race> read();
int maximumPrizeCrazyRace(Race race);

int main() {
    // reads the possible races
    vector<Race> races = read();
    
    for(unsigned int i = 0; i < races.size(); i++) {
        Race race = races.at(i);
        // for each race calculates the maximum prize of a craze race
        int result = maximumPrizeCrazyRace(race);
        cout << result << "\n";
    }

    return 0;
}

/*
 * Reads the input races
 */
vector<Race> read() {
    int numberOfRaces;
    
    // read the number of races in the input
    cin >> numberOfRaces;

    vector<Race> races;

    for(int j = 0; j < numberOfRaces; j++) {
        Race race;
        int numberOfVertexes,
            numberOfEdges;

        // read the race details
        cin 
            >> numberOfVertexes 
            >> numberOfEdges 
            >> race.origin 
            >> race.destiny 
            >> race.limitOfPoints;

        race.graph = new Graph(numberOfVertexes);

        for(int i = 0; i < numberOfEdges; i++) {
            int u, v, weight;
            // read each hall of the supermaket and represents it as
            // an edge in the race graph
            cin >> u >> v >> weight;
            race.graph->addEdge(u, v, weight);
        }

        races.push_back(race);
    }

    return races;    
}

/*
 * Initializes the graph to find the single source minimum path 
 */
void initializeSingleSource(Graph* graph, int source) {
    for(int i = 0; i < graph->getNumberOfVertexes(); i++) {
        // at the beginning, every vertex has infinity distance from the source
        // this is the distance if there is no path between the vertex and the source
        Vertex* vertex = graph->getVertex(i);
        vertex->distance = INT_MAX;
        vertex->father = NULL;
    }

    // the distance from the source vertex to itself is zero
    Vertex* sourceVertex = graph->getVertex(source);
    sourceVertex->distance = 0;
}

/*
 * Relax an edge. Check if we can improve the distance between
 * the source vertex and the edge's destiny vertex
 */
void relax(Edge* edge, MinHeap* heap) {
    if(edge->destiny->distance > edge->origin->distance + edge->weight) {
        // if the edges' origin vertex has infinity distance from the origin vertex
        // so we can't improve the minimum path using this edge
        if(edge->origin->distance != INT_MAX) {
            // but if we can, decrease the edges' destiny vertex priority
            // in the priority queue
            heap->decreaseKey(edge->destiny, edge->origin->distance + edge->weight);
            edge->destiny->father = edge->origin;
        }
    }
}

/*
 * Dijkstra algorithm for single-source minimum path problem
 */
void dijkstra(Graph* graph, int source) {
    // initializes the graph
    initializeSingleSource(graph, source);

    // initializes the priority queue with the graphs' vertexes
    MinHeap* heap = new MinHeap(graph->getVertexes());
    while(!heap->isEmpty()) {
        Vertex* vertex = heap->extractMin();
        
        // untill the priority queue be empty, we relax every edge
        // from the minimum vertex in the queue
        for(unsigned int i = 0; i < vertex->adjacencies.size(); i++) {
            Edge* edge = vertex->adjacencies.at(i);
            relax(edge, heap);
        }
    }
}

/*
 * Returns a new graph with the edges of the original graph reversed
 */
Graph* reverse(Graph* graph) {
    Graph* inverted = new Graph(graph->getNumberOfVertexes());

    // for each edge in the graph, we add a new edge
    // in the inverted graph with its vertexes reversed
    for(int i = 0; i < graph->getNumberOfEdges(); i++) {
        Edge* edge = graph->getEdges().at(i);
        inverted->addEdge(edge->destiny->id, edge->origin->id, edge->weight);
    }

    return inverted;
}

/*
 * Compare edges by its weight to be used in a sorting algorithm
 * to sort the edges in descending order
 */
bool compareEdgesDesc(Edge* edge1, Edge* edge2) {
    return edge1->weight > edge2->weight;
}

/*
 * Calculates the maximum prize that can be reached in a craze race
 */
int maximumPrizeCrazyRace(Race race) {
    // first we apply dijkstra in the race graph to
    // discover each vertex distance from the race origin
    dijkstra(race.graph, race.origin);

    // then we reverse the graph
    Graph* inverted = reverse(race.graph);

    // and find the minimum distance from each vertex from the race destiny
    dijkstra(inverted, race.destiny);

    // then we sort in descending order both graphs edges
    vector<Edge*> edges = race.graph->getEdges();
    sort(edges.begin(), edges.end(), compareEdgesDesc);
    vector<Edge*> invertedEdges = inverted->getEdges();
    sort(invertedEdges.begin(), invertedEdges.end(), compareEdgesDesc);

    // search for the maximum prize, the haevier edge will be evaluated before
    for(int i = 0; i < race.graph->getNumberOfEdges(); i++) {
        Edge* edge = edges.at(i);
        Edge* invertedEdge = invertedEdges.at(i);

        // if the edge can't be reached from the origin and arrive to the destiny
        // so it is not considered
        if(edge->origin->distance == INT_MAX || invertedEdge->origin->distance == INT_MAX) {
            continue;
        }

        // the maximum prize will be the points of the heavier edge (u, v)
        // that the sum of its points with the minimum distance of u from the race origin
        // and the minimum distance of v from the race destiny
        int points = edge->origin->distance + edge->weight + invertedEdge->origin->distance;
        if(points <= race.limitOfPoints){
            return edge->weight;
        }
    }

    return -1;
}

/*
 * Constructs a heap given a array of vertexes and its size
 */
MinHeap::MinHeap(vector<Vertex*> vertexes) {
    this->size = vertexes.size();
    // create a copy of the array, so it wont destroy the input array
    for(int i = 0; i < this->size; i++) {
        Vertex* vertex = vertexes.at(i);
        this->vertexes.push_back(vertex);
        // keeps the vertex index in an atributte
        // to reduce the decreaseKey complexity
        vertex->index = (i);
    }

    // apply the min heapify algorithm to reorganize each element
    // to follow the minimum heap property
    for (int i = this->size/2; i >= 0; i--) {
        this->minHeapify(i);
    }

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
    if (left < this->size && this->vertexes.at(left)->distance < this->vertexes.at(i)->distance) {
        lowest = left;
    } else {
        lowest = i;
    }
    // if the right child is less than the current node, so the heap is in a wrong configuration
    // and the current node must be changed with its lowest child
    if (right < this->size && this->vertexes.at(right)->distance < this->vertexes.at(lowest)->distance) {
        lowest = right;
    }

    if (lowest != i) {
        // if the heap is in the wrong configuration because of the current node, than
        // we swaps the node with its lowest child
        iter_swap(this->vertexes.begin() + i, this->vertexes.begin() + lowest);

        // updates the swapped vertexes index
        this->vertexes.at(i)->index = i;
        this->vertexes.at(lowest)->index = lowest;

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
    Vertex* min = this->vertexes.at(0);

    // removes the minimum element by changing it with the last element in the array 
    this->vertexes.at(0) = this->vertexes.at(this->size - 1);
    // update the changed vertex index
    this->vertexes.at(0)->index = 0;
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
    int i = vertex->index;

    // if the key is bigger than the vertex current key, so the operation is invalid 
    if (key > this->vertexes.at(i)->distance) {
        return;
    }

    // changes the vertex key
    this->vertexes.at(i)->distance = key;
    // while its key is less than its father on the heap tree, we swap the vertex with its father
    // until it be in the right position
    while (i > 0 && this->vertexes.at(this->father(i))->distance > this->vertexes.at(i)->distance) {
        int father = this->father(i);

        swap(this->vertexes.at(i), this->vertexes.at(father));

        this->vertexes.at(i)->index = i;
        this->vertexes.at(father)->index = father;

        i = this->father(i);
    }

}