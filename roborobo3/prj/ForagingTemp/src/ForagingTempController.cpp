/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "ForagingTemp/include/ForagingTempController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

ForagingTempController::ForagingTempController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

ForagingTempController::~ForagingTempController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void ForagingTempController::stepController()
{
    TemplateEEController::stepController();
}

void ForagingTempController::initController()
{
    TemplateEEController::initController();
}

void ForagingTempController::initEvolution()
{
    TemplateEEController::initEvolution();
}

void ForagingTempController::stepEvolution()
{
    TemplateEEController::stepEvolution();
}

void ForagingTempController::performSelection()
{
    TemplateEEController::performSelection();
}

void ForagingTempController::performVariation()
{
    TemplateEEController::performVariation();
}

void ForagingTempController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

bool ForagingTempController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool ForagingTempController::receiveGenome( Packet* p )
{
    return TemplateEEController::receiveGenome(p);
}

double ForagingTempController::getFitness()
{
    return TemplateEEController::getFitness();
}

void ForagingTempController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void ForagingTempController::updateFitness()
{
    // nothing to do -- updating is performed in ForagingTempController::AgentObserver (automatic event when energy item are captured)
}

void ForagingTempController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
