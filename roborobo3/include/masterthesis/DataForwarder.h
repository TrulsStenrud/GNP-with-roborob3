#pragma once

#include "DataListener.h"
#include <vector>

// This is a singleton.
class DataForwarder{
private:
	static DataForwarder* _instance;
	std::vector<DataListener*>* _dataListeners;
	DataForwarder();
public:
	static DataForwarder* getDataForwarder();
	~DataForwarder();
	void registerListener(DataListener* dl);
	bool removeListener(DataListener* dl);
	void forwardData(DataPacket* dp);
};

