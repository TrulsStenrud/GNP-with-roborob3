#pragma once
#include <vector>
#include "../core/Agents/Robot.h"

struct DataPacket{
	int generation = 0;
	float fitness = 0;
    float foragingPercentage = 0;
    std::vector<std::vector<double>> behaviorData;
};

// Interface
class DataListener{
	public:
		virtual void onGenerationDone(DataPacket* data) = 0;
		virtual void onSimulationDone() = 0;
};
