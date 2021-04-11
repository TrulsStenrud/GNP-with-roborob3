//
//  Network.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 08/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "Network.h"
#include "GNPEvolver.h"

namespace GNP{



Network::Network(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements, std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections){
    
    _processes = processes;
    _judgements = judgements;
    _nodes = nodes;
    _connections = connections;
}

void Network::step(){
    int maxCost = 15; // TODO set somewhere else
    int cost = 0;
    
    while (cost < maxCost) {
        cost+=_nodes[_currentNode][2]; //add cost of current node
        int nextConnection;
        
        switch(_nodes[_currentNode][0]){
            case NodeType::Start:
            {
                
                nextConnection = 0;
                
            }break;
                
            case NodeType::Judgement:
            {
                
                auto judgeIndex = _nodes[_currentNode][1];
                nextConnection = (*_judgements)[judgeIndex]();
                
            }break;
                
            case NodeType::Processing:
            {
                
                auto processIndex = _nodes[_currentNode][1];
                (*_processes)[processIndex](1);
                
                nextConnection = 0;
            }break;
            default:
                std::cout << "[ERROR] Unknown node type [" << _nodes[_currentNode][0] << "]" << std::endl;
                exit(-1);
        }
        _currentNode = _connections[_currentNode][nextConnection][0];
        cost+= _connections[_currentNode][nextConnection][1]; //add cost of traversing this connection
    }
    
}

}
