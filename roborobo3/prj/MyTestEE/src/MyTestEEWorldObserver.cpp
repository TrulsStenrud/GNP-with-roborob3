/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEWorldObserver.h"
#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/MyTestEESharedData.h"
#include "MyTestEE/include/ForagingObject.h"
#include "MyTestEE/include/NestObject.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

MyTestEEWorldObserver::MyTestEEWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

MyTestEEWorldObserver::~MyTestEEWorldObserver()
{
    // superclass destructor called before
}

void MyTestEEWorldObserver::initPre()
{
    TemplateEEWorldObserver::initPre();
    
    {
        int id = PhysicalObjectFactory::getNextId();
        
        auto nest = new NestObject(id);
        gPhysicalObjects.push_back(nest);
        nest->relocate();
    }
    {
        int id = PhysicalObjectFactory::getNextId();
        
        auto nest = new NestObject(id);
        gPhysicalObjects.push_back(nest);
        nest->relocate();
    }
    
    int nbOfObjects = 20;
    
    for(int i = 0; i < nbOfObjects; i++){
        int id = PhysicalObjectFactory::getNextId();
        
        auto newItem = new ForagingObject(id);
        gPhysicalObjects.push_back(newItem);
        newItem->setDisplayColor(255,128,64);
        newItem->relocate();
    }
    
}

void MyTestEEWorldObserver::initPost()
{
    TemplateEEWorldObserver::initPost();
    gNbOfPhysicalObjects = (int)gPhysicalObjects.size(); // must be done in the initPost() method for objects created in initPre().
}

void MyTestEEWorldObserver::stepPre()
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

void MyTestEEWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void MyTestEEWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
