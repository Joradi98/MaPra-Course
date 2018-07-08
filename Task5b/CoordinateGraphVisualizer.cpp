#pragma once

#include "unit.h"
#include <iostream>
#include "VisualizationUtilities.h"
#include "CoordinateGraph.h"

class CoordinateGraphVisualizer : public GraphVisualizer
{
private:
    sf::RenderWindow window;
    CoordinateGraph graph;
    std::vector<VertexInformation> vertexInfos;    /// Drawable information for each vertex
    std::map<EdgeT,EdgeInformation> edgeInfos; /// Drawable information for each edge
    sf::Font font;

    std::vector<std::pair<double,double>> screenCoordinates; //Screen coordinates for the verttices

public:
    
    /**
     Initialize for a given window size
     */
    CoordinateGraphVisualizer(int width, int height, CoordinateGraph cgraph): graph(cgraph) {
        window.create(sf::VideoMode(width, height), "Coordinate Graph visualizer");
        for (unsigned int i = 0; i < graph.numVertices(); i++) {
            vertexInfos.push_back(VertexInformation());
            vertexInfos[i].status = VertexStatus::UnknownVertex;
            vertexInfos[i].gValue = 0.0;
            vertexInfos[i].hValue = 0.0;
        }
        
        if (!font.loadFromFile("font/BebasNeue-Regular.ttf")) {
            std::cout << "Error on loading font!" << std::endl;
        }

        //Calculate screen coordinates for the vertices
        std::vector<std::pair<double,double>> coordinates; //In meters x and y
        double minX = 99999999999999;                       //Reference point in the upper left
        double minY = 99999999999999;
        double maxX = 0;                       //Reference point in the lower right
        double maxY = 0;
        for (unsigned int v = 0; v < graph.numVertices(); v++) {
            std::pair<double,double> coords = graph.getCoordinates(v);
            double y = lat2y_m(coords.first);  //Latitude to meters
            double x = lon2x_m(coords.second); //Longitude to meters
            
            if (x < minX)
                minX = x;
            if (y < minY)
                minY = y;
            if (x > maxX)
                maxX = x;
            if (y > maxY)
                maxY = y;
            
            coordinates.push_back(std::make_pair (x,y));
        }
        std::cout << minX << ", " << minY << std::endl;
        std::cout << maxX << ", " << maxY << std::endl;
        for (unsigned int v = 0; v < graph.numVertices(); v++) {
            double bufferSpace = 50.0;
            double x = bufferSpace/2.0 + (window.getSize().x - bufferSpace) / (maxX-minX) * (coordinates[v].first - minX); //-50 cause we dont wanna use all the space of the window
            double y = (window.getSize().y - bufferSpace/2.0) - (window.getSize().y - bufferSpace) / (maxY-minY) * (coordinates[v].second - minY); //-50 cause we dont wanna use all the space of the window
            screenCoordinates.push_back(std::make_pair(x,y));
        }

        
        
        //Draw the first time
        draw();
    }
    
    
    /**
     Keep the window open until the user closes it
     */
    void keepRunning() {
        while (window.isOpen()) {

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }

    
    
    
    //Convert long/lat coordinates to coordinates within the plane
    
    
    //implement draw() method
    
    void markVertex(VertexT vertex, VertexStatus status) override {
        vertexInfos[vertex].status = status;
  //      draw();
    }
    
    void markEdge(EdgeT e, EdgeStatus status) override {
        if ( edgeInfos.count(e) == 0 ) { //Create an empty record first
            edgeInfos[e] = EdgeInformation();
            edgeInfos[e].status = EdgeStatus::UnknownEdge;
            edgeInfos[e].cost = 0;
            draw();
        }
        
        
    }
    
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override {
        vertexInfos[vertex].status = status;
        vertexInfos[vertex].gValue = cost;
        vertexInfos[vertex].hValue = estimate;
//        draw();
    }
    
    
    void draw() override {
        window.clear(sf::Color::Black);
        drawVertices();
        drawEdges();
      //  std::cout << "Drawing..." << std::endl;

        
        window.display();
    }
private:
    
    void drawVertices() {
        sf::CircleShape shape(10);
        shape.setOrigin(shape.getRadius(), shape.getRadius());

       // shape.setOrigin(0, 0);
        shape.setFillColor(sf::Color(255, 255, 255));   //Done: White
     //   shape.setSize(sf::Vector2f(20,20));

        
        for (unsigned int v = 0; v < graph.numVertices(); v++) {
            shape.setPosition(screenCoordinates[v].first,screenCoordinates[v].second);
            window.draw(shape);
        }
    }
    
    
    void drawEdges() {
        
        for (auto const& keyValuePair : edgeInfos) {
            EdgeT edge = keyValuePair.first;
            EdgeInformation info = keyValuePair.second;

            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(screenCoordinates[edge.first].first, screenCoordinates[edge.first].second)),
                sf::Vertex(sf::Vector2f(screenCoordinates[edge.second].first, screenCoordinates[edge.second].second))
            };
            
            window.draw(line, 2, sf::Lines);

            
            
            
        }
        
        
        
    }
    
    
    
    
    
};
