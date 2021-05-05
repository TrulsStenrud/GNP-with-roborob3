#pragma once

#include "common.h"
#include "DataListener.h"


class MscMain : public DataListener{
	private:
		void initiateParameters();

	public:
		MscMain(int argc, char* argv[]);
		~MscMain();
		static void processArgument(char arg);
		void onGenerationDone(DataPacket* data) override;
		void onSimulationDone() override;
};

// Simulation parameters
extern std::string gMscLogName;
extern double gMscPheromoneDecay;
extern double gMscPheromoneEvaporationTreshold;

// General evolver parameters.
extern double gMscMutationRate;
extern double gMscCrossoverRate;
extern double gMscTournamentSize;

// GNP
extern double gMscNbParents;
extern int gMscNbEachProcessingNode;
extern int gMscNbEachJudgementNode;
extern int gMscProcessT;
extern int gMscJudgeT;

// GNP++
extern int gMscNbNeatNodes;
extern int gMscNeatT;

// MPFA
extern double gMscMutationModifier;

// NEAT
extern int gMscYoungAgeThreshold;
extern double gMscYoungAgeFitnessBoost;
extern int gMscOldAgeThreshold;
extern double gMscOldAgePenalty;
extern bool gMscPhasedSearching; // default false
extern bool gMscDeltaCoding; // default false. dependent on PhasedSearching being true.

// Novelty search
extern double gMscSampledAverageTimeWindow; // Parameter for the novelty measure.
