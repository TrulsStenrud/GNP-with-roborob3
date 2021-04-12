//
//  Population.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "GNPPopulation.h"
#include "Utilities/Misc.h"


namespace GNP
{

Population::Population(NodeInformation nodeInformation, Parameters* params){
    _params = params;
    _nodeInformation = nodeInformation;
    
    for(int i = 0; i < _params->populationSize; i++){
        _genes.push_back(Genome(nodeInformation.nbProcessingNodes,  nodeInformation.judgementNodeOutputs));
    }
}

bool genome_greater(Genome& ls, Genome& rs)
{
    return (ls.getFitness() > rs.getFitness());
}

void Population::Epoch(){
    std::sort(_genes.begin(), _genes.end(), genome_greater);
    
    std::vector<Genome> parents;
    
    std::vector<double> probabilityFitness;

    for(Genome& gene : _genes){
        gene.adjustFitness();
    }
    
    double power = 1;
    double sum = 0;
    double t = 0;
    for(auto gene : _genes){
        sum+=pow(gene.getFitness(), power);
    }
    std::cout << "sum " << sum << std::endl;
    for(auto gene : _genes){
        t += pow(gene.getFitness(), power);
        probabilityFitness.push_back(t);
    }
    
    parents.push_back(_genes[0]); //elitism
    
    
    while(parents.size() < _params->nbParents){
        double r = random01();
        for(int i = 0; i < _genes.size(); i++){
            if (probabilityFitness[i] > r) {
                    parents.push_back(_genes[i]);
                break;
            }
        }
    }
    
    while(parents.size() < _params->populationSize){
        int pA = random01() * _params->nbParents;
        int pB = random01() * _params->nbParents;
        while (pA == pB){
            pB = random01() * _params->nbParents;
        }
        
        auto offspsring = _genes[pA].crossover(_genes[pB]);
        
        if(random01() > _params->mutationRate)
            parents.push_back(offspsring[0]);
        else
            parents.push_back(offspsring[0].mutate());
        
        if(parents.size() < _params->populationSize){
            if(random01() > _params->mutationRate)
                parents.push_back(offspsring[1]);
            else
                parents.push_back(offspsring[1].mutate());
        }
    }
    
    
    _genes = parents;
}


Genome& Population::AccessGenomeByIndex(int const index){
    return _genes[index];
}



}
