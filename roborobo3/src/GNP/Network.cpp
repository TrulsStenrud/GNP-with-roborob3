//
//  Network.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 08/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//
#include "common.h"
#include "Network.h"
#include "GNPController.h"

namespace GNP{



Network::Network(std::vector<Node> nodes, std::vector<std::vector<Connection>> connections, std::vector<int>& nodeUsage, std::vector<std::vector<int>>& connectionUsage): _connectionUsage(connectionUsage), _nodeUsage(nodeUsage){
    
    
    _nodes = nodes;
    _connections = connections;
}

void Network::reset(){
    _currentNode = 0;
}

void Network::step(GNPController* controller){
    int maxCost = 5; // TODO set somewhere else
    int cost = 0;
    
    while (cost < maxCost) {
        cost+=_nodes[_currentNode].T; //add cost of current node
        int nextConnection;
        
        switch(_nodes[_currentNode].type){
            case NodeType::Start:
            {
                nextConnection = 0;
                
            }break;
                
            case NodeType::Judgement:
            {
                auto judgeIndex = _nodes[_currentNode].index;
                nextConnection = controller->judge(judgeIndex);
                
            }break;
                
            case NodeType::Processing:
            {
                auto processIndex = _nodes[_currentNode].index;
                controller->process(processIndex, _nodes[_currentNode].v);
                
                nextConnection = 0;
            }break;
                
            case NodeType::NEAT:
            {
                auto neatIndex = _nodes[_currentNode].index;
                controller->processNeat(neatIndex);
                
                nextConnection = 0;
            }break;
                
            default:
                std::cout << "[ERROR] Unknown node type [" << _nodes[_currentNode].type << "]" << std::endl;
                exit(-1);
        }
        
        // update node and connection usage
        _nodeUsage[_currentNode]++;
        _connectionUsage[_currentNode][nextConnection]++;
        
        cost += _connections[_currentNode][nextConnection].T; //add cost of traversing this connection
        _currentNode = _connections[_currentNode][nextConnection].node;
    }
    
}

}
