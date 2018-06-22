
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


inline double getDist(double lat1,double long1,double lat2,double long2) {
    /// @brief The usual PI/180 constant
    static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
    /// @brief Earth's quatratic mean radius for WGS-84
    static const double EARTH_RADIUS_IN_METERS = 1; //6372797.560856

    double latitudeArc  = (lat1 - lat2) * DEG_TO_RAD;
    double longitudeArc = (long1 - long2) * DEG_TO_RAD;
    double latitudeH = sin(latitudeArc * 0.5);
    latitudeH *= latitudeH;
    double lontitudeH = sin(longitudeArc * 0.5);
    lontitudeH *= lontitudeH;
    double tmp = cos(lat1*DEG_TO_RAD) * cos(lat2*DEG_TO_RAD);
    return EARTH_RADIUS_IN_METERS*2.0 * asin(sqrt(latitudeH + tmp*lontitudeH));
}






class CoordinateGraph : public BasicStaticGraph {
public:
    std::map<VertexT, std::pair<double, double>> coordinates;
    int example;
public:
    CoordinateGraph(int num_verts, std::vector<std::pair<double, double>> coordinateData, std::vector<std::pair<EdgeT, CostT>> edgeData, int example) : BasicStaticGraph(num_verts, edgeData) {
        for(unsigned int i = 0; i < coordinateData.size(); i++){
            coordinates[i] = coordinateData[i];
        }
        this->example = example;
    }


