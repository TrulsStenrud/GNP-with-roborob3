#if defined PRJ_FORAGINGTEMP || !defined MODULAR

#include "Config/ForagingTempConfigurationLoader.h"
#include "ForagingTemp/include/ForagingTempWorldObserver.h"
#include "ForagingTemp/include/ForagingTempAgentObserver.h"
#include "ForagingTemp/include/ForagingTempController.h"
#include "WorldModels/RobotWorldModel.h"

ForagingTempConfigurationLoader::ForagingTempConfigurationLoader()
{
}

ForagingTempConfigurationLoader::~ForagingTempConfigurationLoader()
{
	//nothing to do
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
	return new ForagingTempController(wm);
}

#endif
