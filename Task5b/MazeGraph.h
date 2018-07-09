#include <iostream>
#include "unit.h"
#include "Constants.h" //MAZE_HEURISTIC

#ifndef MAZEGRAPH_H
#define MAZEGRAPH_H



/** MazeGraph
A graph representing a maze.
Each vertex can be either start/finish, waklable or a wall
*/
class MazeGraph : public CoordinateGraph {

private:

	//MARK: Static functions useful for constructing the maze and calling the super constructor

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

	
private:
    std::vector<CellType> cellData;
    
public:
	
	//MARK: Interface
	VertexT start;
	VertexT end;
	
    int width; //Number of tiles in a row
    int height;//Number of tiles in a line
    
	//Creates a maze where each tile is a vetex, nomatter if its passable or not. 
	//Costs are 1 is poassable and 0 if not.
	MazeGraph(std::vector<CellType> mazeData, int breite, int example) : CoordinateGraph(mazeData.size(), coordinatesFromMazeData(mazeData, breite), edgeDataFromMazeData(mazeData, breite), example) {
		findStartAndEnd(mazeData, breite);
        
        width = breite;
        height = mazeData.size() / breite;
        cellData = mazeData;
	}
	
	//Manhatten Distance
	CostT estimatedCost(VertexT from, VertexT to) const {

		std::pair<double, double> firstCoordinate = coordinates.at(from);
		std::pair<double, double> secondCoordinate = coordinates.at(to);

		double xDifference = (firstCoordinate.first-secondCoordinate.first);
		double yDifference = (firstCoordinate.second-secondCoordinate.second);
		
        switch (MAZE_HEURISTIC) {
            case 1:
                return fabs(xDifference) + fabs(yDifference);                      //Manhattan Distance
                break;
            case 2:
                return sqrt(xDifference*xDifference + yDifference*yDifference);    //Euclidean Distance
                break;
            case 3:
                return 2*sqrt(xDifference*xDifference + yDifference*yDifference);  //Scaled Euclidean Distance
                break;
            default:
                return 0;                                                          //0 Distance
                break;
        }

    
    }
    
    
    CellType typeOfCell(VertexT vertex) {
        return cellData[vertex];
    }
    
};


#endif

