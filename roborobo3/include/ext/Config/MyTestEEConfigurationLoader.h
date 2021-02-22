/*
 * MyTestEEConfigurationLoader.h
 */

#ifndef MYTESTEECONFIGURATIONLOADER_H
#define MYTESTEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class MyTestEEConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MyTestEEConfigurationLoader();
		~MyTestEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
