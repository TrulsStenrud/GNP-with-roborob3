#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"
#include <stdlib.h>
#include "../../include/masterthesis/evolvers/ControllerEvolver.h"
#include "../../include/masterthesis/evolvers/MultiNEATEvolver.h"
#include "../../include/masterthesis/evolvers/GNPEvolver.h"

using namespace std;

ControllerEvolver::CONTROLLER ForagingTempConfigurationLoader::controllerType = ControllerEvolver::GNP;

ForagingTempConfigurationLoader::ForagingTempConfigurationLoader()
{
	std::cout<<"controller type: "<<controllerType<<std::endl;
	switch(controllerType){
	case ControllerEvolver::GNP:
            _evolver = new GNPEvolver();
		exit(3);
		break;
	case ControllerEvolver::NEAT:
	case ControllerEvolver::NoveltySearch:
	case ControllerEvolver::HyperNEAT:
	case ControllerEvolver::ESHyperNEAT:
		_evolver = new MultiNEATEvolver(controllerType);
		break;
	case ControllerEvolver::MPFA:
		cout<<"MPFA controller type not implemented yet. Exiting..."<<endl;
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
