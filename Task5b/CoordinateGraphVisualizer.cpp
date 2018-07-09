#pragma once

#include "unit.h"
#include <iostream>
#include "VisualizationUtilities.h"
#include "CoordinateGraph.h"
#include "Constants.h"              //SHOULD_DRAW_TEXT
#include <thread>
#include <future>

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
        
      //  std::thread t(&CoordinateGraphVisualizer::keepRunning, this);
      //  t.join()
        
        // create a thread with func() as entry point
     //   sf::Thread thread(&CoordinateGraphVisualizer::keepRunning,this);
        // run it

       

    }
    
    
    /**
     Returns when a mouse click was recognized
     */
    void waitForMouseClick() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::MouseButtonReleased)
                    return;
            }
        }
    }

    
    
    
    //Convert long/lat coordinates to coordinates within the plane
    
    
    //implement draw() method
    
    void markVertex(VertexT vertex, VertexStatus status,  bool updateGraphic=true) override {
        vertexInfos[vertex].status = status;
        if (updateGraphic == true)
            draw();
    }
    
    void markEdge(EdgeT e, EdgeStatus status,  bool updateGraphic=true) override {
        if ( edgeInfos.count(e) == 0 ) { //Create an empty record first
            edgeInfos[e] = EdgeInformation();
            edgeInfos[e].status = EdgeStatus::UnknownEdge;
            edgeInfos[e].cost = graph.cost(e.first,e.second);
        }
        edgeInfos[e].status = status;

    
        if (updateGraphic == true)
            draw();
    }
    
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status,  bool updateGraphic=true) override {
        vertexInfos[vertex].status = status;
        vertexInfos[vertex].gValue = cost;
        vertexInfos[vertex].hValue = estimate;
        
        if (updateGraphic == true)
            draw();
    }
    
    ///Resets all information
    void reset() {
        for (unsigned int v = 0; v < graph.numVertices(); v++) {
            vertexInfos[v].status = VertexStatus::UnknownVertex;
        }
        std::map<EdgeT, EdgeInformation>::iterator it;
        for (it = edgeInfos.begin(); it != edgeInfos.end(); it++) {
            it->second.status = EdgeStatus::UnknownEdge;
        }
        
        draw();
    }
    
    
    void draw() override {
        window.clear(sf::Color::Black);
        drawVertices();
        drawEdges();
        window.display();
    }
    
private:
    /// Returns the point on the line when traversing it from ratio=0 to ratio=1.
    sf::Vector2f lineBetween(VertexT v1, VertexT v2, double ratio) {
        return sf::Vector2f(screenCoordinates[v1].first+ratio*(screenCoordinates[v2].first-screenCoordinates[v1].first),
                            screenCoordinates[v1].second+ratio*(screenCoordinates[v2].second-screenCoordinates[v1].second)
                            );
    }
    
    
    void drawVertices() {
        for (unsigned int v = 0; v < graph.numVertices(); v++) {
            switch (vertexInfos[v].status) {
                case VertexStatus::UnknownVertex:
                    drawVertex(v, sf::Color(100, 100, 100)); //Unknown: Grey
                    break;
                case VertexStatus::Destination:
                    drawVertex(v, sf::Color(60, 30, 114));   //Destination: Violett
                    break;
                case VertexStatus::Active:
                    drawVertex(v,sf::Color(220, 40, 40));   //Active: Red
                    break;
                case VertexStatus::InQueue:
                    drawVertex(v,sf::Color(110, 210, 250));   //In Queue: Blue
                    break;
                case VertexStatus::Done:
                    drawVertex(v,sf::Color(255, 255, 255));   //Done: White
                    break;

                default:
                    break;
            }
        }
    }
    
    // No logic - just drawing
    void drawVertex(VertexT v, sf::Color color) {
        sf::CircleShape shape(10);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setFillColor(color);   //Done: White
        
        sf::Text text;
        text.setFont(font);

        shape.setPosition(screenCoordinates[v].first,screenCoordinates[v].second);
        window.draw(shape);
        
        //Draw text AFTERWARDS
        if ( SHOULD_DISPLAY_TEXT ) {
            text.setString(std::to_string(vertexInfos[v].gValue) + "\n" + std::to_string(vertexInfos[v].hValue));
            text.setCharacterSize(12); // in pixels, not points!
            text.setOrigin(0, 0);
            text.setPosition(shape.getPosition());
            text.setFillColor(color);
            window.draw(text);
        }
    }
    
    
    
    
    void drawEdges() {
        for (auto const& keyValuePair : edgeInfos) {
            EdgeT edge = keyValuePair.first;
            EdgeInformation info = keyValuePair.second;

            
            switch (info.status) {
                case EdgeStatus::UnknownEdge:
                    drawEdge(edge.first, edge.second, sf::Color(100, 100, 100), info.cost); //Unknown: Grey
                    break;
                case EdgeStatus::Active:
                    drawEdge(edge.first, edge.second, sf::Color(110, 210, 250), info.cost); //Active: Light blue
                    break;
                case EdgeStatus::Visited:
                    drawEdge(edge.first, edge.second, sf::Color::White, info.cost);         //Visited: White
                    break;
                case EdgeStatus::Optimal:
                    drawEdge(edge.first, edge.second, sf::Color::Green, info.cost);         //Optimal: Green
                default:
                    break;
            }
            
        }
    }
    
    // No logic - just drawing
    void drawEdge(VertexT from, VertexT to, sf::Color color, double cost) {
        sf::Text text;
        text.setFont(font);
        
        sf::CircleShape triangle(5, 3);

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = lineBetween(from,to,0);
        line[0].color = color;
        line[1].position = lineBetween(from,to,1);
        line[1].color = color;
        window.draw(line);
        
        triangle.setOrigin(triangle.getRadius(), triangle.getRadius());
        triangle.setFillColor(color);
        triangle.setPosition(lineBetween(from,to,0.8));
        
        
        window.draw(triangle);

        //Draw text AFTERWARDS
        if ( SHOULD_DISPLAY_COSTS ) {
            text.setString(std::to_string(cost));
            text.setCharacterSize(10); // in pixels, not points!
            text.setOrigin(0, 0);
            text.setPosition(lineBetween(from,to,0.5));
            text.setFillColor(sf::Color::Red);
            window.draw(text);
        }
    }
    
    
    
    
};
