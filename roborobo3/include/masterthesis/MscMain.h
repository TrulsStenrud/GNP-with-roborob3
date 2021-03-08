#pragma once

#include "common.h"
#include "DataListener.h"


class MscMain : public DataListener{

	public:
		MscMain(int argc, char* argv[]);
		~MscMain();
		void onGenerationDone(DataPacket* data) override;
		void onSimulationDone() override;
};

