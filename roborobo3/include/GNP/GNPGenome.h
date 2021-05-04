//
//  Genome.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef GNPGenome_h
#define GNPGenome_h

#include <stdio.h>
#include <vector>
#include "Network.h"
#include "NeuralNetwork.h"

namespace GNP {

class Genome{

private:
    std::vector<Node> _nodes;
    std::vector<int> _nodeUsage;
    std::vector<std::vector<Connection>> _connections;
    std::vector<std::vector<int>> _connectionUsage;
    
    double _fitness;
    static int getRandomNode(int i, int nbNodes);
    void initUsage();

public:
    
    Genome(std::vector<Node> nodes, std::vector<std::vector<Connection>> connections);
    void setFitness(double fitness);
    double getFitness();
    Genome mutate();
    Genome simpleMutate(); // only mutate transitioned connections.
    Genome processingNodeMutate();
    Genome* uniformMutation(double probability);
    std::vector<Genome> crossover(Genome& genome);
    std::vector<Genome> simpleCrossover(Genome& genome);
    void adjustFitness();
    Network* buildNetwork();
    void printUsage();
    void reset(); // removes usage statistics and fitness
    
    static Genome createGenome(int nbProcessingNodes, std::vector<int> judgementNodesOutput, int processT, int judgeT, int connectionT, int nbEachProcessingNode, int nbEachJudgementNode, int nbNeatNodes);
    
};
}

#endif /* GNPGenome_h */
