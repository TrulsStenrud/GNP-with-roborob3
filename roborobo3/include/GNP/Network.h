//
//  Network.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 08/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include <stdio.h>
#include "vector"

namespace GNP{

class Network{
private:
    std::vector<std::function<void(double)>>* _processes;
    std::vector<std::function<double()>>* _judgements;
    std::vector<std::vector<int>> _nodes;
    std::vector<std::vector<std::vector<int>>> _connections;
    int _currentNode = 0;
public:
    Network(std::vector<std::function<void(double)>>* processes, std::vector<std::function<double()>>* judgements, std::vector<std::vector<int>> nodes, std::vector<std::vector<std::vector<int>>> connections);
    void step();
};

}

#endif /* Network_hpp */
