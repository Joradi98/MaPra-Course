#pragma once


#include "unit.h"
#include <iostream>
#include "MazeGraph.h"
#include <SFML/Graphics.hpp>

///A struct storing information relevant for drawing a vertex
struct VertexInformation {
    VertexStatus status;
    double gValue;
    double hValue;
};


class MazeVisualizer : public GraphVisualizer
{
private:
    sf::RenderWindow window;
    MazeGraph graph;
    std::vector<VertexInformation> vertices;    /// Drawable information for each vertex
    std::vector<EdgeT> markedEdges;             /// Edges that should be highlighted (i.e. the successfull path at the end)
    
public:

    /**
    Initialize for a given window size
     */
    MazeVisualizer(int width, int height, MazeGraph maze): graph(maze) {
        window.create(sf::VideoMode(width, height), "Maze visualizer");
        for (unsigned int i = 0; i < maze.numVertices(); i++) {
            vertices.push_back(VertexInformation());
            vertices[i].status = VertexStatus::UnknownVertex;
            vertices[i].gValue = 0.0;
            vertices[i].hValue = 0.0;
        }
        draw();

        
    }


    /**
     Keep the window open until the user closes it
     */
    void keepRunning() {
        while (window.isOpen()) {
            
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event)) // event loop
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }

    

    
    //hold reference to graoh and costs, and whatever
    
    //implement draw() method
     void draw() {
         //reset to black
         //window.clear(sf::Color::Black);

         drawVertices();
         
         //draw edges?
         drawEdges();
         //draw path?

         window.display();
    }
    
    
    void markVertex(VertexT vertex, VertexStatus status) override {
        vertices[vertex].status = status;
        draw();
    }
    
    void markEdge(EdgeT e, EdgeStatus status) override {
        
        if ( status == EdgeStatus::Optimal ) {
            markedEdges.push_back(e);
        }
        draw();
    }
    
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override {
        vertices[vertex].status = status;
        vertices[vertex].gValue = cost;
        vertices[vertex].hValue = estimate;
        draw();
    }
    

private:
    
    std::pair<double,double> centerOfVertex(VertexT i) {
        double singleWidth = window.getSize().x/graph.width;
        double singleHeight = window.getSize().y/graph.height;
        double x = singleWidth*(i%graph.width+0.5);
        double y = singleHeight*(i/graph.width+0.5);
        
        return std::make_pair (x,y);
    }
    
    ///Draws the edges from markedEdges
    void drawEdges() {
        sf::RectangleShape shape;

        for (int i = 0; i < markedEdges.size(); i++) {
            //Draw a red line
            std::pair<double,double> pos1 = centerOfVertex(markedEdges[i].first);
            std::pair<double,double> pos2 = centerOfVertex(markedEdges[i].second);

            shape.setPosition(pos1.first, pos1.second);
            shape.setSize(sf::Vector2f(pos2.first-pos1.first + 5, pos2.second-pos1.second + 5));
            shape.setFillColor(sf::Color(220, 40, 40));   //Active: Red

            shape.setOutlineThickness(1); // creates an outline around the circle
            shape.setFillColor(sf::Color(220, 40, 40));   //Active: Red

            //Display
            window.draw(shape);

        }
        
        
    }
    
    
    
    void drawVertices() {
        //Create a rectangle for each node
        sf::RectangleShape shape;
        double singleWidth = window.getSize().x/graph.width;
        double singleHeight = window.getSize().y/graph.height;
        
        shape.setSize(sf::Vector2f(singleWidth, singleHeight));
        for (int i = 0; i < graph.height; i++) {
            for (int j = 0; j < graph.width; j++) {
                int vertexIndex = j+graph.height*i;
                
                //Place it at the correct point. (Counting from upper left to lwer right?)
                shape.setOrigin(0, 0);
                shape.setPosition(singleWidth*j,singleHeight*i); // positioning the circle 200 units right from the top left corner

                //Distinguish (wall vs ground)
                if (graph.typeOfCell(vertexIndex) == CellType::Wall ) {
                    shape.setFillColor(sf::Color(0, 0, 0));         //Walls: Black
                } else {
               //     std::cout << vertices.size() << std::endl;
                    //Distinguish VertexStatus (UknownVertex vs Active vs Done etc)
                    switch ( vertices[vertexIndex].status ) {
                        case VertexStatus::Destination:
                            shape.setFillColor(sf::Color(60, 30, 114));   //Destination: Violett
                            break;
                        case VertexStatus::UnknownVertex:
                            shape.setFillColor(sf::Color(100, 100, 100));   //UnknownVertex: grey
                            break;
                        case VertexStatus::Active:
                            shape.setFillColor(sf::Color(220, 40, 40));   //Active: Red
                            break;
                        case VertexStatus::InQueue:
                            shape.setFillColor(sf::Color(110, 210, 250));   //In Queue: Blue
                            break;
                        case VertexStatus::Done:
                            shape.setFillColor(sf::Color(255, 255, 255));   //Done: White
                            break;

                        default:
                            break;
                    }
                }
                
                
                window.draw(shape);
            }
        }

    }
    
};
