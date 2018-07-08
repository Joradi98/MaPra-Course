#pragma once

#include <iostream>
#include "unit.h"

/**
Earth is not flat, so better use angles on the sphere to calculate effective travel distances
*/
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


/** CoordinateGraph
Each vertex represents a point on space holding a longitude and latitude.
*/
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
		  return 0;
		} else if (example == 3) {
			return sqrt( xDifference*xDifference + yDifference*yDifference);
		} else if (example == 4){
			return getDist(firstCoordinate.first, firstCoordinate.second, secondCoordinate.first, secondCoordinate.second) * 1.0 / 200.0; // 200kmh
		} else {
			return 0;
		}
		
		return 0;
	}

};

