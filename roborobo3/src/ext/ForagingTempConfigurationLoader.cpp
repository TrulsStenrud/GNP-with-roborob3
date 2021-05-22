#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"
#include "../../include/masterthesis/evolvers/ControllerEvolver.h"
#include "../../include/masterthesis/evolvers/MultiNEATEvolver.h"
#include "NoveltySearchEvolver.h"
#include "../../include/masterthesis/evolvers/GNPEvolver.h"
#include "../../include/masterthesis/evolvers/MPFAEvolver.h"
#include <stdlib.h>

using namespace std;

ControllerEvolver::CONTROLLER ForagingTempConfigurationLoader::controllerType = static_cast<ControllerEvolver::CONTROLLER>(-1);

ForagingTempConfigurationLoader::ForagingTempConfigurationLoader()
{
	DataForwarder::getDataForwarder()->registerListener(this);
	int cType = 0;
    if (controllerType == -1){
        gProperties.checkAndGetPropertyValue("gControllerType", &cType, true);
        controllerType = static_cast<ControllerEvolver::CONTROLLER>(cType);
    }

	std::cout<<"controller type: "<<controllerType<<std::endl;
	switch(controllerType){
	case ControllerEvolver::GNP:
    case ControllerEvolver::GNPPlusPLus:
		_evolver = new GNPEvolver(controllerType);
		break;
	case ControllerEvolver::NEAT:
	
	case ControllerEvolver::HyperNEAT:
		_evolver = new MultiNEATEvolver(controllerType);
		break;
    case ControllerEvolver::NoveltySearch:
        _evolver = new NoveltySearchEvolver();
        break;
	case ControllerEvolver::MPFA:
		_evolver = new MPFAEvolver();
		break;
	default:
		cout<<"controller type not implemented yet. Exiting..."<<endl;
		exit(3);
		break;
	}
}

ForagingTempConfigurationLoader::~ForagingTempConfigurationLoader()
{
	DataForwarder::getDataForwarder()->removeListener(this);
	delete _evolver;
}

void ForagingTempConfigurationLoader::onGenerationDone(DataPacket* data){

}

void ForagingTempConfigurationLoader::onSimulationDone(){
	delete _evolver;
}

WorldObserver* ForagingTempConfigurationLoader::make_WorldObserver(World* wm)
{
	ForagingTempWorldObserver* wo = new ForagingTempWorldObserver(wm);
	wo->setControllerEvolver(_evolver);
	return wo;
}

RobotWorldModel* ForagingTempConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* ForagingTempConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new ForagingTempAgentObserver(wm);
}

Controller* ForagingTempConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return _evolver->make_Controller(wm);
}

#endif
