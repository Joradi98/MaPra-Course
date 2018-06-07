#include <vector>
#include <iostream>
#include <fstream>

typedef struct element {
    int profit, cost, lastMoved;
    bool used;
} element;

std::ostream& operator << (std::ostream& o, const element& e){
    //o << "[profit: " << e.profit << ", cost: " << e.cost << ", lastMoved: " << e.lastMoved << ", used: " << e.used << "]";
    o << "[" << e.profit << ", " << e.cost << "]";
    return o;
}

std::vector<element>& tabuSearch(std::vector<element>& universe, std::vector<element>& solution, int capacity, int iterations, int tabuLength){
    int currentSolutionCost = 0;
    int currentSolutionValue = 0;
    int currentOptimumSolutionMask[universe.size()];
    int currentOptimumSolutionValue = 0;
    std::cout << "running tabu search with data: " << std::endl;
    std::cout << "> capacity: " << capacity << std::endl;
    std::cout << "> iterations: " << iterations << std::endl;
    std::cout << "> tabuLength: " << tabuLength << std::endl;

    //
    currentSolutionCost = 20;
    currentSolutionValue = 35;
    universe[1].lastMoved = -4;
    universe[1].used = true;
    universe[2].lastMoved = -4;
    universe[2].used = true;
    universe[3].lastMoved = -4;
    universe[3].used = true;
    //

    for(int i = 0; i < iterations; i++){
        std::cout << "(i = " << i << ") >> ";
        // add element
        int maxElementIndex = -1;
        for(int j = 0; j < universe.size(); j++){
            if(!universe[j].used && universe[j].lastMoved + tabuLength < i && currentSolutionCost + universe[j].cost <= capacity){
                //std::cout << std::endl;
                //std::cout << ">> possible element to add: " << j;
                //std::getchar();
                if(maxElementIndex == -1 || universe[j].profit > universe[maxElementIndex].profit || (universe[j].profit == universe[maxElementIndex].profit && universe[j].cost < universe[maxElementIndex].cost)){
                    maxElementIndex = j;
                }
            }
        }
        if(maxElementIndex == -1){
            // remove element
            int minElementIndex = -1;
            for(int j = 0; j < universe.size(); j++){
                if(universe[j].used && universe[j].lastMoved + tabuLength < i){
                    if(minElementIndex == -1 || universe[j].profit < universe[minElementIndex].profit || (universe[j].profit == universe[minElementIndex].profit && universe[j].cost > universe[minElementIndex].cost)){
                        minElementIndex = j;
                    }
                }
            }
            if(minElementIndex != -1){
                std::cout << "removing element " << minElementIndex << " so " << universe[minElementIndex];
                currentSolutionCost -= universe[minElementIndex].cost;
                currentSolutionValue -= universe[minElementIndex].profit;
                universe[minElementIndex].lastMoved = i;
                universe[minElementIndex].used = false;
            }
        }
        else {
            std::cout << "adding element " << maxElementIndex << " so " << universe[maxElementIndex];
            currentSolutionCost += universe[maxElementIndex].cost;
            std::cout << " cost now: " << currentSolutionCost;
            currentSolutionValue += universe[maxElementIndex].profit;
            universe[maxElementIndex].lastMoved = i;
            universe[maxElementIndex].used = true;
        }
        if(currentSolutionValue > currentOptimumSolutionValue){
            for(int i = 0; i < universe.size(); i++){
                currentOptimumSolutionMask[i] = universe[i].used;
                currentOptimumSolutionValue = currentSolutionValue;
            }
        }
        std::cout << " | cost: " << currentSolutionCost << ", value: " << currentSolutionValue;
        std::cout << std::endl;
    }
    std::cout << "optimum: ";
    for(int i = 0; i < universe.size(); i++){
        std::cout << currentOptimumSolutionMask[i];
        if(currentOptimumSolutionMask[i]){
            solution.push_back(universe[i]);
        }
    }
    std::cout << std::endl;
    return solution;
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
        e.lastMoved = -4;
        e.used = false;
        universe.push_back(e);
    }
    stream.close();
    std::vector<element> solution;
    tabuSearch(universe, solution, capacity, iterations, tabuLength);
    //outputVector(solution);
    return 0;
}
