
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
    BasicStaticGraph(int num_verts, std::vector<std::pair<EdgeT, CostT>> edgeData) : DistanceGraph(num_verts){
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
    CoordinateGraph(int num_verts, std::vector<std::pair<double, double>> coordinateData, std::vector<std::pair<EdgeT, CostT>> edgeData) : BasicStaticGraph(num_verts, edgeData) {
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

        double xDifference = fabs(firstCoordinate.first-secondCoordinate.first);
        double yDifference = fabs(firstCoordinate.second-secondCoordinate.second);

        return sqrt( xDifference*xDifference + yDifference*yDifference);
        
    }

};


class MazeGraph : public CoordinateGraph {

private:
    std::vector<std::pair<double, double>> coordinatesFromMazeData(std::vector<CellType> mazeData, int breite) {
        std::vector<std::pair<double, double>> coordinates;
        
        int hoehe = (int)mazeData.size() / breite;
        
        for ( int b = 0; b < breite; b++) {
            for ( int h = 0; h < hoehe; h++) {
                std::pair<double, double> coordinate = std::make_pair(b, h);
                coordinates.push_back(coordinate);
            }
        }
        return coordinates;
    }
    
    
    std::vector<std::pair<int, int>> validNeighbours(std::vector<CellType> mazeData, std::pair<int, int> currentTile, int breite) {
        std::vector<std::pair<int, int>> valid;
        
        int b = currentTile.first;
        int h = currentTile.second;
        int hoehe = mazeData.size() / breite;
        int size = mazeData.size();
        int neighbour = b-1 + h*breite;
        if (neighbour > 0 && b-1 >= 0 && neighbour < size) { //Nicht nach links rausfallen
            switch (mazeData[neighbour]) {
            	case CellType::Wall:
                    break;
                default:
                    valid.push_back(std::make_pair(b-1, h));
            }
        }
        
        
        neighbour = b+1 + h*breite;
        if (neighbour > 0 && b+1 < breite && neighbour < size) {
            switch (mazeData[neighbour]) { ///Nicht nach rechts rausfallen
            	case CellType::Wall:
                    break;
                default:
                    valid.push_back(std::make_pair(b+1, h));
            }
        }
        
        neighbour = b + (h+1)*breite;
        if (neighbour > 0 && h+1 < hoehe && neighbour < size) {
            switch (mazeData[neighbour]) { ///Nicht nach unten rausfallen
            	case CellType::Wall:
                    break;
                default:
                    valid.push_back(std::make_pair(b, h+1));
            }
        }
        
        neighbour = b + (h-1)*breite;
        if (neighbour > 0 && h-1 >= 0 && neighbour < size) {
            switch (mazeData[neighbour]) { ///Nicht nach oben rausfallen
            	case CellType::Wall:
                    break;
                default:
                    valid.push_back(std::make_pair(b, h-1));
            }
        }
        
        return valid;

    }    
    
    std::vector<std::pair<EdgeT, CostT>> costsFromMazeData(std::vector<CellType> mazeData, int breite)  {
        std::vector<std::pair<EdgeT, CostT>> edgeData; //Edgedata contains a list of edges and their costs
        
        int hoehe = mazeData.size()/breite;
        for ( int b = 0; b < breite; b++) {
            for ( int h = 0; h < hoehe; h++) {
                int myVertexNumber = b + h*breite;
                switch (mazeData[myVertexNumber]) {
                	case CellType::Wall: 
                        break;
                    default: //Passable tile
                        break;
                        //Schaue all moeglichen Nachbarn an, d.h. solche die passierbar sind
                        std::vector<std::pair<int, int>> valids = validNeighbours(mazeData,std::make_pair(b, h), breite);
                        for (unsigned int k = 0; k < valids.size(); k++) {
                            std::pair<int, int> coordinate = valids.at(k);
                            int neighbourVertexNumber = coordinate.first + breite*coordinate.second;
                            EdgeT edge = std::make_pair(myVertexNumber, neighbourVertexNumber);
                            edgeData.push_back( std::make_pair(edge, 1) ); //Cost is always 1
                        }
                }
            }
        }
        
        
        return edgeData;
    }

public:

    //Creates a maze where each tile is a vetex, nomatter if its passable or not. 
    //Costs are 1 is poassable and 0 if not.
    MazeGraph(std::vector<CellType> mazeData, int breite) : CoordinateGraph(mazeData.size(), coordinatesFromMazeData(mazeData, breite), costsFromMazeData(mazeData, breite)) {}
    
