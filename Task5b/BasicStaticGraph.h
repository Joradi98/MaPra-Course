#include <iostream>
#include "unit.h"

/*
Basic graph without any sensefull heuristic yet. Better use the subclasses Coordinategraph and MazeGraph
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

