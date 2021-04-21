/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEWorldObserver.h"
#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/MyTestEESharedData.h"
#include "MyTestEE/include/ForagingObject.h"
#include "MyTestEE/include/PheromoneObject.h"
#include "MyTestEE/include/ObjectFactory.h"

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
    nest->setCoordinates(x, y);
    nest->registerObject();

    gPhysicalObjects.push_back(nest);
    gNestObjects.push_back(nest);
}

void MyTestEEWorldObserver::reset(){
    
    if(gNestObjects.size() == 0){
        addNestObject(1200, 350);
        addNestObject(400, 600);
    }else{
        for(auto nest : gNestObjects){
            nest->resetValues();
        }
    }
    
    PheromoneObject* p;
    ForagingObject* f;
    for(auto object : gPhysicalObjects){
        if((p = dynamic_cast<PheromoneObject*>(object)) != NULL){
            p->evaporate();
        }
        else if((f = dynamic_cast<ForagingObject*>(object)) != NULL){
            f->hideObject();
        }
    }
    
    for(auto robot : gRobots){
        auto controller = dynamic_cast<MyTestEEController*>(robot->getController());
        controller->reset();
    }
    placeRobotsInAllNests();
    
    
    
    semiClusteredForagingObjects();
    
}

void MyTestEEWorldObserver::clusteredForagingObjects(){
    //placeGridOfObjects(500, 500, 4, 4);
    
    placeGridOfObjects(650, 150, 8, 8);
    placeGridOfObjects(1250, 650, 3, 3);
    placeGridOfObjects(1100, 200, 3, 6);
    placeGridOfObjects(150, 300, 6, 6);
}

void MyTestEEWorldObserver::semiClusteredForagingObjects()
{
    
    placeGridOfObjects(650, 150, 4, 4);
    placeGridOfObjects(1100, 200, 3, 3);
    placeGridOfObjects(150, 150, 4, 4);
    placeGridOfObjects(850, 600, 4, 4);
    placeGridOfObjects(100, 550, 4, 4);
    randomForagingObjects(45);
}


void MyTestEEWorldObserver::randomForagingObjects(int n)
{
    srand(1);
    for(int i = 0; i < n; i++){
        ObjectFactory::placeRandomForagingObject();
    }
}

void MyTestEEWorldObserver::initPre()
{
    int temp = gNbOfPhysicalObjects;
    reset();
    gNbOfPhysicalObjects = temp;
}

void MyTestEEWorldObserver::placeGridOfObjects(int x, int y, int columns, int rows){
    int cellWidth = 12;
    int cellHeight = 12;
    
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            ObjectFactory::placeForagingObject(x + i * cellWidth, y + j * cellHeight);
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

void MyTestEEWorldObserver::placeRobotsInAllNests() {
    int perNest = gNbOfRobots / gNestObjects.size();
    int width = sqrt(perNest);
    int spacing = 10;
    
    
    for(int i = 0; i < gNbOfRobots; i++){
        int startX = gNestObjects[i/perNest]->getXReal() - (spacing * width) / 2;
        int startY = gNestObjects[i/perNest]->getYReal() - (spacing * width) / 2;
        
        int x = startX + ((i%perNest) % width + 0.5) * spacing;
        int y = startY + ((i%perNest) / width + 0.5) * spacing;
        
        gRobots[i]->unregisterRobot();
        gRobots[i]->setCoord(x, y);
        gRobots[i]->setCoordReal(x, y);
        gRobots[i]->getWorldModel()->_agentAbsoluteOrientation = -90;
    }
}

void MyTestEEWorldObserver::initPost()
{
    gNbOfPhysicalObjects = (int)gPhysicalObjects.size(); // must be done in the initPost() method for objects created in initPre().
    
    placeRobotsInAllNests();
}

void MyTestEEWorldObserver::stepPre()
{
}

void MyTestEEWorldObserver::stepPost( )
{
}
