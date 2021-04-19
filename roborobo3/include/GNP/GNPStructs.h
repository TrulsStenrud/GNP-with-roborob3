//
//  Header.h
//  roborobo3
//
//  Created by Truls Stenrud on 09/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Header_h
#define Header_h

namespace GNP{

enum NodeType:int{Start=0, Judgement=1, Processing=2};

struct NodeInformation{
    int nbProcessingNodes;
    std::vector<int> judgementNodeOutputs;
};

struct Node{
    NodeType type;
    int index;
    int T;
    
    Node(NodeType type, int index, int T){
        this->type = type;
        this->index = index;
        this->T = T;
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
