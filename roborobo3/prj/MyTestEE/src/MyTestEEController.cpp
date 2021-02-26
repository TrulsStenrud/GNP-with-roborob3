/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/NestObject.h"
#include "MyTestEE/include/ForagingObject.h"
#include "MyTestEE/include/PheromoneObject.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;



MyTestEEController::MyTestEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
    _carriedObjectId = -1;
    
    _testPheromone = new PheromoneObject(-1);
    _testPheromone->unregisterObject();
}

MyTestEEController::~MyTestEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
    delete _testPheromone;
}

bool MyTestEEController::isCarrying(){
    return _carriedObjectId != -1;
}

void MyTestEEController::unLoad(){
    _carriedObjectId = -1;
}

void MyTestEEController::setCarrying(int objectId){
    if (PhysicalObject::isInstanceOf(objectId)){
        _carriedObjectId = objectId;
    }
    else{
        std::cerr << "Object id [" << objectId << "] not within the range of physical objects" << std::endl;
    }
}

void MyTestEEController::stepController()
{

    int v = _wm->getGroundSensorValue();
    if(PhysicalObject::isInstanceOf(v)){
        auto object = gPhysicalObjects[v-gPhysicalObjectIndexStartOffset];
        auto nest = dynamic_cast<NestObject*>( object );
        
        if(nest != NULL)
        {
            //std::cout << nest->getCollectedGoods() << std::endl;
            
//            int id = PhysicalObjectFactory::getNextId();
//
//            auto newItem = new ForagingObject(id);
//            gPhysicalObjects.push_back(newItem);
//            newItem->setDisplayColor(255,128,64);
//            newItem->relocate();
            gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
//            newItem->unregisterObject();
//            newItem->setCoordinates();
//            newItem->registerObject();
        }
    }
    
    setTranslation(1);
    setRotation(0.1);
    
    if(isPheromone()){
        
    }
    dropPheromone();
    //TemplateEEController::stepController();
}

void MyTestEEController::dropPheromone(){
    int x = _wm->_xReal, y = _wm->_yReal;
    _testPheromone->setCoordinates(x, y);
    
    if(_testPheromone->canRegister()){
        int id = PhysicalObjectFactory::getNextId();
        auto newItem = new PheromoneObject(id);
        newItem->unregisterObject();
        newItem->setCoordinates(x, y);
        newItem->registerObject();
        gPhysicalObjects.push_back(newItem);
        gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
    }
}

bool MyTestEEController::isPheromone(){
    int v = _wm->getGroundSensorValue();
    if(PhysicalObject::isInstanceOf(v)){
        auto nest = dynamic_cast<PheromoneObject*>(gPhysicalObjects[v-gPhysicalObjectIndexStartOffset]);
        
        return nest != NULL;
    }
    
    return false;
}



void MyTestEEController::initController()
{
    TemplateEEController::initController();
}

void MyTestEEController::initEvolution()
{
    TemplateEEController::initEvolution();
}

void MyTestEEController::stepEvolution()
{
    TemplateEEController::stepEvolution();
}

void MyTestEEController::performSelection()
{
    TemplateEEController::performSelection();
}

void MyTestEEController::performVariation()
{
    TemplateEEController::performVariation();
}

void MyTestEEController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

bool MyTestEEController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool MyTestEEController::receiveGenome( Packet* p )
{
    return TemplateEEController::receiveGenome(p);
}

double MyTestEEController::getFitness()
{
    return TemplateEEController::getFitness();
}

void MyTestEEController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void MyTestEEController::updateFitness()
{
    // nothing to do -- updating is performed in MyTestEEController::AgentObserver (automatic event when energy item are captured)
}

void MyTestEEController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
