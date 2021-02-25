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

ForagingTempWorldObserver::ForagingTempWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

ForagingTempWorldObserver::~ForagingTempWorldObserver()
{
    // superclass destructor called before
    //delete _dataForwarder;
}

void ForagingTempWorldObserver::initPre()
{
    // nothing to do.
}

void ForagingTempWorldObserver::initPost()
{
    // nothing to do.
}

void ForagingTempWorldObserver::stepPre()
{
    TemplateEEWorldObserver::stepPre();

    // New generation
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
    {
        sendGenerationalUpdate(constructDataPacket());
    }

}

void ForagingTempWorldObserver::stepPost( )
{
    // denne blir kalt på slutten av hvert tidstrinn i evolusjonen.
}

void ForagingTempWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}



DataPacket* ForagingTempWorldObserver::constructDataPacket(){
	// construct datapacket, then send to all registered listeners.
	DataPacket* dp = new DataPacket();
	dp->generation = _generationCount;
	dp->bestFitness = -99999;
	for(int i=0; i<_world->getNbOfRobots(); i++){
//		dp.bestFitness = math.
	}
	return dp;
}

void ForagingTempWorldObserver::sendGenerationalUpdate(DataPacket* dp){
	DataForwarder::getDataForwarder()->forwardData(dp);
}
