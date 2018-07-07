#include "text_visualizer.h"
#include "unit.h"
#include <map>          // std::map (like a dictionary)
#include <set>          // std::set
#include <fstream>      // processing files
#include <algorithm>    // std::set_difference
#include <iostream>
#include "BasicStaticGraph.h"
#include "CoordinateGraph.h"
#include "MazeGraph.h"
#include "MazeVisualizer.cpp"
#include <SFML/Graphics.hpp>


/** Dijkastra Algorithm 
* Solves the Single-Source Shortest Path Problem with non-negative edge weights
*/
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



/** A* Algorithm 
* Finds the shortest path between start and ziel based on the graph's heuristic
*/
std::list<VertexT> A_star(DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel) {
   // std::cout << "Soll Weg finden von " << start << " nach " << ziel << std::endl;
   // std::cout << "Insgesamt " << g.numVertices() << " Knoten." << std::endl;

    std::list<VertexT> weg;

    
    std::vector<VertexT> openVertices;
    std::vector<VertexT> closedVertices;

    std::map<VertexT, VertexT> cameFrom; 
    std::map<VertexT, CostT> gScore; 
    std::map<VertexT, CostT> fScore; 

    
    //Init gScores
    for (VertexT i = 0; i < g.numVertices(); i++) {
        if (i == start ) {
            gScore[i] = 0;
        } else {
            gScore[i] = infty;
        }
    }

    //Init fScores
    for (VertexT i = 0; i < g.numVertices(); i++) {
        if (i == start ) {
            fScore[i] = g.estimatedCost(start, ziel);
        } else {
            fScore[i] = infty;
        }

    }

    //MARK all vertices, update current g and f scores
    for (VertexT i = 0; i < g.numVertices(); i++) {
     //   v.updateVertex(i, gScore[i], fScore[i]-gScore[i], 0, VertexStatus::UnknownVertex); //the second last argument is described as "VertexT parent" in unit.h . This is totally irrelevant and nonsense for us, so just pass 0 to it.
        
        //MARK all edges with their costs (will only be applicable to CoordinateGraphs, mazes dont need that shit)
        std::vector<std::pair<VertexT, CostT>> neighbors = g.getNeighbors(i);
        for(unsigned int k = 0; i < neighbors.size(); i++){
            v.markEdge(EdgeT(i, neighbors[k].first), EdgeStatus::UnknownEdge );
        }
    }
    //MARK destination
    v.markVertex(ziel, VertexStatus::Destination);

    
    
    openVertices.push_back(start);
    //MARK node in queue
    v.markVertex(start, VertexStatus::InQueue);

    while (!openVertices.empty()) {
        VertexT current = openVertices.back();
        // Get the node in openSet having the lowest fScore[] value
        for (unsigned int i = 0; i < openVertices.size(); i++) {
            VertexT instance = openVertices.at(i);
            if (fScore.at(instance) < fScore.at(current) ){
                current = instance;
                
            }
        }
        //MARK currently active node
        v.markVertex(current, VertexStatus::Active);

        
        if ( current == ziel ) {
            VertexT nachher = ziel;
            VertexT vorher;
            weg.push_back(ziel);
            while ( cameFrom.count(nachher) > 0 ) {
                //Calculate vertex from which the goal was reached
                vorher = cameFrom.at(nachher);

                //MARK optimal edge
                v.markEdge(EdgeT(vorher, nachher), EdgeStatus::Optimal );

                //Update for next iteration
                nachher = vorher;
                weg.push_back(vorher);
                
            }
            //Reverse the path
            weg.reverse();
            
            return weg;
        }
        
        openVertices.erase(std::find(openVertices.begin(),openVertices.end(),current) );
        closedVertices.push_back(current);
        
        std::vector<std::pair<VertexT, CostT>> neighbors = g.getNeighbors(current);
        for(unsigned int i = 0; i < neighbors.size(); i++){
            VertexT neighbor = neighbors[i].first;
            
            // if neighbor in closedSet
            if ( std::find(closedVertices.begin(), closedVertices.end(), neighbor) != closedVertices.end()  ) {
                continue;
            }
            
            //MARK Currently considered edge
            v.markEdge(EdgeT(current, neighbor), EdgeStatus::Active );
            
            // if neighbor not in openSet	// Discover a new node
            if ( std::find(openVertices.begin(), openVertices.end(), neighbor) == openVertices.end()  ) {
                openVertices.push_back(neighbor);
                //MARK neighbour as inQueue
                v.markVertex(neighbor, VertexStatus::InQueue);

            }
            
            
            double tentative_gScore = gScore.at(current) + g.cost(current, neighbor);
            

            //MARK Visited edge
            v.markEdge(EdgeT(current, neighbor), EdgeStatus::Visited );

            if (tentative_gScore >= gScore.at(neighbor) ) {
                continue;
            } else {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + g.estimatedCost(neighbor, ziel);
                
                //MARK gScore, hScore
                v.updateVertex(neighbor, gScore[neighbor], fScore[neighbor]-gScore[neighbor], 0, VertexStatus::InQueue); //the second last argument is described as "VertexT parent" in unit.h . This is totally irrelevant and nonsense for us, so just pass 0 to it.

            }
            
        }
        
        //MARK node as done
        v.markVertex(current, VertexStatus::Done);

        
        
    }


    //std::cout << "A* finished without finding a way" << std::endl;
    return weg;
}


