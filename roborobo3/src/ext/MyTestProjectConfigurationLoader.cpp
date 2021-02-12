#if defined PRJ_MYTESTPROJECT || !defined MODULAR

#include "Config/MyTestProjectConfigurationLoader.h"
#include "MyTestProject/include/MyTestProjectWorldObserver.h"
#include "MyTestProject/include/MyTestProjectAgentObserver.h"
#include "MyTestProject/include/MyTestProjectController.h"
#include "WorldModels/RobotWorldModel.h"


MyTestProjectConfigurationLoader::MyTestProjectConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
}

MyTestProjectConfigurationLoader::~MyTestProjectConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MyTestProjectConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MyTestProjectWorldObserver(wm);
}

RobotWorldModel* MyTestProjectConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* MyTestProjectConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new MyTestProjectAgentObserver(wm);
}

Controller* MyTestProjectConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new MyTestProjectController(wm);
}


#endif
