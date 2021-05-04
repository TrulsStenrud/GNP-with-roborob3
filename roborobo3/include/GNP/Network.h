//
//  Network.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 08/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//
#pragma once

#include <stdio.h>
#include "vector"
#include "GNPStructs.h"

class GNPController;

namespace GNP{

class Network{
private:
    std::vector<Node> _nodes;
    std::vector<int>& _nodeUsage;
    std::vector<std::vector<Connection>> _connections;
    std::vector<std::vector<int>>& _connectionUsage;
    int _currentNode = 0;
public:
    
    Network(std::vector<Node> nodes, std::vector<std::vector<Connection>> connections, std::vector<int>& nodeUsage, std::vector<std::vector<int>>& connectionUsage);
    
    void step(GNPController* controller);
};

}

