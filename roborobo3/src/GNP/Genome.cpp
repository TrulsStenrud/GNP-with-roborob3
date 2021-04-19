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

Genome::Genome(int nbProcessingNodes, std::vector<int> judgementNodesOutput, int processT, int judgeT, int connectionT, int nbEachNode){
    _nbProcessingNodes = nbProcessingNodes;
    _judgementNodesOutput = judgementNodesOutput;
    
    
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
    
    initUsage();
}

Genome::Genome(std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections, int nbProcessingNodes, std::vector<int> judgementNodesOutput){
    
    _nbProcessingNodes = nbProcessingNodes;
    _judgementNodesOutput = judgementNodesOutput;
    _nodes = nodes;
    _connections = connections;
    
    initUsage();
}

void Genome::initUsage(){
    _nodeUsage.clear();
    assert(_nodeUsage.size() == 0);
    _nodeUsage = std::vector<int>(_nodes.size(), 0);
    
    _connectionUsage.clear();
    for(auto connections : _connections){
        _connectionUsage.push_back(std::vector<int>(connections.size(), 0));
    }
}

void Genome::adjustFitness(){
    if(_fitness == 0){
        _fitness = 0.0001;
    }
}

// TODO, figure out how to mutate, uniform probability, or one random connection
Genome Genome::mutate(){
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
            break;
        }
        else{
            index += _connections[i].size();
        }
    }
    
    return Genome(_nodes, newConnections, _nbProcessingNodes, _judgementNodesOutput);
}
struct Point{
    const int x;
    const int y;
    Point(const int x1, const int y1):x(x1), y(y1){}
};

Genome Genome::simpleMutate(){
    
    std::vector<Point> usedConnections;
    for(int i = 0; i < _connectionUsage.size(); i++){
        for(int j = 0; j < _connectionUsage[i].size(); j ++){
            if(_connectionUsage[i][j]>0){
                usedConnections.push_back(Point(i, j));
            }
        }
    }
    
    int mutateConnection = randint() % usedConnections.size();
    Point mCon = usedConnections[mutateConnection];
    
    auto newConnections = _connections;
    
    int randomNode = getRandomNode(mCon.x);
    newConnections[mCon.x][mCon.y][0] = randomNode;
    
    return Genome(_nodes, newConnections, _nbProcessingNodes, _judgementNodesOutput);
}

Genome* Genome::uniformMutation(double probability){
   
    auto newConnections = _connections;
    
    for(int i = 0; i < _connections.size(); i++){
        for(int j = 0; j < _connections[i].size(); j++){
            if(random01() > probability){
                int randomNode = getRandomNode(i);
                newConnections[i][j][0] = randomNode;
            }
        }
    }
    
    return new Genome(_nodes, newConnections, _nbProcessingNodes, _judgementNodesOutput);
}

std::vector<Genome> Genome::crossover(Genome& genome){
    
    int nbConnections = 0;
    for(int i = 0; i < _connections.size(); i++){ // i = 1 to skip start node;
        nbConnections += _connections[i].size();
    }
    
    int crossoverConnection = randint() % nbConnections;
    auto o1Connections = _connections;
    auto o2Connections = genome._connections;
    
    int index = 0;
    for(int i = 0; i < _connections.size(); i++){
        if(index + _connections[i].size() > crossoverConnection){
            int temp = o1Connections[i][crossoverConnection - index][0];
            o1Connections[i][crossoverConnection - index][0] = o2Connections[i][crossoverConnection - index][0];
            o2Connections[i][crossoverConnection - index][0] = temp;
            break;
        }
        else{
            index += _connections[i].size();
        }
    }
    
    return {
        Genome(_nodes,  o1Connections, _nbProcessingNodes, _judgementNodesOutput),
        Genome(_nodes,  o2Connections, _nbProcessingNodes, _judgementNodesOutput)
    };
}

std::vector<Genome> Genome::simpleCrossover(Genome& genome){
    
    std::vector<Point> usedConnections;
    for(int i = 0; i < _connectionUsage.size(); i++){ // i = 1 to skip start node;
        for(int j = 0; j < _connectionUsage[i].size(); j++){
            if(_connectionUsage[i][j] > 0
               || genome._connectionUsage[i][j] > 0){
                usedConnections.push_back(Point(i, j));
            }
        }
    }
    
    Point crossCon = usedConnections[randint() % usedConnections.size()];
    auto o1Connections = _connections;
    auto o2Connections = genome._connections;
    
    int temp = o1Connections[crossCon.x][crossCon.y][0];
    o1Connections[crossCon.x][crossCon.y][0] = o2Connections[crossCon.x][crossCon.y][0];
    o2Connections[crossCon.x][crossCon.y][0] = temp;
    
    
    return {
        Genome(_nodes,  o1Connections, _nbProcessingNodes, _judgementNodesOutput),
        Genome(_nodes,  o2Connections, _nbProcessingNodes, _judgementNodesOutput)
    };
}

Network* Genome::buildNetwork(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements){
    return new Network(processes, judgements, _nodes, _connections, _nodeUsage, _connectionUsage);
}

void Genome::reset(){
    setFitness(0);
    initUsage();
}

double Genome::getFitness(){
    return _fitness;
}

void Genome::setFitness(double fitness){
    _fitness = fitness;
}

}

