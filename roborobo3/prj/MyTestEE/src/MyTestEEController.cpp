/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/NestObject.h"
#include "MyTestEE/include/ForagingObject.h"
#include "MyTestEE/include/PheromoneObject.h"
#include "MyTestEE/include/PheromoneObjectFactory.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;



MyTestEEController::MyTestEEController( RobotWorldModel *wm ) : Controller( wm )
{
    // superclass constructor called before this baseclass constructor.

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

void MyTestEEController::reset(){
    
}

void MyTestEEController::step()
{
    setTranslation(1);
    setRotation(0.01);
        
    dropPheromone();
    //TemplateEEController::stepController();
}

void MyTestEEController::dropPheromone(){
    int x = _wm->_xReal, y = _wm->_yReal;
    _testPheromone->setCoordinates(x, y);
    
    if(_testPheromone->canRegister()){
        PheromoneObjectFactory::placePheromoneObject(x, y);
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
