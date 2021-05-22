#pragma once

#include "common.h"
#include "DataListener.h"
#include "MyTestEEWorldObserver.h"

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
extern PlacementConfiguration gMscPlacementConfiguration;

// General evolver parameters.
extern double gMscMutationRate;
extern double gMscCrossoverRate;
extern double gMscTournamentSize;

// GNP
extern int gMscNbEachProcessingNode;
extern int gMscNbEachJudgementNode;
extern int gMscProcessT;
extern int gMscJudgeT;

extern bool gMscLogGnpNodeUsage;

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
extern unsigned int gMscNoveltySearchK;
extern double gMscNoveltySearchPMin;
extern bool gMscDynamicPMin;
extern unsigned int gMscNoveltySearch_No_Archiving_Stagnation_Treshold;
extern double gMscNoveltySearch_Pmin_lowering_multiplier;
extern double gMscNoveltySearch_Pmin_min;
extern unsigned int gMscNoveltySearch_Quick_Archiving_Min_Evaluations;
extern double gMscNoveltySearch_Pmin_raising_multiplier;
extern unsigned int gMscNoveltySearch_Recompute_Sparseness_Each;
