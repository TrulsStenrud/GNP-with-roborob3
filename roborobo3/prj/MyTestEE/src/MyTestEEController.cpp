/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "MyTestEE/include/MyTestEEController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;



MyTestEEController::MyTestEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
    _carriedObjectId = -1;
}

MyTestEEController::~MyTestEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

bool MyTestEEController::isCarrying(){
    return _carriedObjectId != -1;
}

void MyTestEEController::unLoad(){
    _carriedObjectId = -1;
}

void MyTestEEController::setCarrying(int objectId){
    if (objectId >= gPhysicalObjectIndexStartOffset
        && objectId < gPhysicalObjectIndexStartOffset + gPhysicalObjects.size()){
        _carriedObjectId = objectId;
    }
    else{
        std::cerr << "Object id [" << objectId << "] not within the range of physical objects" << std::endl;
    }
}

void MyTestEEController::stepController()
{
    TemplateEEController::stepController();
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
