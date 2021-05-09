//
//  Population.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "../../include/GNP/GNPPopulation.h"
#include "Utilities/Misc.h"
#include <algorithm>

namespace GNP
{

Population::Population(NodeInformation nodeInformation, Parameters* params){
    _params = params;
    _nodeInformation = nodeInformation;

   
    
    
    for(int i = 0; i < _params->populationSize; i++){
        _genes.push_back(Genome::createGenome(nodeInformation, *_params));
    }
}


void Population::Epoch(){
    simpleOperators();

    for(Genome& gene : _genes){
        gene.reset();
    }
}


bool genome_greater(Genome& ls, Genome& rs)
{
    return (ls.getFitness() > rs.getFitness());
}

Genome tournementSelection(std::vector<Genome>& genes, int t){
    int currentBest = randint() % genes.size();

    for(int i = 1; i < t; i++){
        int candidate = randint() % genes.size();
        if(genes[candidate].getFitness() > genes[currentBest].getFitness()){
            currentBest = candidate;
        }
    }

    Genome result = genes[currentBest];
    genes.erase(genes.begin() + currentBest);

    return result;
}

int indexTournementSelection(std::vector<Genome>& genes, int t, int exclude = -1){
    int size = exclude == -1 ? genes.size() -1 : genes.size();
    
    int currentBest = randint() % size;

    for(int i = 1; i < t; i++){
        int candidate = randint() % size;
        if(candidate == exclude){
            candidate++;
        }
        if(genes[candidate].getFitness() > genes[currentBest].getFitness()){
            currentBest = candidate;
        }
    }
    return currentBest;
}

Genome getBest(std::vector<Genome>& genes){
    int currentBest = 0;

    for(int i = 1; i < genes.size(); i++){
        if(genes[i].getFitness() > genes[currentBest].getFitness()){
            currentBest = i;
        }
    }

    Genome result = genes[currentBest];
    genes.erase(genes.begin() + currentBest);

    return result;
}

Genome getRandom(std::vector<Genome>& genes){
    int r = randint() % genes.size();
    
    Genome result = genes[r];
    genes.erase(genes.begin() + r);

    return result;
}


void Population::simpleOperators(){
    double sum = 0;
    for(auto& gene : _genes){
        sum+=pow(gene.getFitness(), 1);
    }
    std::cout << "sum " << sum << std::endl;
    
    

    std::vector<Genome> result;
    
    result.push_back(getBest(_genes)); // elitism
    
    while(result.size() < _params->populationSize){
        int iA = indexTournementSelection(_genes, _params->tournamentSize);
        int iB = indexTournementSelection(_genes, _params->tournamentSize, iA);
        
        Genome geneA = _genes[iA];
        Genome geneB = _genes[iB];
        bool aModified = false;
        bool bModified = false;
        
        std::vector<Genome> toAdd;
        
        if(random01() < _params->crossoverRate){
            auto offspring = geneA.crossover(geneB);
            toAdd.insert(toAdd.end(), offspring.begin(), offspring.end());
            aModified = bModified = true;
        }
        
        if(random01() < _params->mutationRate){
            toAdd.push_back(geneA.mutate());
            aModified = true;
        }
        if(random01() < _params->mutationRate){
            toAdd.push_back(geneB.mutate());
            bModified = true;
        }
    
        if(!aModified){
            toAdd.push_back(geneA);
        }
        if(!bModified){
            toAdd.push_back(geneB);
        }
        
        
        while(!toAdd.empty() && result.size() < _params->populationSize){
            result.push_back(getRandom(toAdd));
        }
    }
    
    _genes = result;
}


Genome& Population::AccessGenomeByIndex(int const index){
    return _genes[index];
}



}
