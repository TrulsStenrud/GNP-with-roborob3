#pragma once

struct DataPacket{
	int generation;
	float bestFitness;
	float avgFitness;
};

class DataListener{
	public:
		virtual void onGenerationDone(DataPacket* data) = 0;
		virtual void onSimulationDone() = 0;
};
