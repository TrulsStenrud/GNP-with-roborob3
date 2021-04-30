#include "GNPController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/World/World.h"
#include "RoboroboMain/roborobo.h"

#define NB_SENSORS 8 // should be coherent with gRobotSpecsImageFilename value read from the property file.

GNPController::GNPController(RobotWorldModel *wm, GNP::Genome& genome, std::vector<NEAT::Genome> neatGenomes):MyTestEEController(wm){
    
    for(int i = 0; i < neatGenomes.size(); i++){
       _neatNetworks.push_back(new NEAT::NeuralNetwork());
    }
    
    buildBrain(genome, neatGenomes);
}

GNPController::~GNPController(){

}

void print(std::string text){
    std::cout << text << std::endl;
}

void GNPController::reset(){
    MyTestEEController::reset();
}

void GNPController::step(){
    _gnpNetwork->step(this);
}

GNP::NodeInformation GNPController::getNodeLibrary(){

    GNP::NodeInformation info;
    info.nbProcessingNodes = 3;


//    for(int i  = 0; i < 8; i++) // must match the number of camera sensors
//    {
//        if ( gSensoryInputs_distanceToContact )
//            info.judgementNodeOutputs.push_back(3);
//
//
//        if ( gSensoryInputs_physicalObjectType )
//        {
//            info.judgementNodeOutputs.push_back(2); // is foraging object or not foraging object
//        }
//
//        if ( gSensoryInputs_isOtherAgent )
//        {
//            info.judgementNodeOutputs.push_back(2); // is agent or is not agent
//        }
//
//        if ( gSensoryInputs_isWall )
//        {
//            info.judgementNodeOutputs.push_back(2); // is wall or not wall
//        }
//
//    }

    // object sensor: left, forward, right, back
    info.judgementNodeOutputs.push_back(4); // nothing, agent, foraging object, wall
    info.judgementNodeOutputs.push_back(4);
    info.judgementNodeOutputs.push_back(4);
    info.judgementNodeOutputs.push_back(4);


    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        info.judgementNodeOutputs.push_back(2); // pheromone or not
    }

    // nest sensor
    info.judgementNodeOutputs.push_back(4); // 4 directions makes sense, right?

    // carrying sensor
    info.judgementNodeOutputs.push_back(2);

    return info;
}

int GNPController::judge(int judgeIndex){
    
    double direction;
    
    switch(judgeIndex){
        case 0:
            return judgeObjectTypeForSensors({7, 0, 1}); // left
        case 1:
            return judgeObjectTypeForSensors({1, 2, 3}); // front
        case 2:
            return judgeObjectTypeForSensors({3, 4, 5}); //right
        case 3:
            return judgeObjectTypeForSensors({5, 6, 7}); //back
        case 4:
            return getPheromoneValue() > 0 ? 0 : 1; // pheromone sensor
        case 5:
            direction = getNestRelativeOrientation();
            if(direction < -0.75){
                return 0;
            }
            if(direction < -0.25){
                return 1;
            }
            if(direction <= 0.25){
                return 2;
            }
            if(direction <= 0.75){
                return 3;
            }
            return 0;
        
        case 6:
            return isCarrying() ? 1 : 0;
        
        default:
            std::cout << "[Error] - Judgement index outside of range: " << judgeIndex << std::endl;
            exit(-1);
            
    }
}
void GNPController::process(int processIndex, double value){
    
    switch(processIndex){
        case 0:
            setTranslation(value*2-1);
            break;
        case 1:
            setRotation(value*2-1);
            break;
        case 2:
            if(value > 0.5){ // questionable
                dropPheromone();
            }
            break;
        default:
            std::cout << "[Error] - Process index outside of range: " << processIndex << std::endl;
            exit(-1);
    }
}
void GNPController::processNeat(int neatIndex){
    auto inputs = buildInputVector();
    _neatNetworks[neatIndex]->Input(inputs);
    
    _neatNetworks[neatIndex]->ActivateFast(); // Fast funker kun med unsigned sigmoid som aktiveringsfunksjon.
    std::vector<double> output = _neatNetworks[neatIndex]->Output();
    
    setTranslation(output[0]*2 - 1);
    setRotation(output[1]*2 - 1);

    if(output[2] > 0.5){
        dropPheromone();
    }
}

void GNPController::buildBrain(GNP::Genome& genome, std::vector<NEAT::Genome> neatGenomes){
    delete _gnpNetwork;

    for(int i = 0; i < neatGenomes.size(); i++){
        neatGenomes[i].BuildPhenotype(*_neatNetworks[i]);
    }
    
    _gnpNetwork = genome.buildNetwork();
}



int GNPController::judgeObjectTypeForSensors(std::vector<int> sensors) {
    bool wall = false, agent = false, foragingObject = false;
    for(int i : sensors){
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        if(PhysicalObject::isInstanceOf(objectId) && gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5){
            foragingObject = true;
        }
        // input: another agent? If yes: same group?
        if ( Agent::isInstanceOf(objectId) )
        {
            // this is an agent
            agent = true;
        }
        // input: wall or empty?
        if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ){
            wall = true;
        }
    }

    if(foragingObject){
        return 0;
    }
    if(agent){
        return 1;
    }
    if(wall){
        return 2;
    }

    return 3;
}




GNP::Network* GNPController::getNetwork(){
    return _gnpNetwork;
}

void GNPController::applyOutputVector(std::vector<double> output){
    setTranslation(output[0]*2 - 1);
    setRotation(output[1]*2 - 1);

    if(output[2] > 0.5){
        dropPheromone();
    }
}

std::vector<double> GNPController::buildInputVector(){
    //straight up copied from MultiNEATCOntroller.cpp
    
    
    std::vector<double> inputs;
    
    // distance sensors
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        if ( gSensoryInputs_distanceToContact )
            inputs.push_back( _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i) );
        
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        
        if ( gSensoryInputs_physicalObjectType )
        {
            // input: physical object? which type?
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
                {
                    inputs.push_back( 1 ); // match
                }
                else
                {
                    inputs.push_back( 0 );
                }
                
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                inputs.push_back( 0 );
                
            }
        }
        
        if ( gSensoryInputs_isOtherAgent )
        {
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                inputs.push_back( 1 );
            }
            else
            {
                inputs.push_back( 0 ); // not an agent...
            }
        }
        
        if ( gSensoryInputs_isWall )
        {
            // input: wall or empty?
            if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
            {
                inputs.push_back( 1 );
            }
            else
            {
                inputs.push_back( 0 ); // nothing. (objectId=-1)
            }
        }
        
    }
    
    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        inputs.push_back(getPheromoneValue());
    }
    
    // nest sensor
    inputs.push_back(getNestRelativeOrientation());
    
    if(isCarrying()){
        inputs.push_back(1);
    }else{
        inputs.push_back(0);
    }
    
    inputs.push_back(1);
    return inputs;
}
