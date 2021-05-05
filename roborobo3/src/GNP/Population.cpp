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
        _genes.push_back(Genome::createGenome(nodeInformation.nbProcessingNodes,  nodeInformation.judgementNodeOutputs, _params->processT, _params->judgeT, _params->connectionT, _params->neatT, _params->nbEachProcessingNode, _params->nbEachJudgementNode, _params->nbNEATNodes));
    }
}


void Population::Epoch(){


    simpleOperators2();

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
    std::vector<Genome> parents;
    std::vector<Genome> mutations;
    std::vector<Genome> genesCopy = _genes;

    parents.push_back(getBest(genesCopy)); // elitism

    int nbMutations = _params->populationSize * _params->mutationRate;

    while(parents.size() < _params->nbParents){
        parents.push_back(tournementSelection(genesCopy, _params->tournamentSize));
    }
    while(mutations.size() < nbMutations){
        mutations.push_back(tournementSelection(_genes, _params->tournamentSize).mutate());
    }
    
    parents.insert(parents.end(), mutations.begin(), mutations.end());

    while(parents.size() < _params->populationSize){
        int pA = random01() * _params->nbParents;
        int pB = random01() * _params->nbParents;
        while (pA == pB){
            pB = random01() * _params->nbParents;
        }

        auto offspsring = parents[pA].simpleCrossover(parents[pB]);

        parents.push_back(offspsring[0]);

        if(parents.size() < _params->populationSize){
            parents.push_back(offspsring[1]);
        }
    }

    _genes = parents;
}

void Population::simpleOperators2(){
    double sum = 0;
    for(auto& gene : _genes){
        sum+=pow(gene.getFitness(), 1);
    }
    std::cout << "sum " << sum << std::endl;
    
    

    std::vector<Genome> result;
    
    result.push_back(getBest(_genes)); // elitism
    
    while(result.size() < _params->populationSize){
        Genome geneA = tournementSelection(_genes, _params->tournamentSize);
        Genome geneB = tournementSelection(_genes, _params->tournamentSize);
        
        std::vector<Genome> toAdd;
        if(random01() < _params->crossoverRate){
            auto offspring = geneA.crossover(geneB);
            toAdd.insert(toAdd.end(), offspring.begin(), offspring.end());
        }
        
        toAdd.push_back(random01() < _params->mutationRate ? geneA.mutate() : geneA);
        toAdd.push_back(random01() < _params->mutationRate ? geneB.mutate() : geneB);
    
        while(!toAdd.empty() && result.size() < _params->populationSize){
            result.push_back(getRandom(toAdd));
        }
    }
    
    _genes = result;
}

void Population::doProbabilitySelection(){

    std::vector<Genome> parents;

    for(Genome& gene : _genes){
        gene.adjustFitness();
    }

    std::vector<double> probabilityFitness;

    std::sort(_genes.begin(), _genes.end(), genome_greater);

    double power = 1;
    double t = 0;
    double sum = 0;
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

        auto offspsring = parents[pA].crossover(parents[pB]);

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
