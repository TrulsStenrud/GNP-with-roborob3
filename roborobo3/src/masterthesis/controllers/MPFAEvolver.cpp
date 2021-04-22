#include "MPFAEvolver.h"
#include "MPFAController.h"

using namespace std;

MPFAEvolver::MPFAEvolver(){
	std::string resultFile = gLogDirectoryname + "/" + "MPFA" + "_" + gStartTime + "_" + getpidAsReadableString() + ".csv";
    _logFile.open(resultFile);

    _logManager = new LogManager();
    _logManager->setLogFile(_logFile);
}

MPFAEvolver::~MPFAEvolver(){
	_logFile.close();
}

Controller* MPFAEvolver::make_Controller(RobotWorldModel* wm){
	return new MPFAController(wm);
}


void MPFAEvolver::evalDone(DataPacket* dp){
	cout<<"fitness: "<<dp->fitness<<endl;
	for(Robot* rob : *(dp->robots)){
		rob->reset();
	}
}


bool MPFAEvolver::usesBehavior(){
	return false;
}