/**
* Workflow for example being in [1,4] and thus a distance graph should be constructed
*/
void processDistance(int example) {
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
        CoordinateGraph graph = CoordinateGraph(numVertices, coordinateData, graphData, example); //CoordinateGraph constructed from file
        TextVisualizer visualizer = TextVisualizer();
        PruefeHeuristik(graph);
      
        for(VertexT i = 0; i < graph.numVertices(); i++){                //Apply Dijksstra to each vertex
            std::vector<CostT> costs;
            Dijkstra(graph, i, costs); //Start Dijkstra from each vertex
            PruefeDijkstra(example, i, costs);
            std::getchar();
            //Find every combination with A*
            for (VertexT j = 0; j < graph.numVertices();j++) {            //Apply A* to all combinations of start and ending vertices
                std::list<VertexT> weg = A_star(graph, visualizer,i, j);
                if (weg.size() > 1) {
                    PruefeWeg(example, weg);
                }
            }
       }
    }

}


/**
* Workflow for example being in [10,10] and thus a maze should be constructed
*/
void processMaze(int example) {
    std::ifstream file;

    if (example <= 9) {
        //load maze
        file.open("daten/Maze" + std::to_string(example-4) + ".dat");
        int breite;
        int hoehe;
        file >> breite;
        file >> hoehe;
        std::cout << breite << " und " << hoehe << std::endl;
        std::vector<CellType> mazeData;
        for(int i = 0; i < breite*hoehe; i++){
            char symbol;
            file >> symbol;
            //std::cout << "Read Symbol: " << symbol << std::endl;
            if (symbol == '.') {
                mazeData.push_back(CellType::Ground);
            } else if (symbol == '#') {
                mazeData.push_back(CellType::Wall);
            }
        }
        MazeGraph graph(mazeData,breite,example);    //Maze from file
        MazeVisualizer visualizer(1000,1000,graph);
        PruefeHeuristik(graph);
        
        for ( auto pair : StartZielPaare(example)) {            //Find shortest path solving the maze
            auto start = pair.first;
            auto goal  = pair.second;
            std::list<VertexT> weg = A_star(graph, visualizer, start, goal );
            PruefeWeg(example, weg);
        }
        visualizer.keepRunning();

    } else {
        //generate random maze
        srand(time(NULL));
        int seed = rand();
        std::cout << "Using seed " << seed << std::endl;
        std::vector<CellType> mazeData = ErzeugeLabyrinth(10, 10 , seed);
        MazeGraph graph(mazeData,10,10);              //Randomly generated maze
        MazeVisualizer visualizer(1000,1000,graph);

        PruefeHeuristik(graph);
        std::list<VertexT> weg = A_star(graph, visualizer, graph.start, graph.end); //Find shortest path solving the maze
        PruefeWeg(10, weg);
        visualizer.keepRunning();
    }
    

    
}


int main()
{
    int example = 0;
    std::cout << "enter [1-10] >> ";
    std::cin >> example;
    if (example <= 4 ) {
        processDistance(example);
    } else if (example <= 10) {
        processMaze(example);
    } else {
        std::cout << "error reading file.";
    }
    
    return 0;
}
