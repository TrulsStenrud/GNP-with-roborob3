/*
 * MyTestProjectConfigurationLoader.h
 */

#ifndef MYTESTPROJECTCONFIGURATIONLOADER_H
#define	MYTESTPROJECTCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class MyTestProjectConfigurationLoader : public ConfigurationLoader
{
	public:
		MyTestProjectConfigurationLoader();
		~MyTestProjectConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};

#endif
