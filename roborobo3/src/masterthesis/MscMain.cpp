#include "MscMain.h"
#include "../../include/core/RoboroboMain/main2.h"
#include "DataForwarder.h"
#include "../../include/ext/Config/ForagingTempConfigurationLoader.h"

using namespace std;

MscMain::MscMain(int argc, char* argv[]){
	DataForwarder::getDataForwarder()->registerListener(this);
	initiateParameters();
	roboroboMain(argc, argv);
}

MscMain::~MscMain(){
	DataForwarder::getDataForwarder()->removeListener(this);

	std::cout<<"main deleted"<<std::endl;
}

void MscMain::onGenerationDone(DataPacket* data){
}

void MscMain::onSimulationDone(){
	cout<<"simulation done"<<endl;
}

void MscMain::processArgument(char c){
    std::cout << c << std::endl;
	switch(c){
		case 'a':
			if (optarg == std::string("GNP")){
				ForagingTempConfigurationLoader::controllerType = ControllerEvolver::GNP;
			}
			else if(optarg == std::string("NEAT")){
				ForagingTempConfigurationLoader::controllerType = ControllerEvolver::NEAT;
			}
			else if(optarg == std::string("NoveltySearch")){
				ForagingTempConfigurationLoader::controllerType = ControllerEvolver::NoveltySearch;
			}
			else if(optarg == std::string("GNP++")){
				ForagingTempConfigurationLoader::controllerType = ControllerEvolver::GNPPlusPLus;
			}
			else if(optarg == std::string("MPFA")){
				ForagingTempConfigurationLoader::controllerType = ControllerEvolver::MPFA;
			}
			break;

		case 'q':
			gMscPheromoneDecay = stod(optarg);
			break;
		case 'w':
			gMscPheromoneEvaporationTreshold = stod(optarg);
			break;
		case 'e':
			gMscMutationRate = stod(optarg);
			break;
		case 't':
			gMscCrossoverRate = stod(optarg);
			break;
		case 'y':
			gMscTournamentSize = stoi(optarg);
			break;
		case 'u':
			gMscLogGnpNodeUsage = true;
			break;
		case 'i':
			gMscNbEachProcessingNode = stoi(optarg);
			break;
		case 'p':
			gMscNbEachJudgementNode = stoi(optarg);
			break;
		case 'd':
			gMscMutationModifier = stod(optarg);
			break;
		case 'f':
			gMscYoungAgeThreshold = stoi(optarg);
			break;
		case 'g':
			gMscYoungAgeFitnessBoost = stod(optarg);
			break;
		case 'j':
			gMscOldAgeThreshold = stoi(optarg);
			break;
		case 'k':
			gMscOldAgePenalty = stod(optarg);
			break;
		case 'z':
			gMscPhasedSearching = stoi(optarg);
			break;
		case 'x':
			gMscDeltaCoding = stoi(optarg);
			break;
		case 'c':
			gMscSampledAverageTimeWindow = stoi(optarg);
			break;
		case 'n':
			gMscLogName = static_cast<std::string>(optarg);
			break;
        case 'm':
            gMscJudgeT =stoi(optarg);
            break;
        case '1':
            gMscProcessT =stoi(optarg);
            break;
        case '2':
            gMscNeatT =stoi(optarg);
            break;
        case '3':
            gMscNbNeatNodes = stoi(optarg);
            break;
        case '4':
            gMscPlacementConfiguration = static_cast<PlacementConfiguration>(stoi(optarg));
        case '5':
            gMscDynamicPMin = stoi(optarg);
            break;
        case '6':
            gMscNoveltySearchK = stoi(optarg);
            break;
        case '7':
            gMscNoveltySearchPMin = stod(optarg);
            break;
        case '8':
            gMscNoveltySearch_No_Archiving_Stagnation_Treshold = stoi(optarg);
            break;
        case '9':
            gMscNoveltySearch_Pmin_lowering_multiplier = stod(optarg);
            break;
        case ']':
            gMscNoveltySearch_Pmin_min = stod(optarg);
            break;
        case '[':
            gMscNoveltySearch_Quick_Archiving_Min_Evaluations = stoi(optarg);
            break;
        case '}':
            gMscNoveltySearch_Pmin_raising_multiplier = stod(optarg);
            break;
        case '{':
            gMscNoveltySearch_Recompute_Sparseness_Each = stoi(optarg);
            break;
	}
}

