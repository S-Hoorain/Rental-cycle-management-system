#include "graph.hpp"
#include <climits>
#include <unordered_map>
#include <algorithm>

#ifndef INF
#define INF INT_MAX
#endif

bool GNode::operator==(GNode n) {   return this->station == n.station;  }

GraphPriorityQueue::GraphPriorityQueue() : size(0) {}
bool GraphPriorityQueue::empty() {
    return size == 0;
}

void GraphPriorityQueue::push(GNode d, double p) {
    if (size == 0) {
        data.push_back(d);
        priority.push_back(p);
        ++size;
        return;
    }
    for (int i = 0; i < size; ++i) {
        if (data[i] == d && p < priority[i]) {
            priority[i] = p;
            return;
        }
    }
    data.push_back(d);
    priority.push_back(p);
    ++size;
}

pair<GNode, double> GraphPriorityQueue::pop() {
    int min = 0;
    for (int i = 1; i < size; ++i) {
        if (priority[i] < priority[min]) min = i;
    }
    int last = (-1)%size;
    GNode d = data[last];
    data[last] = data[min];
    data[min] = d;
    double p = priority[last];
    priority[last] = priority[min];
    priority[min] = p;
    data.pop_back();
    priority.pop_back();
    --size;
    return pair<GNode, double>(data[min], priority[min]);
}

void Graph::addNode(BikeStationEntry s) {
    for (GNode n: nodes) {
        if (n.station == s) return;
    }
    GNode node;
    node.station = s;
    this->nodes.push_back(node);
}

void Graph::addEdge(BikeStationEntry s1, BikeStationEntry s2) {
    for (GEdge e: edges) {
        bool x = e.a.station == s1 && e.b.station == s2;
        bool y = e.a.station == s2 && e.b.station == s1;
        if (x || y) return;
    }
    bool nodeA = false, nodeB = false;
    GNode n1, n2;
    for (GNode n: nodes) {
        if (n.station == s1) {
            nodeA = true;   n1 = n;
        } else if (n.station == s2) {
            nodeB = true;   n2 = n;
        }
    }
    if (!nodeA || !nodeB) return;
    BikeStations B;
    double dist = B.CalculateDistance(s1.Latitude, s1.Longitude, s2.Latitude, s2.Longitude);
    GEdge edge;
    edge.a = n1;    edge.b = n2;
    edge.weight = dist;
    this->edges.push_back(edge);
}

void Graph::deleteEdge(BikeStationEntry s1, BikeStationEntry s2) {
    int u = this->edges.size();
    bool found = false;
    for (int i = 0; i < u; ++i) {
        GEdge e = edges[i];
        if ((e.a.station == s1 && e.b.station == s2) || (e.a.station == s2 && e.b.station == s1)) {
            GEdge temp = edges[(-1)%u];
            edges[i] = temp;
            edges[(-1)%u] = e;
            found = true;
            break;
        }
    }
    if (found) edges.pop_back();
}

void Graph::deleteNode(BikeStationEntry s) {
    for (GEdge e: edges) {
        if (e.a.station == s || e.b.station == s) this->deleteEdge(e.a.station, e.b.station);
    }
}

vector<BikeStationEntry> Graph::shortestPath(BikeStationEntry source, BikeStationEntry dest) {

    GraphPriorityQueue queue;
    vector<double> distances(nodes.size(), INF); 
    vector<int> previous(nodes.size(), -1); 
    vector<bool> visited(nodes.size(), false);

    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].station == source) {
            sourceIndex = i;
            distances[i] = 0;
            queue.push(nodes[i], 0);
        }
        if (nodes[i].station == dest) {
            destIndex = i;
        }
    }

    if (sourceIndex == -1 || destIndex == -1) {
        return {};
    }

    while (!queue.empty()) {
        pair<GNode, double> currentPair = queue.pop();
        GNode currentNode = currentPair.first;
        double currentDist = currentPair.second;
        int currentNodeIndex = -1;
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i] == currentNode) {
                currentNodeIndex = i;
                break;
            }
        }
        if (visited[currentNodeIndex]) {
            continue;
        }
        visited[currentNodeIndex] = true;
        for (int i = 0; i < edges.size(); ++i) {
            GEdge edge = edges[i];
            GNode neighbor;
            int neighborIndex = -1;
            if (edge.a == currentNode) {
                neighbor = edge.b;
            } else if (edge.b == currentNode) {
                neighbor = edge.a;
            } else {
                continue;
            }
            for (int j = 0; j < nodes.size(); ++j) {
                if (nodes[j] == neighbor) {
                    neighborIndex = j;
                    break;
                }
            }
            if (neighborIndex == -1 || visited[neighborIndex]) {
                continue;
            }
            double newDist = currentDist + edge.weight;
            if (newDist < distances[neighborIndex]) {
                distances[neighborIndex] = newDist;
                previous[neighborIndex] = currentNodeIndex;
                queue.push(neighbor, newDist);
            }
        }
    }
    vector<BikeStationEntry> path;
    int currentNodeIndex = destIndex;
    while (currentNodeIndex != -1) {
        path.push_back(nodes[currentNodeIndex].station);
        currentNodeIndex = previous[currentNodeIndex];
    }
    for (int i = 0; i < path.size()/2; ++i) {
        int j = (path.size() - 1) - i;
        BikeStationEntry e = path[i];
        path[i] = path[j];
        path[j] = e;
    }
    return path;
}