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
#include "../../../include/masterthesis/controllers/ControllerEvolver.h"
#include "../../../include/masterthesis/controllers/MultiNEATController.h"


ForagingTempWorldObserver::ForagingTempWorldObserver( World* world ) : MyTestEEWorldObserver( world )
{
    // superclass constructor called before
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
    _evolver = nullptr;
    gProperties.checkAndGetPropertyValue("gEvaluationTime", &_evalTime, true);
    
    
}

ForagingTempWorldObserver::~ForagingTempWorldObserver()
{
    delete _sampledState;
    gLitelogFile.close();
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
        fitness += nest->getCollectedGoods()*gNbOfRobots;
    }
    
        for(auto robot : gRobots){
            auto controller = dynamic_cast<MyTestEEController*>(robot->getController());
            if(controller->isCarrying()){
                fitness+=1;
            }
        }

    return fitness;
}


void ForagingTempWorldObserver::stepPre( )
{
    // denne blir kalt på slutten av hvert tidstrinn i evolusjonen.
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % _evalTime == 0 )
    {
        
		DataPacket* dp = constructDataPacket();
        DataForwarder::getDataForwarder()->forwardData(dp);
        
        if(gWorld->getIterations() == gMaxIt){
			DataForwarder::getDataForwarder()->simulationDone();
        }

        _evolver->evalDone(dp);
        if(_captureBehavior){
            delete _sampledState;
            _sampledState = new SampledAverageState();
        }
        reset();
        

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




DataPacket* ForagingTempWorldObserver::constructDataPacket(){
	// construct datapacket, then send to all registered listeners.
	DataPacket* dp = new DataPacket();
	dp->generation = gWorld->getIterations()/_evalTime;
    auto fitness = getFitness();
    
    if(_captureBehavior){
        dp->behaviorData = _sampledState->getSampledAverage();
    }
    
    dp->fitness = fitness;
	dp->robots = new std::vector<Robot*>();
	for(int i=0; i<gWorld->getNbOfRobots(); i++){
		dp->robots->push_back(gWorld->getRobot(i));
	}
	return dp;
}



