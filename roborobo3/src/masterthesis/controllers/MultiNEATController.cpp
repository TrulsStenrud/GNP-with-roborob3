#include "MultiNEATController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/World/World.h"

using namespace NEAT;

MultiNEATController::MultiNEATController(RobotWorldModel *wm, Genome* genome, ControllerEvolver::CONTROLLER controllerType):MyTestEEController(wm){
	_genome = genome;
	_controllerType = controllerType;
	_nn = new NEAT::NeuralNetwork();
	RebuildBrain(_genome);
}

MultiNEATController::~MultiNEATController(){

}

void MultiNEATController::reset(){
    MyTestEEController::reset();
}

void MultiNEATController::step(){
	_nn->Input(*buildInputVector());
	_nn->ActivateFast(); // Fast funker kun med unsigned sigmoid som aktiveringsfunksjon.
	std::vector<double> output = _nn->Output();

	setTranslation(output[0]*2 - 1);
    setRotation(output[1]*2 - 1);
    
    if(output[2] > 0.5){
        dropPheromone();
    }
    
}


void MultiNEATController::RebuildBrain(Genome* genome){
	switch(_controllerType){
	case ControllerEvolver::NEAT:
		_genome->BuildPhenotype(*_nn);
		break;
	case ControllerEvolver::HyperNEAT:
	case ControllerEvolver::ESHyperNEAT:
		//_genome-> NEAT::Genome::BuildHyperNEATPhenotype(_nn, Substrate& substr)
		break;
	}
}

std::vector<double>* MultiNEATController::buildInputVector(){
    //straight up copied from TemplateEEController.cpp
    // and then modified to use our new sensors
    
    std::vector<double>* inputs = new std::vector<double>();
    
    // distance sensors
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        if ( gSensoryInputs_distanceToContact )
            inputs->push_back( _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i) );
        
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        
        if ( gSensoryInputs_physicalObjectType )
        {
            // input: physical object? which type?
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
                {
                    inputs->push_back( 1 ); // match
                }
                else
                {
                    inputs->push_back( 0 );
                }
                
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                inputs->push_back( 0 );
                
            }
        }
        
        if ( gSensoryInputs_isOtherAgent )
        {
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                inputs->push_back( 1 );
            }
            else
            {
                inputs->push_back( 0 ); // not an agent...
            }
        }
        
        if ( gSensoryInputs_isWall )
        {
            // input: wall or empty?
            if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
            {
                inputs->push_back( 1 );
            }
            else
            {
                inputs->push_back( 0 ); // nothing. (objectId=-1)
            }
        }
        
    }
    
    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        inputs->push_back(getPheromoneValue());
    }
    
    // nest sensor
    inputs->push_back(getNestRelativeOrientation());
    
    return inputs;
}
