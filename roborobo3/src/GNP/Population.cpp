//
//  Population.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "GNPPopulation.h"


namespace GNP
{

Population::Population(NodeInformation nodeInformation, Parameters* params){
    _params = params;
    _nodeInformation = nodeInformation;
    
    for(int i = 0; i < _params->populationSize; i++){
        _genes.push_back(Genome(nodeInformation.nbProcessingNodes,  nodeInformation.judgementNodeOutputs));
    }
}

struct less_than_key
{
    inline bool operator() ( Genome& x,  Genome& y)
    {
        return (x.getFitness() < y.getFitness());
    }
};

bool genome_greater(Genome& ls, Genome& rs)
{
    return (ls.getFitness() > rs.getFitness());
}

void Population::Epoch(){
    std::sort(_genes.begin(), _genes.end(), genome_greater);
    
}


Genome& Population::AccessGenomeByIndex(int const index){
    
    Genome* a;
    return *a;
}



}
