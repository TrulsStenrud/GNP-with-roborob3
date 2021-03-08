#include "DataForwarder.h"
#include "common.h"

using namespace std;

DataForwarder *DataForwarder::_instance = nullptr; // definerer variabelen slik at den får minne.

DataForwarder::DataForwarder(){
	_dataListeners = new std::vector<DataListener*>();
}

DataForwarder::~DataForwarder(){
	delete _dataListeners;
}

DataForwarder* DataForwarder::getDataForwarder(){

	if(_instance == nullptr){
		_instance = new DataForwarder();
	}
	return _instance;
}

void DataForwarder::registerListener(DataListener* dl){
	_dataListeners->push_back(dl);
}

bool DataForwarder::removeListener(DataListener* dl){
	for(auto it = _dataListeners->begin(); it != _dataListeners->end();){
		if(*it == dl){
			_dataListeners->erase(it);
			return true;
		}
	}
	return false;
}

void DataForwarder::forwardData(DataPacket* dp){
	for(auto l : *_dataListeners){
		l->onGenerationDone(dp);
	}
}

void DataForwarder::simulationDone(){
	for(auto l : *_dataListeners){
		l->onSimulationDone();
	}
}