    CostT estimatedCost(VertexT from, VertexT to) const {
        // todo implement heuristic
        std::pair<double, double> firstCoordinate = coordinates.at(from);
        std::pair<double, double> secondCoordinate = coordinates.at(to);

        double xDifference = (firstCoordinate.first-secondCoordinate.first);
        double yDifference = (firstCoordinate.second-secondCoordinate.second);

        return  fabs(xDifference) + fabs(yDifference);
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







struct CompararingInstance {
    VertexT vertex;                // The actual graph this instance represents  
    VertexT target;                // The target where you wanna go at the end
    DistanceGraph *contextGraph;  // The graph in which this vertex can be found. Needed to estimate distances
    std::vector<VertexT> *gValues;  //gValues get changed, so only hold a pointer
    
    int vorgaenger;
    
    CompararingInstance(VertexT vertex, VertexT target, DistanceGraph *contextGraph, std::vector<VertexT> *gValues) {
        this->vertex = vertex;
        this->target = target;
        this->contextGraph = contextGraph;
        this->gValues = gValues;
        
        this->vorgaenger = -1;

    }
    
   // double f;
};

struct Comparator {
    bool operator()(const CompararingInstance &a, const CompararingInstance &b) const {
        return (*a.gValues)[a.vertex] + a.contextGraph->estimatedCost(a.vertex, a.target) > (*b.gValues)[b.vertex] + b.contextGraph->estimatedCost(b.vertex, b.target); 
    }
};



//bool A_star(DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
std::list<VertexT> A_star(DistanceGraph& g, VertexT start, VertexT ziel) {
    std::cout << "Soll Weg finden von " << start << " nach " << ziel << std::endl;

    std::list<VertexT> weg;

    
    std::vector<CompararingInstance> openElements;
    std::vector<CompararingInstance> exploredElements;


    std::set<VertexT> openVertices;
    std::set<VertexT> exploredVertices;


    std::vector<VertexT> gValues;

    for(unsigned int i = 0; i < g.numVertices(); i++){
        gValues.push_back(infty);
    }
    CompararingInstance startElement = CompararingInstance(start, ziel, &g, &gValues); //Construct starting element

    openElements.push_back(startElement); //Enquere starting element
    openVertices.insert(start);
    std::make_heap(openElements.begin(),openElements.end(), Comparator() ); //Make heap
    
    
    while ( !openElements.empty() ) {
        std::pop_heap(openElements.begin(), openElements.end(), Comparator()); //Moves highest element to the end
        CompararingInstance minfElement = openElements.back();
        openElements.pop_back();      //Pops last element
        
        exploredElements.push_back(minfElement);
        exploredVertices.insert(minfElement.vertex);
        if ( minfElement.vertex == ziel ) {
            // todo: reconstruct path
            CompararingInstance vorher = minfElement;
            weg.push_back(ziel);
            while ( vorher.vorgaenger >= 0 ) {
                std::cout << vorher.vertex;
                vorher = exploredElements.at(vorher.vorgaenger);
                weg.push_back(vorher.vertex);
                if (vorher.vertex == start) {  break; }
                //std::cout << " hat vorgaegner: " << vorher.vertex << std::endl;
              //  std::getchar();
            }
            weg.reverse();
          //  std::cout << " hat vorgaegner: " << minfElement.vorgaenger->vertex << std::endl;

            
            
            return weg;
        }
        
        //Expand to neighbours
        // überprüft alle Nachfolgeknoten und fügt sie der Open List hinzu, wenn entweder
        // - der Nachfolgeknoten zum ersten Mal gefunden wird oder
        // - ein besserer Weg zu diesem Knoten gefunden wird        
        std::vector<std::pair<VertexT, CostT>> neighbors = g.getNeighbors(minfElement.vertex);
        for(unsigned int i = 0; i < neighbors.size(); i++){
            //CompararingInstance(neighbors[i].first, ziel, &g,&gValues);
            CompararingInstance neighbourElement = CompararingInstance(neighbors[i].first, ziel, &g,&gValues); //Construct neighbour element
            if(exploredVertices.find(neighbourElement.vertex) != exploredVertices.end() ){ 
                continue;
            }   
        
            double tentative_g = gValues[minfElement.vertex] + g.cost(minfElement.vertex, neighbourElement.vertex);  
            if(openVertices.find(neighbourElement.vertex) != openVertices.end()  && tentative_g > gValues[neighbourElement.vertex]  ){
                continue;
            }
        
            neighbourElement.vorgaenger = exploredElements.size()-1 ;
           // std::cout << "Setze Weg von " << minfElement.vertex << " nach " << neighbourElement.vertex << std::endl;
            gValues[neighbourElement.vertex] = tentative_g;
                        
            if(openVertices.find(neighbourElement.vertex) != openVertices.end() ){
                //Decrease key, i.e. remove first and then reinsert with better key
                for (unsigned int k = 0; k < openElements.size(); k++) {
                    if ( openElements[k].vertex == neighbourElement.vertex ) {
                       /* openElements.erase(openElements.begin()+k);
                        openElements.push_back(neighbourElement);
                        openVertices.insert(neighbourElement.vertex);
                        std::push_heap(openElements.begin(), openElements.end(),Comparator()); */
                        std::make_heap(openElements.begin(),openElements.end(), Comparator() ); //Make heap again
                        break;
                    }

                }
                
                
            } else {
                // Insert
                openElements.push_back(neighbourElement);
                openVertices.insert(neighbourElement.vertex);
                std::push_heap(openElements.begin(), openElements.end(),Comparator()); 

            }
   
            
        }         

    }
    
  
    return weg;
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
           // Dijkstra(graph, i, costs); //Start Dijkstra from each vertex
           // PruefeDijkstra(example, i, costs);
            std::getchar();
            std::list<VertexT> weg = A_star(graph, 0, i);
            if (weg.size() > 1) {
                PruefeWeg(example, weg);
            }
            
        }

        

    }
    else {
        std::cout << "error reading file.";
    }
    
    
    std::vector<CellType> mazeData = ErzeugeLabyrinth(5, 10, 23453);
    MazeGraph graph = MazeGraph(mazeData,5);
    
    // PruefeHeuristik


    return 0;
}
