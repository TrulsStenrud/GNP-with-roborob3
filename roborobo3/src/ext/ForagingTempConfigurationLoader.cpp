#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"
#include <stdlib.h>
#include "../../include/masterthesis/controllers/ControllerEvolver.h"
#include "../../include/masterthesis/controllers/MultiNEATEvolver.h"

using namespace std;

ControllerEvolver::CONTROLLER ForagingTempConfigurationLoader::controller = ControllerEvolver::GNP;

ForagingTempConfigurationLoader::ForagingTempConfigurationLoader()
{
	//init
}

ForagingTempConfigurationLoader::~ForagingTempConfigurationLoader()
{
	//destroy
}

WorldObserver* ForagingTempConfigurationLoader::make_WorldObserver(World* wm)
{
	ForagingTempWorldObserver* wo = new ForagingTempWorldObserver(wm);
	wo->setControllerEvolver(new MultiNEATEvolver(controller));
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
	std::cout<<"controller type: "<<controller<<std::endl;
	switch(controller){
	case ControllerEvolver::GNP:
		cout<<"GNP controller type not implemented yet. Exiting..."<<endl;
		exit(3);
		break;
	case ControllerEvolver::MPFA:
		cout<<"MPFA controller type not implemented yet. Exiting..."<<endl;
		exit(3);
		break;
	case ControllerEvolver::NEAT:
		cout<<"NEAT controller type not implemented yet. Exiting..."<<endl;
		//exit(3);
		break;
	}
	return new ForagingTempController(wm);
}

#endif
