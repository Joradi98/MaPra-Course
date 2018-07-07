/*****************************************************************
*  Name       : text_visualizer.h                                *
*  Verwendung : Implementation einer "Visualisierung" ueber      *
*               die Konsole.                                     *
*  Autor      : Sven Gross, Hauke Sass, Johannes Stemick,        *
*               IGPM RWTH Aachen                                 *
*  Datum      : Mai 2018                                         *
*****************************************************************/

#pragma once


#include "unit.h"

#include <iostream>

class DistanceGraphVisualizer : public GraphVisualizer
{
  public:
    
    //init with adaptive screensize
    
    //Convert long/lat coordinates to coordinates within the plane
    
    //hold reference to graoh and costs, and whatever
    
    //implement draw() method
    

    
    void markVertex(VertexT vertex, VertexStatus status) override {

    }
    
    void markEdge(EdgeT e, EdgeStatus status) override {

    }
    
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override {

    }
};