// Import from properties file.
void MscMain::initiateParameters(){
	//The holy regex:  Find: \<gMsc\([a-zA-Z]*\) Replace with: gProperties.checkAndGetPropertyValue("\0", &\0, false);

	// Simulation parameters
	gProperties.checkAndGetPropertyValue("gMscPheromoneDecay", &gMscPheromoneDecay, false);
	gProperties.checkAndGetPropertyValue("gMscPheromoneEvaporationTreshold", &gMscPheromoneEvaporationTreshold, false);

	// General evolver parameters.
	gProperties.checkAndGetPropertyValue("gMscMutationRate", &gMscMutationRate, false);
	gProperties.checkAndGetPropertyValue("gMscCrossoverRate", &gMscCrossoverRate, false);
	gProperties.checkAndGetPropertyValue("gMscTournamentSize", &gMscTournamentSize, false);
    
    int temp = -1;
    gProperties.checkAndGetPropertyValue("gMscPlacementConfiguration", &temp, false);
    if(temp != -1){
        gMscPlacementConfiguration = static_cast<PlacementConfiguration>(temp);
        std::cout << "walla balla" << std::endl;
    }
    

	// GNP
	gProperties.checkAndGetPropertyValue("gMscNbEachProcessingNode", &gMscNbEachProcessingNode, false);
	gProperties.checkAndGetPropertyValue("gMscNbEachJudgementNode", &gMscNbEachJudgementNode, false);
    
    // GNP++
    gProperties.checkAndGetPropertyValue("gMscNbNeatNodes", &gMscNbNeatNodes, false);

	// MPFA
	gProperties.checkAndGetPropertyValue("gMscMutationModifier", &gMscMutationModifier, false);

	// NEAT
	gProperties.checkAndGetPropertyValue("gMscYoungAgeThreshold", &gMscYoungAgeThreshold, false);
	gProperties.checkAndGetPropertyValue("gMscYoungAgeFitnessBoost", &gMscYoungAgeFitnessBoost, false);
	gProperties.checkAndGetPropertyValue("gMscOldAgeThreshold", &gMscOldAgeThreshold, false);
	gProperties.checkAndGetPropertyValue("gMscOldAgePenalty", &gMscOldAgePenalty, false);
	gProperties.checkAndGetPropertyValue("gMscPhasedSearching", &gMscPhasedSearching, false); // default false
	gProperties.checkAndGetPropertyValue("gMscDeltaCoding", &gMscDeltaCoding, false); // default false. dependent on PhasedSearching being true.

	// Novelty search
	gProperties.checkAndGetPropertyValue("gMscSampledAverageTimeWindow", &gMscSampledAverageTimeWindow, false); // Parameter for the novelty measure.}
}




// Simulation parameters
std::string gMscLogName = "";
double gMscPheromoneDecay = 0.001;
double gMscPheromoneEvaporationTreshold = 0.08;


// General evolver parameters.
double gMscMutationRate = 0.25;
double gMscCrossoverRate = 0.7;
double gMscTournamentSize = 3;
PlacementConfiguration gMscPlacementConfiguration = PlacementConfiguration::SemiCluster;

// GNP
int gMscNbEachProcessingNode = 8;
int gMscNbEachJudgementNode = 4;
int gMscProcessT = 5;
int gMscJudgeT = 1;

bool gMscLogGnpNodeUsage = false;

// GNP++
int gMscNbNeatNodes = 3;
int gMscNeatT = 1;


// MPFA
double gMscMutationModifier = 0.5;

// NEAT
int gMscYoungAgeThreshold = 5;
double gMscYoungAgeFitnessBoost = 1.1;
int gMscOldAgeThreshold = 30;
double gMscOldAgePenalty = 0.5;
bool gMscPhasedSearching = false;
bool gMscDeltaCoding = false; // dependent on PhasedSearching being true.

// Novelty search
double gMscSampledAverageTimeWindow = 50; // Parameter for the novelty measure.}
unsigned int gMscNoveltySearchK = 15;
double gMscNoveltySearchPMin = 0.5;
bool gMscDynamicPMin = true;
unsigned int gMscNoveltySearch_No_Archiving_Stagnation_Treshold = 150;
double gMscNoveltySearch_Pmin_lowering_multiplier = 0.9;
double gMscNoveltySearch_Pmin_min = 0.05;
unsigned int gMscNoveltySearch_Quick_Archiving_Min_Evaluations = 8;
double gMscNoveltySearch_Pmin_raising_multiplier = 1.1;
unsigned int gMscNoveltySearch_Recompute_Sparseness_Each = 10;