    std::pair<double, double> getCoordinates(VertexT v){
        return coordinates.at(v);
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const {
        std::pair<double, double> firstCoordinate = coordinates.at(from);
        std::pair<double, double> secondCoordinate = coordinates.at(to);
        double xDifference = fabs(firstCoordinate.first-secondCoordinate.first);
        double yDifference = fabs(firstCoordinate.second-secondCoordinate.second);

        if (example == 1) {

            return sqrt( xDifference*xDifference + yDifference*yDifference);

        } else if (example == 2) {
            return getDist(firstCoordinate.first, firstCoordinate.second, secondCoordinate.first, secondCoordinate.second);            
        } else if (example == 3) {
            return sqrt( xDifference*xDifference + yDifference*yDifference);
        } else {
            return 0;
        }
        
        return 0;
    }

};


class MazeGraph : public CoordinateGraph {

public:
    static std::vector<std::pair<double, double>> coordinatesFromMazeData(std::vector<CellType> mazeData, int breite) {
        std::vector<std::pair<double, double>> coordinates;
        
        int hoehe = (int)mazeData.size() / breite;
        
        for ( int h = 0; h < hoehe; h++) {
            for ( int b = 0; b < breite; b++) {
                std::pair<double, double> coordinate = std::make_pair(b, h);
                coordinates.push_back(coordinate);
            }
        }
        return coordinates;
    }
    
    
   static std::vector<std::pair<int, int>> validNeighbours(std::vector<CellType> mazeData, std::pair<int, int> currentTile, int breite) {
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
    
    static std::vector<std::pair<EdgeT, CostT>> edgeDataFromMazeData(std::vector<CellType> mazeData, int breite)  {
        std::vector<std::pair<EdgeT, CostT>> edgeData; //Edgedata contains a list of edges and their costs

        int hoehe = mazeData.size()/breite;
        for ( int b = 0; b < breite; b++) {
            for ( int h = 0; h < hoehe; h++) {
                int myVertexNumber = b + h*breite;
                switch (mazeData[myVertexNumber]) {
                	case CellType::Wall: 
                        break;
                    default: //Passable tile
                        //Schaue all moeglichen Nachbarn an, d.h. solche die passierbar sind
                        std::vector<std::pair<int, int>> valids = validNeighbours(mazeData,std::make_pair(b, h), breite);
                        for (unsigned int k = 0; k < valids.size(); k++) {
                            std::pair<int, int> coordinate = valids.at(k);
                            int neighbourVertexNumber = coordinate.first + breite*coordinate.second;
                            EdgeT edge = std::make_pair(myVertexNumber, neighbourVertexNumber);
                            edgeData.push_back( std::make_pair(edge, 1) ); //Cost is always 1
                        }
                        break;
                }
            }
        }
        
        
        return edgeData;
    }

    void findStartAndEnd (std::vector<CellType> mazeData, int breite) {
        int hoehe = (int)mazeData.size() / breite;

        for ( int b = 0; b < breite; b++) {
            for ( int h = 0; h < hoehe; h++) {
                switch (mazeData[b+h*breite]) {
                	case CellType::Start:
                        this->start = b+h*breite;
                        break;
                    case CellType::Destination:
                        this->end = b+h*breite;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    

public:
    
    VertexT start;
    VertexT end;
    
    //Creates a maze where each tile is a vetex, nomatter if its passable or not. 
    //Costs are 1 is poassable and 0 if not.
    MazeGraph(std::vector<CellType> mazeData, int breite, int example) : CoordinateGraph(mazeData.size(), coordinatesFromMazeData(mazeData, breite), edgeDataFromMazeData(mazeData, breite), example) {
        findStartAndEnd(mazeData, breite);

        
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const {

        std::pair<double, double> firstCoordinate = coordinates.at(from);
        std::pair<double, double> secondCoordinate = coordinates.at(to);

        double xDifference = (firstCoordinate.first-secondCoordinate.first);
        double yDifference = (firstCoordinate.second-secondCoordinate.second);
    //std::cout << firstCoordinate.second << secondCoordinate.second << std::endl;
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
    
};

struct Comparator {
    bool operator()(const CompararingInstance &a, const CompararingInstance &b) const {
        return (*a.gValues)[a.vertex] + a.contextGraph->estimatedCost(a.vertex, a.target) > (*b.gValues)[b.vertex] + b.contextGraph->estimatedCost(b.vertex, b.target); 
    }
};



//bool A_star(DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
std::list<VertexT> A_star(DistanceGraph& g, VertexT start, VertexT ziel) {
    std::cout << "Soll Weg finden von " << start << " nach " << ziel << std::endl;
    std::cout << "Insgesamt " << g.numVertices() << " Knoten." << std::endl;

    std::list<VertexT> weg;

    
    std::vector<CompararingInstance> openElements;
    std::vector<CompararingInstance> exploredElements;
    std::set<VertexT> openVertices;
    std::set<VertexT> exploredVertices;


    std::vector<VertexT> gValues;
    for(unsigned int i = 0; i < g.numVertices(); i++){
        if (i == start ) {
            gValues.push_back(0);
        } else {
            gValues.push_back(infty);
        }

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
            std::cout << "yoyo";
            CompararingInstance vorher = minfElement;
            weg.push_back(ziel);
            while ( vorher.vorgaenger >= 0 ) {
                //std::cout << vorher.vertex << std::endl ;
                vorher = exploredElements.at(vorher.vorgaenger);
                weg.push_back(vorher.vertex);
            }
            weg.reverse();
            return weg;
        }
        
        //Expand to neighbours
        // überprüft alle Nachfolgeknoten und fügt sie der Open List hinzu, wenn entweder
        // - der Nachfolgeknoten zum ersten Mal gefunden wird oder
        // - ein besserer Weg zu diesem Knoten gefunden wird        
        std::vector<std::pair<VertexT, CostT>> neighbors = g.getNeighbors(minfElement.vertex);
        for(unsigned int i = 0; i < neighbors.size(); i++){
           // std::cout << "Neighbour:" << std::endl;
            //std::cout << neighbors[i].first << std::endl;
            //CompararingInstance(neighbors[i].first, ziel, &g,&gValues);
            CompararingInstance neighbourElement = CompararingInstance(neighbors[i].first, ziel, &g,&gValues); //Construct neighbour element
            if(exploredVertices.find(neighbourElement.vertex) != exploredVertices.end() ){ 
                continue;
            }     
            double tentative_g = gValues[minfElement.vertex] + g.cost(minfElement.vertex, neighbourElement.vertex);  
            

            //Alternative 2
            if(openVertices.find(neighbourElement.vertex) != openVertices.end()  && tentative_g >= gValues[neighbourElement.vertex]  ){
                continue;
            }

            neighbourElement.vorgaenger = exploredElements.size()-1 ;
            // std::cout << "Setze Weg von " << minfElement.vertex << " nach " << neighbourElement.vertex << std::endl;
            gValues[neighbourElement.vertex] = tentative_g;
                        
            if(openVertices.find(neighbourElement.vertex) != openVertices.end() ){
                std::make_heap(openElements.begin(),openElements.end(), Comparator() ); //Make heap again to update f value                
            } else {
                // Insert
                openElements.push_back(neighbourElement);
                openVertices.insert(neighbourElement.vertex);
                std::push_heap(openElements.begin(), openElements.end(),Comparator()); 
            }
            
        }         

    }
    
    std::cout << "A* finished without finding a way" << std::endl;
    return weg;
}


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
        CoordinateGraph graph = CoordinateGraph(numVertices, coordinateData, graphData, example);
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

}

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
        MazeGraph graph = MazeGraph(mazeData,breite,example);
        PruefeHeuristik(graph);
        for ( auto pair : StartZielPaare(example)) {
            auto start = pair.first;
            auto goal  = pair.second;
            std::list<VertexT> weg = A_star(graph, start, goal );
            PruefeWeg(example, weg);
        }



        
        
        
        
        
        
    } else {
        //generate maze
        srand(time(NULL));
        int seed = rand();
        std::cout << "Using seed " << seed << std::endl;
        std::vector<CellType> mazeData = ErzeugeLabyrinth(5, 10 , seed);
        MazeGraph graph = MazeGraph(mazeData,5,10);
         
        PruefeHeuristik(graph);
        std::list<VertexT> weg = A_star(graph, graph.start, graph.end);
        PruefeWeg(10, weg);

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
