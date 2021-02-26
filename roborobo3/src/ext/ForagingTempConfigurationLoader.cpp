#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"
#include <stdlib.h>

ForagingTempConfigurationLoader::CONTROLLER ForagingTempConfigurationLoader::controller = GNP;

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
	return new ForagingTempWorldObserver(wm);
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
	std::exit(0);
	return new ForagingTempController(wm);
}

#endif
