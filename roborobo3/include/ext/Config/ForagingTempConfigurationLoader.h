/*
 * ForagingTempConfigurationLoader.h
 */

#ifndef FORAGINGTEMPCONFIGURATIONLOADER_H
#define FORAGINGTEMPCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "../../masterthesis/evolvers/ControllerEvolver.h"
#include "../../masterthesis/DataListener.h"
#include "../../masterthesis/DataForwarder.h"


class ForagingTempConfigurationLoader : public ConfigurationLoader, DataListener
{
	private:
		ControllerEvolver* _evolver;
	public:
		static ControllerEvolver::CONTROLLER controllerType;

		ForagingTempConfigurationLoader();
		~ForagingTempConfigurationLoader();

		void onGenerationDone(DataPacket* data) override;
		void onSimulationDone() override;

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
