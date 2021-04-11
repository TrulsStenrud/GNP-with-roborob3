//
//  Genome.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "GNPGenome.h"
#include "Utilities/Misc.h"
namespace GNP{


int Genome::getRandomNode(int i) {
    int nextNode = randint() % (_nodes.size() - 2) + 1;
    if(nextNode == i){
        nextNode++;
    }
    return nextNode;
}

Genome::Genome(std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections, int nbProcessingNodes, std::vector<int> judgementNodesOutput){
    
    _nbProcessingNodes = nbProcessingNodes;
    _judgementNodesOutput = judgementNodesOutput;
    _nodes = nodes;
    _connections = connections;
}

Genome::Genome(int nbProcessingNodes, std::vector<int> judgementNodesOutput){
    _nbProcessingNodes = nbProcessingNodes;
    _judgementNodesOutput = judgementNodesOutput;
    
    
    int nbEachNode = 4;
    int processT = 10;
    int judgeT = 2;
    int connectionT = 0;
    
    // add start node
    _nodes.push_back({NodeType::Start, 0, 0});
    
    // add judgement nodes
    for(int i = 0; i < judgementNodesOutput.size(); i++){
        for(int j = 0; j < nbEachNode; j++){
            _nodes.push_back({NodeType::Judgement, i, judgeT});
        }
    }
    
    // add processing nodes
    for(int i = 0; i < _nbProcessingNodes; i++){
        for(int j = 0; j < nbEachNode; j++){
            _nodes.push_back({NodeType::Processing, i, processT});
        }
    }
    
    for(int i = 0; i < _nodes.size(); i++){
        
        switch(_nodes[i][0]){ //node type
            case NodeType::Start:
            {
                int firstNode = randint() % (_nodes.size() - 1) + 1;
                
                _connections.push_back({{firstNode,  connectionT}});
            }break;
                
            case NodeType::Judgement: // judgement node
            {
                std::vector<std::vector<int>> connections;
                int nbConnections = _judgementNodesOutput[_nodes[i][1]];
                
                for (int j = 0; j < nbConnections; j++){
                    int nextNode = getRandomNode(i);
                    connections.push_back({nextNode, connectionT});
                }
                
                _connections.push_back(connections);
                
            }break;
                
            case NodeType::Processing: // processing node
            {
                int nextNode = randint() % (_nodes.size() - 2) + 1;
                if(nextNode == i){
                    nextNode++;
                }
                _connections.push_back({{nextNode,  connectionT}});
            }break;
                
        }
        
    }
}

Genome* Genome::mutate(){
    int nbConnections = 0;
    for(int i = 0; i < _connections.size(); i++){ // i = 1 to skip start node;
        nbConnections += _connections[i].size();
    }
    
    int mutateConnection = randint() % nbConnections;
    auto newConnections = _connections;
    
    int index = 0;
    for(int i = 0; i < _connections.size(); i++){
        if(index + _connections[i].size() > mutateConnection){
            int randomNode = getRandomNode(i);
            newConnections[i][mutateConnection - index][0] = randomNode;
        }
        else{
            index += _connections[i].size();
        }
    }
    
    return new Genome(_nodes, newConnections, _nbProcessingNodes, _judgementNodesOutput);
}

Network* Genome::buildNetwork(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements){
    auto n = new Network(processes, judgements, _nodes, _connections);
    
    return n;
}


double Genome::getFitness(){
    return _fitness;
}

void Genome::setFitness(double fitness){
    _fitness = fitness;
}

}

