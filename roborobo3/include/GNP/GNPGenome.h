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
    std::vector<int> _nodeUsage;
    std::vector<std::vector<std::vector<int>>> _connections;
    std::vector<std::vector<int>> _connectionUsage;
    std::vector<int> _judgementNodesOutput;
    int _nbProcessingNodes;
    double _fitness;
    int getRandomNode(int i);
    void initUsage();
    
public:
    int extracted(int i);
    
    Genome(int nbProcessingNodes, std::vector<int> judgementNodesOutput, int processT, int judgeT, int connectionT, int nbEachNode);
    Genome(std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections, int nbProcessingNodes, std::vector<int> judgementNodesOutput);
    void setFitness(double fitness);
    double getFitness();
    Genome mutate();
    Genome simpleMutate(); // only mutate transitioned connections. 
    Genome* uniformMutation(double probability);
    std::vector<Genome> crossover(Genome& genome);
    std::vector<Genome> simpleCrossover(Genome& genome);
    void adjustFitness();
    Network* buildNetwork(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements);
    
    void reset(); // removes usage statistics and fitness
    
};
}

#endif /* Genome_hpp */
