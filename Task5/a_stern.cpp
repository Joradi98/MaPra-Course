
#include "text_visualizer.h"
#include "unit.h"
#include <map>
#include <set>
#include <fstream>
#include <algorithm>    // std::set_difference
#include <iostream>

// Ein Graph, der Koordinaten von Knoten speichert.
/*
class CoordinateGraph : public DistanceGraph {
public:
    CoordinateGraph
    const NeighborT& getNeighbors( VertexT v) const override;
    CostT estimatedCost( VertexT from, VertexT to) const override;
    CostT cost( VertexT from, VertexT to) const override;
};
*/

class BasicStaticGraph : public DistanceGraph {
public:
    std::vector<std::vector<CostT>> costs;
    std::map<VertexT, NeighborT> neighbors;
public:
    BasicStaticGraph(int num_verts, std::vector<std::pair<EdgeT, CostT>>& edgeData) : DistanceGraph(num_verts){
        for(int i = 0; i < num_verts; i++){
            costs.push_back(std::vector<CostT>());
            for(int j = 0; j < num_verts; j++){
                costs[i].push_back(infty);
            }
        }
        for(int i = 0; i < num_verts; i++){
            neighbors[i] = std::vector<LocalEdgeT>();
        }
        for(unsigned int i = 0; i < edgeData.size(); i++){
            EdgeT edge = edgeData[i].first;
            CostT cost = edgeData[i].second;
            neighbors[edge.first].push_back(LocalEdgeT(edge.second, cost));
            costs[edge.first][edge.second] = cost;
        }
    }

    const NeighborT& getNeighbors(VertexT v) const {
        return neighbors.at(v);
    }

    CostT estimatedCost(VertexT from, VertexT to) const {
        return 42424242;
    }

    CostT cost(VertexT from, VertexT to) const {
        return costs[from][to];
    }
};

class CoordinateGraph : public BasicStaticGraph {
public:
    std::map<VertexT, std::pair<double, double>> coordinates;
public:
    CoordinateGraph(int num_verts, std::vector<std::pair<double, double>> coordinateData, std::vector<std::pair<EdgeT, CostT>>& edgeData) : BasicStaticGraph(num_verts, edgeData) {
        for(unsigned int i = 0; i < coordinateData.size(); i++){
            coordinates[i] = coordinateData[i];
        }
    }

    std::pair<double, double> getCoordinates(VertexT v){
        return coordinates.at(v);
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const {
        std::pair<double, double> firstCoordinate = coordinates.at(from);
        std::pair<double, double> secondCoordinate = coordinates.at(to);

        double xDifference = (firstCoordinate.first-secondCoordinate.first);
        double yDifference = (firstCoordinate.second-secondCoordinate.second);

        return sqrt( xDifference*xDifference + yDifference*yDifference);
        
    }

};

// TODO: this is bullshit according to the wanted "general type" VertexT
// we should have an option to iterate other the verticies, tho...
void Dijkstra(const DistanceGraph& g, VertexT start, std::vector<CostT>& costToStart) {
    std::set<VertexT> visitedVerticies;
    visitedVerticies.insert(start);

    for(VertexT i = 0; i < g.numVertices(); i++){
        costToStart.push_back(g.cost(start, i)); //Initialize with direct distances from the graph
    }
    costToStart[start] = 0;

    // As long as we have not visited all vertices
    while(visitedVerticies.size() < g.numVertices()){
        std::set<VertexT>::iterator it;
        std::set<VertexT> unexploredVerticies;
        
        //Get all the unvisited vertices
        // As long as all of the vertices are named {1,...,N} this works, but this ia not really general.
        for (VertexT i=0; i < g.numVertices(); i++) {
            if(visitedVerticies.find(i) == visitedVerticies.end()){
                unexploredVerticies.insert(i);
            }
        }
        if (unexploredVerticies.size() == 0) { break; }
    
        // Choose a random vertex at the beginning.
        VertexT minCostVertex = *unexploredVerticies.begin();
        CostT minCost = costToStart[minCostVertex];

        //Now get the minimum vertex
        for(it = unexploredVerticies.begin(); it != unexploredVerticies.end(); it++){
            if(costToStart[*it] < minCost){
                minCost = costToStart[*it];
                minCostVertex = *it;
            }
        }
        
        //Update values
        visitedVerticies.insert(minCostVertex);
        for(it = unexploredVerticies.begin(); it != unexploredVerticies.end(); it++){
            if(costToStart[*it] > costToStart[minCostVertex] + g.cost(minCostVertex, *it)){
                costToStart[*it] = costToStart[minCostVertex] + g.cost(minCostVertex, *it);
            }
        }
    }
}

class MazeGraph : public BasicStaticGraph {
public:

