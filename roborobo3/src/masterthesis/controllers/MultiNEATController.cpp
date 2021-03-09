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

	setTranslation(output[0]);
    setRotation(output[1]);
//    setTranslation(1);
//    setRotation(0.5);
}

double MultiNEATController::normalize(double num){
	return -1 + 2/(1+exp(-num));
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
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    if ( i == gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() )
                    {
                        inputs->push_back( 1 ); // match
                    }
                    else
                    {
                        inputs->push_back( 0 );
                    }
                }
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    inputs->push_back( 0 );
                }
            }
        }

        if ( gSensoryInputs_isOtherAgent )
        {
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                inputs->push_back( 1 );

                if ( gSensoryInputs_otherAgentGroup )
                {
                    // same group?
                    if ( gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->getGroupId() == _wm->getGroupId() )
                    {
                        inputs->push_back( 1 ); // match: same group
                    }
                    else
                    {
                        inputs->push_back( 0 ); // not the same group
                    }
                }

                if ( gSensoryInputs_otherAgentOrientation )
                {
                    // relative orientation? (ie. angle difference wrt. current agent)
                    double srcOrientation = _wm->_agentAbsoluteOrientation;
                    double tgtOrientation = gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->_agentAbsoluteOrientation;
                    double delta_orientation = - ( srcOrientation - tgtOrientation );
                    if ( delta_orientation >= 180.0 )
                    {
                        delta_orientation = - ( 360.0 - delta_orientation );
                    }
                    else
                    {
                        if ( delta_orientation <= -180.0 )
                        {
                            delta_orientation = - ( - 360.0 - delta_orientation );
                        }
                    }
                    inputs->push_back( delta_orientation/180.0 );
                }
            }
            else
            {
                inputs->push_back( 0 ); // not an agent...
                if ( gSensoryInputs_otherAgentGroup )
                {
                    inputs->push_back( 0 ); // ...therefore no match wrt. group.
                }
                if ( gSensoryInputs_otherAgentOrientation )
                {
                    inputs->push_back( 0 ); // ...and no orientation.
                }
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
        inputs->push_back( (double)_wm->getGroundSensor_redValue()/255.0 );
        inputs->push_back( (double)_wm->getGroundSensor_greenValue()/255.0 );
        inputs->push_back( (double)_wm->getGroundSensor_blueValue()/255.0 );
    }

    // landmark(s)
    if ( gSensoryInputs_landmarkTrackerMode == 1 ) // closest landmark only
    {
        _wm->updateLandmarkSensor(); // update with closest landmark
        if ( gSensoryInputs_distanceToLandmark )
        {
            inputs->push_back( _wm->getLandmarkDistanceValue() );
        }
        if ( gSensoryInputs_orientationToLandmark )
        {
            inputs->push_back( _wm->getLandmarkDirectionAngleValue() );
        }
    }
    else
        if ( gSensoryInputs_landmarkTrackerMode == 2 ) // register all landmarks
        {
            for ( int i = 0 ; i != gNbOfLandmarks ; i++ )
            {
                _wm->updateLandmarkSensor(i); // update with closest landmark
                if ( gSensoryInputs_distanceToLandmark )
                {
                    inputs->push_back( _wm->getLandmarkDistanceValue() );
                }
                if ( gSensoryInputs_orientationToLandmark )
                {
                    inputs->push_back( _wm->getLandmarkDirectionAngleValue() );
                }
            }
        }

    // energy level
    if ( gEnergyLevel && gSensoryInputs_energyLevel )
    {
        inputs->push_back( _wm->getEnergyLevel() / gEnergyMax );
    }

    // reentrant mapping from output neurons (motor or virtual output neurons, if any) -- Jordan-like reccurence.
    if ( gReentrantMapping_motorOutputs || gReentrantMapping_virtualOutputs )
    {
        std::vector<double> outputs = _nn->Output();

        size_t i_start = 0;
        size_t i_end = outputs.size();

//        if ( gReentrantMapping_motorOutputs == false )
//        {
//            if ( TemplateEESharedData::gEnergyRequestOutput )
//            {
//                i_start = 3;
//            }
//            else
//            {
//                i_start = 2;
//            }
//        }
//
//        if ( gReentrantMapping_virtualOutputs == false )
//        {
//            if ( TemplateEESharedData::gEnergyRequestOutput )
//            {
//                i_end = 3;
//            }
//            else
//            {
//                i_end = 2;
//            }
//        }

        for ( size_t i = i_start ; i < i_end ; i++ )
        {
            inputs->push_back( outputs[i] );
        }
    }

    return inputs;
}
