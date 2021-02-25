#include "MscMain.h"
#include "../../include/core/RoboroboMain/main2.h"
#include "DataForwarder.h"

using namespace std;

MscMain::MscMain(int argc, char* argv[]){
	DataForwarder::getDataForwarder()->registerListener(this);
	roboroboMain(argc, argv);
}

MscMain::~MscMain(){
	DataForwarder::getDataForwarder()->removeListener(this);
	std::cout<<"main deleted"<<std::endl;
}

void MscMain::onGenerationDone(DataPacket* data){
	cout<<"packet received"<<endl;
}

void MscMain::onSimulationDone(){
	cout<<"simulation done"<<endl;
}
