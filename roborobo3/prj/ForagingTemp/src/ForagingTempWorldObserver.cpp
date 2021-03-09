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
    gLitelogFile.close();
}

void ForagingTempWorldObserver::setControllerEvolver(ControllerEvolver* evolver){
	_evolver = evolver;
}

double ForagingTempWorldObserver::getFitness(){
    
    
    double fitness = 0;
    
    for(auto nest : gNestObjects){
        fitness += nest->getCollectedGoods();
    }
    return fitness;
}

void ForagingTempWorldObserver::stepPre( )
{
	
    // denne blir kalt på slutten av hvert tidstrinn i evolusjonen.
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % _evalTime == 0 )
    {
    	std::cout<<"generasjon ferdig"<<std::endl;
    	std::vector<Robot*>* robots = new std::vector<Robot*>();
        for(int i=0; i<gWorld->getNbOfRobots(); i++){
			robots->push_back(gWorld->getRobot(i));
        }

        DataForwarder::getDataForwarder()->forwardData(constructDataPacket());
        if(gWorld->getIterations() == gMaxIt){
			DataForwarder::getDataForwarder()->simulationDone();
        }
        
        //todo add fitness
        _evolver->evalDone(robots);
        
        reset();
    }
}


DataPacket* ForagingTempWorldObserver::constructDataPacket(){
	// construct datapacket, then send to all registered listeners.
	DataPacket* dp = new DataPacket();
	dp->generation = 0;
	dp->bestFitness = -99999;
	float avgFitness = 0;
	for(int i=0; i<gWorld->getNbOfRobots(); i++){
		//avgFitness += 0;
		//dp->bestFitness = std::max(dp->bestFitness, -999);
	}
	return dp;
}



