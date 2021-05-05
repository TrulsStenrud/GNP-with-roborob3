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


int Genome::getRandomNode(int i, int nbNodes) {
    int nextNode = randint() % (nbNodes - 2) + 1;
    if(nextNode == i){
        nextNode++;
    }
    return nextNode;
}

Genome::Genome(std::vector<Node> nodes, std::vector<std::vector<Connection>> connections){
    _nodes = nodes;
    _connections = connections;

    initUsage();
}

void Genome::printUsage(){
    int nbJ = 0, nbJ_Used = 0, nbP = 0, nbP_Used = 0;
    
    for(int i = 0; i < _nodeUsage.size(); i++){
        if(_nodes[i].type == NodeType::Processing){
            nbP++;
            if(_nodeUsage[i] > 0){
                nbP_Used++;
            }
        }
        else if(_nodes[i].type == NodeType::Judgement){
            nbJ++;
            if(_nodeUsage[i] > 0){
                nbJ_Used++;
            }
        }
    }
    
    std::cout << "Judgement used: " << ((double)nbJ_Used / nbJ)*100 << " Processes used: " << ((double)nbP_Used / nbP) * 100 << std::endl;
}

void Genome::initUsage(){
    _nodeUsage.clear();

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
//Genome Genome::mutate(){
//    int nbConnections = 0;
//    for(int i = 0; i < _connections.size(); i++){ // i = 1 to skip start node;
//        nbConnections += _connections[i].size();
//    }
//
//    int mutateConnection = randint() % nbConnections;
//    auto newConnections = _connections;
//
//    int index = 0;
//    for(int i = 0; i < _connections.size(); i++){
//        if(index + _connections[i].size() > mutateConnection){
//            int randomNode = getRandomNode(i);
//            newConnections[i][mutateConnection - index].node = randomNode;
//            break;
//        }
//        else{
//            index += _connections[i].size();
//        }
//    }
//
//    return Genome(_nodes, newConnections, _nbProcessingNodes, _judgementNodesOutput);
//}


Genome Genome::mutate(){

    int random = randint() % 2;

    switch(random){
        case 0:
            return simpleMutate();
            break;
        case 1:
            return processingNodeMutate();
            break;
        default:
            exit(-1);
    }
}

Genome Genome::processingNodeMutate(){
    std::vector<int> usedProcessingNodes;
    for(int i = 0; i < _nodeUsage.size(); i++){
        if(_nodes[i].type == NodeType::Processing && _nodeUsage[i] > 0){
            usedProcessingNodes.push_back(i);
        }
    }
    if(usedProcessingNodes.size() == 0){
        return (*this);
    }
    int mIndex = randint() % usedProcessingNodes.size();
    int mNode = usedProcessingNodes[mIndex];

    auto newNodes = _nodes;

    newNodes[mNode].v = random01();//+= randgaussian()/5;

    if(newNodes[mNode].v < 0.0){
        newNodes[mNode].v = 0.0;
    }else if(newNodes[mNode].v > 1.0){
        newNodes[mNode].v = 1.0;
    }

    return Genome(newNodes, _connections);
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

    int randomNode = getRandomNode(mCon.x, _nodes.size());
    newConnections[mCon.x][mCon.y].node = randomNode;

    return Genome(_nodes, newConnections);
}

Genome* Genome::uniformMutation(double probability){

    auto newConnections = _connections;

    for(int i = 0; i < _connections.size(); i++){
        for(int j = 0; j < _connections[i].size(); j++){
            if(random01() > probability){
                int randomNode = getRandomNode(i, _nodes.size());
                newConnections[i][j].node = randomNode;
            }
        }
    }

    return new Genome(_nodes, newConnections);
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
            int temp = o1Connections[i][crossoverConnection - index].node;
            o1Connections[i][crossoverConnection - index].node = o2Connections[i][crossoverConnection - index].node;
            o2Connections[i][crossoverConnection - index].node = temp;
            break;
        }
        else{
            index += _connections[i].size();
        }
    }

    return {
        Genome(_nodes,  o1Connections),
        Genome(_nodes,  o2Connections)
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

    int temp = o1Connections[crossCon.x][crossCon.y].node;
    o1Connections[crossCon.x][crossCon.y].node = o2Connections[crossCon.x][crossCon.y].node;
    o2Connections[crossCon.x][crossCon.y].node = temp;


    return {
        Genome(_nodes,  o1Connections),
        Genome(_nodes,  o2Connections)
    };
}

Network* Genome::buildNetwork(){
    return new Network(_nodes, _connections, _nodeUsage, _connectionUsage);
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


Genome Genome::createGenome(int nbProcessingNodes, std::vector<int> judgementNodesOutput, int processT, int judgeT, int connectionT, int neatT, int nbEachProcessingNode, int nbEachJudgementNode, int nbNeatNodes){
    
    std::vector<Node> nodes;
    std::vector<std::vector<Connection>> connections;
    
    // add start node
    nodes.push_back(Node(NodeType::Start, 0, 0));

    // add judgement nodes
    for(int i = 0; i < judgementNodesOutput.size(); i++){
        for(int j = 0; j < nbEachJudgementNode; j++){
            nodes.push_back(Node(NodeType::Judgement, i, judgeT));
        }
    }

    // add processing nodes
    for(int i = 0; i < nbProcessingNodes; i++){
        for(int j = 0; j < nbEachProcessingNode; j++){
            double v = random01();//randgaussian()/5 + 0.5;
            if(v < 0){
                v = 0;
            }else if(v > 1){
                v = 1;
            }

            nodes.push_back(Node(NodeType::Processing, i, processT, v));
        }
    }
    
    // add neat nodes
    for(int i = 0; i < nbNeatNodes; i++){
        nodes.push_back(Node(NodeType::NEAT, i, neatT));
    }

    for(int i = 0; i < nodes.size(); i++){

        switch(nodes[i].type){
            case NodeType::Start:
            {
                int firstNode = randint() % (nodes.size() - 1) + 1;

                connections.push_back({Connection(firstNode,  connectionT)});
            }break;

            case NodeType::Judgement:
            {
                std::vector<Connection> con;
                int nbConnections = judgementNodesOutput[nodes[i].index];

                for (int j = 0; j < nbConnections; j++){
                    int nextNode = getRandomNode(i, nodes.size());
                    con.push_back(Connection(nextNode, connectionT));
                }

                connections.push_back(con);

            }break;

            case NodeType::Processing:
            {
                int nextNode = randint() % (nodes.size() - 2) + 1;
                if(nextNode == i){
                    nextNode++;
                }
                connections.push_back({Connection(nextNode,  connectionT)});
            }break;
            case NodeType::NEAT:
            {
                int nextNode = randint() % (nodes.size() - 2) + 1;
                if(nextNode == i){
                    nextNode++;
                }
                connections.push_back({Connection(nextNode,  connectionT)});
            }break;

        }

    }
    
    return Genome(nodes, connections);
}

}
