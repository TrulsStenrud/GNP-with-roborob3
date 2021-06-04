/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "ForagingTemp/include/ForagingTempSharedData.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "RoboroboMain/common.h"
#include "../../../include/masterthesis/DataForwarder.h"
#include "../../../include/core/Agents/Robot.h"
#include "../../../include/masterthesis/controllers/MultiNEATController.h"
#include "MscMain.h"


ForagingTempWorldObserver::ForagingTempWorldObserver( World* world ) : MyTestEEWorldObserver( world )
{
    // superclass constructor called before

    gProperties.checkAndGetPropertyValue("gEvaluationTime", &_evalTime, true);
}

ForagingTempWorldObserver::~ForagingTempWorldObserver()
{
    delete _sampledState;
}

void ForagingTempWorldObserver::setControllerEvolver(ControllerEvolver* evolver){
	_evolver = evolver;
    if(_evolver->usesBehavior()){
        _sampledState = new SampledAverageState();
        _captureBehavior = true;
    }else{
        delete _sampledState;
        _captureBehavior = false;
    }
}

double ForagingTempWorldObserver::getFitness(){
    double fitness = 0;

    for(auto nest : gNestObjects){
        fitness += nest->getCollectedGoods();
    }

	for(auto robot : gRobots){
		auto controller = dynamic_cast<MyTestEEController*>(robot->getController());
		if(controller->isCarrying()){
			fitness+=1/gNbOfRobots;
		}
	}

    return fitness;
}

double ForagingTempWorldObserver::getForagingPercentage(){
    double foragedObjects = 0;
    for(auto nest : gNestObjects){
        foragedObjects += nest->getCollectedGoods();
    }

    if(foragedObjects > _nbForagingObjects){
        std::cout << "[ALERT]" << std::endl;
    }

    return (foragedObjects / _nbForagingObjects) * 100;
}

void ForagingTempWorldObserver::stepPre( )
{
 //do nothing
}

void ForagingTempWorldObserver::initConfigurations() {
    if(gMscPlacementConfiguration == PlacementConfiguration::MIX){
        _evalConfigurations.push_back(PlacementConfiguration::Random);
        _evalConfigurations.push_back(PlacementConfiguration::SemiCluster);
        _evalConfigurations.push_back(PlacementConfiguration::Cluster);
    }
    else{
        _evalConfigurations.push_back(gMscPlacementConfiguration);
    }
}

void ForagingTempWorldObserver::resetField(){
    reset(_evalConfigurations.back());
    _evalConfigurations.pop_back();
}

void ForagingTempWorldObserver::initPre(){
    MyTestEEWorldObserver::initPre();
    _dp = new DataPacket();
    initConfigurations();
    int temp = gNbOfPhysicalObjects;
    resetField();
    gNbOfPhysicalObjects = temp;
}

void ForagingTempWorldObserver::stepPost(){
    if( (gWorld->getIterations() + 1) % _evalTime == 0 )
    {
        appendToDataPacket();
        
        if(_evalConfigurations.empty()){
            if(gMscPlacementConfiguration == PlacementConfiguration::MIX){
                _dp->foragingPercentage /= 3;
            }
            DataForwarder::getDataForwarder()->forwardData(_dp);
            _evolver->evalDone(_dp);
            delete _dp;
            
            if(_captureBehavior){
                delete _sampledState;
                _sampledState = new SampledAverageState();
            }
            if(gMaxIt != -1 && gWorld->getIterations() >= gMaxIt){
                DataForwarder::getDataForwarder()->simulationDone();
            }
            
            _dp = new DataPacket();
            initConfigurations();
        }
            
        resetField();
    }

    if(_captureBehavior)
    {
        std::vector<double> state;

        for(auto robot : gRobots){
            auto controller = dynamic_cast<MultiNEATController*>(robot->getController());
            if(controller == NULL){
                std::cout << "[ALERT] Controller type must be MultiNEATController" << std::endl;
            }
            else{
                if(state.empty()){
                    state = controller->buildInputVector();
                }
                else{
                    auto sensorState = controller->buildInputVector();
                    for(int i = 0; i < state.size(); i++){
                        state[i] += sensorState[i];
                    }
                }
            }
        }

        for(int i = 0; i < state.size(); i++){
            state[i] = state[i] / gNbOfRobots;
        }
        _sampledState->addState(state);
    }
}

void ForagingTempWorldObserver::appendToDataPacket(){
	_dp->generation = gWorld->getIterations()/_evalTime;

    if(_captureBehavior){
        _dp->behaviorData = _sampledState->getSampledAverage();
    }

    _dp->fitness += getFitness();
    
    _dp->foragingPercentage += getForagingPercentage();
}



