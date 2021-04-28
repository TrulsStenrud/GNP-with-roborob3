#if defined PRJ_MYTESTEE || !defined MODULAR

#include "Config/MyTestEEConfigurationLoader.h"
#include "MyTestEE/include/MyTestEEWorldObserver.h"
#include "MyTestEE/include/MyTestEEAgentObserver.h"
#include "MyTestEE/include/MyTestEEController.h"
#include "WorldModels/RobotWorldModel.h"

MyTestEEConfigurationLoader::MyTestEEConfigurationLoader()
{
}

MyTestEEConfigurationLoader::~MyTestEEConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MyTestEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MyTestEEWorldObserver(wm);
}

RobotWorldModel* MyTestEEConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* MyTestEEConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new MyTestEEAgentObserver(wm);
}

Controller* MyTestEEConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new MyTestEEController(wm);
}

#endif
