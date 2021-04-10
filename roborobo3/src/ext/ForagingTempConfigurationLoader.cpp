#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"
#include <stdlib.h>
#include "../../include/masterthesis/controllers/ControllerEvolver.h"
#include "../../include/masterthesis/controllers/MultiNEATEvolver.h"
#include "../../include/core/RoboroboMain/roborobo.h"

using namespace std;

ControllerEvolver::CONTROLLER ForagingTempConfigurationLoader::controllerType = ControllerEvolver::GNP;

ForagingTempConfigurationLoader::ForagingTempConfigurationLoader()
{
	int cType = 0;
	gProperties.checkAndGetPropertyValue("gControllerType", &cType, true);
	controllerType = static_cast<ControllerEvolver::CONTROLLER>(cType);
	std::cout<<"controller type: "<<controllerType<<std::endl;
	switch(controllerType){
	case ControllerEvolver::NEAT:
	case ControllerEvolver::NoveltySearch:
	case ControllerEvolver::HyperNEAT:
        _evolver = new MultiNEATEvolver(controllerType);
        break;
	default:
		cout<<"controller type not implemented yet. Exiting..."<<endl;
		exit(3);
		break;
	}
}

ForagingTempConfigurationLoader::~ForagingTempConfigurationLoader()
{
	//destroy
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
	// få _evolver inn i bildet her. sende argument for hvor mange som skal bli skapt?
	return _evolver->make_Controller(wm);
	//return new ForagingTempController(wm);
}

#endif
