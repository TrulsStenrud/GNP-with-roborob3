//
//  Genome.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Genome_hpp
#define Genome_hpp

#include <stdio.h>
#include <vector>
#include "Network.h"

namespace GNP {

enum NodeType:int{Start=0, Judgement=1, Processing=2};

class Genome{
    
private:
    std::vector<std::vector<int>> _nodes;
    std::vector<std::vector<std::vector<int>>> _connections;
    std::vector<int> _judgementNodesOutput;
    int _nbProcessingNodes;
    double _fitness;
    int getRandomNode(int i);
    
public:
    int extracted(int i);
    
    Genome(int nbProcessingNodes, std::vector<int> judgementNodesOutput);
    Genome(std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections, int nbProcessingNodes, std::vector<int> judgementNodesOutput);
    void setFitness(double fitness);
    double getFitness();
    Genome mutate();
    Genome* uniformMutation(double probability);
    std::vector<Genome> crossover(Genome& genome);
    Network* buildNetwork(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements);
    
};
}

#endif /* Genome_hpp */
