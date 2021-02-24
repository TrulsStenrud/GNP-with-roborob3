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
#include "World/MovableObject.h"
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
        
        nest->unregisterObject();
        nest->setCoordinates(1000, 600);
        nest->registerObject();
        
        gPhysicalObjects.push_back(nest);
        
    }
    
    int nbOfObjects = 20;
    
    placeGridOfObjects(50, 50, 20, 20);
    for(int i = 0; i < nbOfObjects; i++){
//        int id = PhysicalObjectFactory::getNextId();
//
//        auto newItem = new ForagingObject(id);
//        gPhysicalObjects.push_back(newItem);
//        newItem->setDisplayColor(255,128,64);
//        newItem->relocate();
    }
    
}

void MyTestEEWorldObserver::placeGridOfObjects(int x, int y, int columns, int rows){
    int cellWidth = 20;
    int cellHeight = 20;
    
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            placeObject(x + i * cellWidth, y + j * cellHeight);
        }
    }
}

void MyTestEEWorldObserver::placeObject(double x, double y){
    int id = PhysicalObjectFactory::getNextId();
    
    auto newItem = new ForagingObject(id);
    gPhysicalObjects.push_back(newItem);
    newItem->setDisplayColor(255,128,64);
    newItem->unregisterObject();
    newItem->setCoordinates(x, y);
    newItem->registerObject();
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
