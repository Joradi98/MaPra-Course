#pragma once

#include <math.h>
#define DEG2RAD(a)   ((a) / (180 / M_PI))
#define RAD2DEG(a)   ((a) * (180 / M_PI))
#define EARTH_RADIUS 6378137

#define SHOULD_DISPLAY_TEXT 0
#define SHOULD_DISPLAY_COSTS 0


///A struct storing information relevant for drawing a vertex
struct VertexInformation {
    VertexStatus status;
    double gValue;
    double hValue;
};

///A struct storing information relevant for drawing an edge
struct EdgeInformation {
    EdgeStatus status;
    double cost;
};

// Mercator projection: Returns approoximate distance from the equator
double lat2y_m(double lat) { return log(tan( DEG2RAD(lat) / 2 + M_PI/4 )) * EARTH_RADIUS; }
double lon2x_m(double lon) { return          DEG2RAD(lon)                 * EARTH_RADIUS; }
