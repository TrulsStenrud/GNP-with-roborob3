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
    /*
    // EXAMPLE
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
    {
        std::cout << "[INFO] new generation.\n";
    }
    */
}

void ForagingTempWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void ForagingTempWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