    MazeGraph(int num_verts, std::vector<std::pair<EdgeT, CostT>>& edgeData) : BasicStaticGraph(num_verts, edgeData) {
        // todo sth?
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const {
        // todo implement heuristic
        return 1;
    }
};



class VertexComparator {
public:
    DistanceGraph* context;
    std::vector<VertexT> gValues;
    VertexT target;
    VertexComparator(const VertexT& target, const DistanceGraph& context){
        this -> target = target;
        *(this -> context) = context;
        for(unsigned int i = 0; i < context.numVertices(); i++){
            gValues.push_back(infty);
        }
    } 

    bool operator()(const VertexT& a, const VertexT&b) const {
        return gValues[a] + context -> estimatedCost(a, target) < gValues[b] + context -> estimatedCost(b, target); 
    }
};


bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    int COST = 1;
    std::vector<VertexT> seenNodes;
    std::vector<VertexT> exploredNodes;
    VertexComparator comparator = VertexComparator(ziel, g);
    seenNodes.push_back(start);
    std::make_heap(seenNodes, VertexComparator());
    for(unsigned int i = 0; i < g.numVertices(); i++){
        comparator.gValues.push_back(infty);
    }
    while(!seenNodes.empty()){
        VertexT minfVertex = seenNodes.front();
        exploredNodes.push_back(minfVertex);
        if(minfVertex == ziel){
            // todo: reconstruct path
            return true;
        }
        std::pop_heap(seenNodes.begin(), seenNodes.end(), VertexComparator());
        seenNodes.pop_back();
        std::vector<std::pair<VectorT, CostT>> neighbors = g.getNeighbors(minfVertex);
        for(int i = 0; i < neighbors.size(); i++){
            if(exploredNodes.find(exploredNodes.begin(), exploredNodes.end(), neighbors[i][0]) == exploredNodes.end()){
                if(seenNodes.find(seenNodes.begin(), seenNodes.end(), neighbors[i][0] == seenNodes.end()){
                    seenNodes.push_back(neighbors[i][0]);
                    std::push_heap(seenNodes.begin(), seenNodes.end()); 
                }
                else {
                    comparator.gValues[neighbors[i][0]] = comparator.gValues[minfVertex] + COST;  
                }
            }
        }          
    }
    return false;
}

int main()
{
    int example = 0;
    std::cout << "enter [1-4] >> ";
    std::cin >> example;
    std::ifstream file;
    file.open("daten/Graph" + std::to_string(example) + ".dat");
    if(file.is_open()){
        int numVertices;
        int numEdges;
        file >> numVertices;
        file >> numEdges;
        std::vector<std::pair<EdgeT, CostT>> graphData;
        std::vector<std::pair<double, double>> coordinateData;
        for(int i = 0; i < numEdges; i++){
            int from, to;
            double cost;
            file >> from >> to >> cost;
            graphData.push_back(std::pair<EdgeT, CostT>(EdgeT(from, to), cost));
        }
        for(int i = 0; i < numVertices; i++){
            double x, y;
            file >> x >> y;
            coordinateData.push_back(std::pair<double, double>(x, y));
        }
        CoordinateGraph graph = CoordinateGraph(numVertices, coordinateData, graphData);
        PruefeHeuristik(graph);
        for(VertexT i = 0; i < graph.numVertices(); i++){
            std::vector<CostT> costs;
            Dijkstra(graph, i, costs); //Start Dijkstra from each vertex
            PruefeDijkstra(example, i, costs);
            std::getchar();
        }

    }
    else {
        std::cout << "error reading file.";
    }
    // PruefeHeuristik

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg

    return 0;
}
