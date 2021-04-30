#include "RoboroboMain/common.h"
#include "main.h"
#include "../masterthesis/MscMain.h"

/*
 * This class is purely for redirecting program flow to the "masterthesis" folder.
 */

#include "../../include/masterthesis/evolvers/GNPEvolver.h"
#include "../../include/GNP/GNPGenome.h"

int main(int argc, char* argv[]){
    
	MscMain* mscMain = new MscMain(argc, argv);
	delete mscMain;
	std::exit(0);
}


