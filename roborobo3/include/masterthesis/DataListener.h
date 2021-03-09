#pragma once
#include <vector>
#include "../core/Agents/Robot.h"

struct DataPacket{
	std::vector<Robot*>* robots;
	int generation;
	float fitness;
};

// Interface
class DataListener{
	public:
		virtual void onGenerationDone(DataPacket* data) = 0;
		virtual void onSimulationDone() = 0;
};
