//
//  Header.h
//  roborobo3
//
//  Created by Truls Stenrud on 09/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include "common.h"
namespace GNP{

enum NodeType:int{Start=0, Judgement=1, Processing=2, NEAT=3};

struct NodeInformation{
    int nbProcessingNodes;
    int nbVariableProcessingNodes;
    std::vector<int> judgementNodeOutputs;
};

struct Node{
    NodeType type;
    int index;
    int T; // time cost
    double v; //value

    Node(NodeType type, int index, int T, double v = -1){
        this->type = type;
        this->index = index;
        this->T = T;
        this->v = v;
    }
};
struct Connection{
    int node;
    int T;

    Connection(int node, int T){
        this->node = node;
        this->T = T;
    }
};
}
#endif /* Header_h */
