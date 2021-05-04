#include "RoboroboMain/common.h"
#include "main.h"
#include "../masterthesis/MscMain.h"

/*
 * This class is purely for redirecting program flow to the "masterthesis" folder.
 */

#include "../../include/masterthesis/evolvers/GNPEvolver.h"
#include "../../include/GNP/GNPGenome.h"

int main(int argc, char* argv[]){
	// Needed for irace. Supresses console output.
//	std::cout.setstate(std::ios_base::failbit);
//	std::cerr.setstate(std::ios_base::failbit);

	MscMain* mscMain = new MscMain(argc, argv);
	delete mscMain;
	std::exit(0);
}


