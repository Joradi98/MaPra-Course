#include <vector>
#include <iostream>
#include <fstream>

typedef struct element {
    int profit, cost, lastMoved;
    bool used;
} element;

std::ostream& operator << (std::ostream& o, const element& e){
    o << "[profit: " << e.profit << ", cost: " << e.cost << ", lastMoved: " << e.lastMoved << ", used: " << e.used << "]";
    return o;
}

bool operator==(const element& e1, element& e2){
    return e1.profit == e2.profit && e1.cost == e2.cost && e1.lastMoved == e2.lastMoved && e1.used == e2.used;
}

int addElement(std::vector<element>& universe, std::vector<element>& solution, int& currentSolutionValue, int capacity, int tabuLength, int currentIteration){
    int bestElementIndex = -1;
    std::vector<element>::iterator it;
    for(int i = 0; i < universe.size(); i++){
        if(!universe[i].used && universe[i].lastMoved + tabuLength <= currentIteration){
            if(currentSolutionValue + universe[i].cost <= capacity){
                if(bestElementIndex == -1 || universe[i].profit > universe[bestElementIndex].profit || (universe[i].profit == universe[bestElementIndex].profit && universe[i].cost < universe[bestElementIndex].cost)){
                    bestElementIndex = i;
                }
            }
        }
    }
    if(bestElementIndex > -1){
        std::cout << "adding: " << universe[bestElementIndex] << std::endl;
        universe[bestElementIndex].used = true;
        universe[bestElementIndex].lastMoved = currentIteration;
        solution.push_back(universe[bestElementIndex]);
        currentSolutionValue += universe[bestElementIndex].cost;
        return bestElementIndex;
    }
    return -1;
}

int removeElement(std::vector<element>& solution, int& currentSolutionValue, int tabuLength, int currentIteration){
    int worstElementIndex = -1;
    std::vector<element>::iterator it;
    for(int i = 0; i < solution.size(); i++){
        if(solution[worstElementIndex].lastMoved + tabuLength <= currentIteration){
            if(worstElementIndex == -1 || solution[worstElementIndex].profit < solution[worstElementIndex].profit || (solution[worstElementIndex].profit == solution[worstElementIndex].profit && solution[worstElementIndex].cost > solution[worstElementIndex].cost)){
                worstElementIndex = i;
            }
        }
    }
    if(worstElementIndex > -1){
        std::cout << "erasing: " << solution[worstElementIndex] << std::endl;
        solution[worstElementIndex].used = false;
        solution[worstElementIndex].lastMoved = currentIteration;
        solution.erase(solution.begin() + worstElementIndex);
        currentSolutionValue -= solution[worstElementIndex].cost;
        return worstElementIndex;
    }
    return -1;
}

std::vector<element> tabuSearch(std::vector<element>& universe, int capacity, int iterations, int tabuLength){
    std::vector<element> currentSolution;
    std::vector<element> currentOptimalSolution;
    int currentSolutionValue = 0;
    int currentOptimalSolutionValue = 0;
    for(int i = 0; i < iterations; i++){
        std::cout << "iteration " << i << ".. solution value: " << currentSolutionValue << std::endl;
        int index = addElement(universe, currentSolution, currentSolutionValue, capacity, tabuLength, i);
        if(index == -1){
            index = removeElement(currentSolution, currentSolutionValue, tabuLength, i);
        }
        if(currentSolutionValue >= currentOptimalSolutionValue){
            currentOptimalSolutionValue = currentSolutionValue;
            currentOptimalSolution.clear();
            std::vector<element>::iterator it;
            for(it = currentSolution.begin(); it != currentSolution.end(); it++){
                currentOptimalSolution.push_back(*it);
            }
        }
    }
    return currentOptimalSolution;
}

template<typename T>
void outputVector(std::vector<T> vector){
    for(int i = 0; i < vector.size(); i++){
        std::cout << vector[i] << ", ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]){
    std::string fileName = argv[1];
    int tabuLength = std::stoi(argv[2]);
    int iterations = std::stoi(argv[3]);
    int outputFrequency = std::stoi(argv[4]);
    std::ifstream stream;
    stream.open(fileName);
    int numElements, capacity;
    stream >> numElements >> capacity;
    std::vector<element> universe;
    for(int i = 0; i < numElements; i++){
        element e;
        stream >> e.profit >> e.cost;
        e.lastMoved = -tabuLength;
        e.used = false;
        universe.push_back(e);
    }
    std::vector<element> solution = tabuSearch(universe, capacity, iterations, tabuLength);
    outputVector(solution);
    return 0;
}
