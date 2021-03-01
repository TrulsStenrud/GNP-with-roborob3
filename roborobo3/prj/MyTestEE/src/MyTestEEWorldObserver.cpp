/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEWorldObserver.h"
#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/MyTestEESharedData.h"
#include "MyTestEE/include/ForagingObject.h"

#include "MyTestEE/include/PheromoneObject.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "World/MovableObject.h"
#include "RoboroboMain/roborobo.h"

std::vector<NestObject*> gNestObjects; // list of nestObjects added to gPhysicalObjects

MyTestEEWorldObserver::MyTestEEWorldObserver( World* world ) : WorldObserver( world )
{
    
    std::string litelogFullFilename = gLogDirectoryname + "/lite_" + gLogFilename;
    gLitelogFile.open(litelogFullFilename.c_str());
        
    if(!gLitelogFile) {
        std::cout << "[CRITICAL] Cannot open \"lite\" log file " << litelogFullFilename << "." << std::endl << std::endl;
        exit(-1);
    }
        
    gLitelogManager = new LogManager();
    gLitelogManager->setLogFile(gLitelogFile);
    gLitelogManager->flush();
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

MyTestEEWorldObserver::~MyTestEEWorldObserver()
{
    gLitelogFile.close();
}



void MyTestEEWorldObserver::addNestObject(double x, double y) {
    int id = PhysicalObjectFactory::getNextId();
    auto nest = new NestObject(id);
    
    nest->unregisterObject();
    nest->setCoordinates(x, y);
    nest->registerObject();
    
    
    gPhysicalObjects.push_back(nest);
    gNestObjects.push_back(nest);
}

void MyTestEEWorldObserver::initPre()
{
    
    addNestObject(1000, 600);
    addNestObject(400, 600);

    
    int nbOfObjects = 20;
    
    placeGridOfObjects(50, 50, 2, 2);
    
    
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
    int cellWidth = 12;
    int cellHeight = 12;
    
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            placeObject(x + i * cellWidth, y + j * cellHeight);
        }
    }
}

void MyTestEEWorldObserver::placeObject(double x, double y){
    int id = PhysicalObjectFactory::getNextId();
    
    auto newItem = new ForagingObject(id);
    newItem->setDisplayColor(255,128,64);
    
    
    newItem->unregisterObject();
    newItem->setCoordinates(x, y);
    gPhysicalObjects.push_back(newItem);
    newItem->registerObject();
}

void MyTestEEWorldObserver::initPost()
{
    gNbOfPhysicalObjects = (int)gPhysicalObjects.size(); // must be done in the initPost() method for objects created in initPre().
}

void MyTestEEWorldObserver::stepPre()
{
    
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
}
