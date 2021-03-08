/*
 * ForagingTempConfigurationLoader.h
 */

#ifndef FORAGINGTEMPCONFIGURATIONLOADER_H
#define FORAGINGTEMPCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "../../masterthesis/controllers/ControllerEvolver.h"



class ForagingTempConfigurationLoader : public ConfigurationLoader
{
	private:
		ControllerEvolver* _evolver;
	public:
		static ControllerEvolver::CONTROLLER controllerType;

		ForagingTempConfigurationLoader();
		~ForagingTempConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
