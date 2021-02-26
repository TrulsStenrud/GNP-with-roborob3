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
		enum CONTROLLER:int{GNP, NEAT, MPFA};
		static CONTROLLER controller;

		ForagingTempConfigurationLoader();
		~ForagingTempConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
