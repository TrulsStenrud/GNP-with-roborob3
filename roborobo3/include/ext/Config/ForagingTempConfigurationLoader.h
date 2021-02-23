/*
 * ForagingTempConfigurationLoader.h
 */

#ifndef FORAGINGTEMPCONFIGURATIONLOADER_H
#define FORAGINGTEMPCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class ForagingTempConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		enum controller:int{GNP, NEAT, MPFA};
		controller controller;

		ForagingTempConfigurationLoader();
		~ForagingTempConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
