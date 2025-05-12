#include "BikeStation.hpp"

#ifndef GRAPHNODE
#define GRAPHNODE
struct GNode{
    BikeStationEntry station;
    bool operator==(GNode);
};
#endif

#ifndef GRAPHEDGE
#define GRAPHEDGE
struct GEdge{
    GNode a, b;
    double weight;
};
#endif

#ifndef PQUEUE
#define PQUEUE
class GraphPriorityQueue{
    private:
        int size;
        vector<GNode> data;
        vector<double> priority;
        
    public:
        GraphPriorityQueue();
        void push(GNode, double);
        pair<GNode, double> pop();
        bool empty();
};
#endif

#ifndef GRAPH
#define GRAPH
class Graph{
    private:
        vector<GNode> nodes;
        vector<GEdge> edges;
    public:
        void addNode(BikeStationEntry);
        void addEdge(BikeStationEntry, BikeStationEntry);

        void deleteNode(BikeStationEntry);
        void deleteEdge(BikeStationEntry, BikeStationEntry);

        vector<BikeStationEntry> shortestPath(BikeStationEntry, BikeStationEntry);

};
#endif