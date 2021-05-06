#include "RoboroboMain/common.h"
#include "main.h"
#include "../masterthesis/MscMain.h"

/*
 * This class is purely for redirecting program flow to the "masterthesis" folder.
 */

#include "../../include/masterthesis/evolvers/GNPEvolver.h"
#include "../../include/GNP/GNPGenome.h"

int getRandom(std::vector<int>& genes){
    int r = randint() % genes.size();

    int result = genes[r];
    genes.erase(genes.begin() + r);

    return result;
}
int main(int argc, char* argv[]){
//    Needed for irace. Supresses console output.
    
	MscMain* mscMain = new MscMain(argc, argv);
	delete mscMain;
	std::exit(0);

}


