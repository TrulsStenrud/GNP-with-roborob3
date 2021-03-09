#include "MscMain.h"
#include "../../include/core/RoboroboMain/main2.h"
#include "DataForwarder.h"
#include "../../include/ext/Config/ForagingTempConfigurationLoader.h"

using namespace std;

MscMain::MscMain(int argc, char* argv[]){
	DataForwarder::getDataForwarder()->registerListener(this);
	ForagingTempConfigurationLoader::controllerType = ControllerEvolver::NEAT;
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
