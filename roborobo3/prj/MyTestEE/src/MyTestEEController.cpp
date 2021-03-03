/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEController.h"
#include "MyTestEE/include/NestObject.h"
#include "MyTestEE/include/ForagingObject.h"
#include "MyTestEE/include/PheromoneObject.h"
#include "MyTestEE/include/PheromoneObjectFactory.h"
#include "MyTestEE/include/MyTestEEWorldObserver.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"


using namespace Neural;



MyTestEEController::MyTestEEController( RobotWorldModel *wm ) : Controller( wm )
{
    // superclass constructor called before this baseclass constructor.

    _carriedObjectId = -1; // -1 means not carrying anything
    _testPheromone = new PheromoneObject(-1);
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
    if(gWorld->getIterations() > 10000){
        //return;
    }
    setTranslation(1);
    
    int v = _wm->getGroundSensorValue();
    PheromoneObject* p;
    
    setTranslation(1);
    setRotation(0.01);
    
    //setRotation(getNestRelativeOrientation()/5);
        
    if(isCarrying()){
        _wm->setRobotLED_colorValues(0, 0, 255);
        
    }else{
        _wm->setRobotLED_colorValues(255, 0, 0);
    }

    dropPheromone();
    //TemplateEEController::stepController();
}

void MyTestEEController::dropPheromone(){
    int v = _wm->getGroundSensorValue();
    PheromoneObject* p;
    
    if(PhysicalObject::isInstanceOf(v)
       && (p = dynamic_cast<PheromoneObject*>(gPhysicalObjects[v-gPhysicalObjectIndexStartOffset]))!=NULL){
        p->updateStrength();
    }
    else{
        int x = _wm->_xReal, y = _wm->_yReal;
        _testPheromone->setCoordinates(x, y);
        if(_testPheromone->canRegister()){
            PheromoneObjectFactory::placePheromoneObject(x, y);
        }
    }
    
}

double MyTestEEController::getPheromoneValue(){
    int v = _wm->getGroundSensorValue();
    if(PhysicalObject::isInstanceOf(v)){
        auto pheromone = dynamic_cast<PheromoneObject*>(gPhysicalObjects[v-gPhysicalObjectIndexStartOffset]);
        
        if(pheromone != NULL){
            return pheromone->getStrength();
        }
    }
    
    return 0;
}

// returns the robots orientation relative to the closest nest mappen to [ -1, +1 ]
double MyTestEEController::getNestRelativeOrientation(){
   
    if(gNestObjects.size() == 0){
        std::cout << "[ERROR] Impossible call to MyTestEEController::getNestRelativeOrientation(.)\n";
        exit(-1);
    }
    
    double inf = std::numeric_limits<double>::infinity();
    double nest_x = 0, nest_y = 0;
    
    int x = _wm->_xReal, y = _wm->_yReal;
    for(auto nest : gNestObjects){
        double n_x = nest->getXReal(), n_y = nest->getYReal();
        double delta = getEuclideanDistance(x, y, n_x, n_y);
        
        if(delta < inf){
            inf = delta;
            nest_x = n_x;
            nest_y = n_y;
        }
    }
    
    return getAngleToTarget(x, y, getOrientation() * 180, nest_x, nest_y) / 180;
}
