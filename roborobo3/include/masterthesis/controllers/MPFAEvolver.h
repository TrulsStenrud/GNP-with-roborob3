#pragma once

#include "../../core/Agents/Robot.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../DataListener.h"
#include "ControllerEvolver.h"

class MPFAEvolver : public ControllerEvolver{
private:

public:
	MPFAEvolver();
	~MPFAEvolver();
	Controller* make_Controller(RobotWorldModel* wm) override;
	void evalDone(DataPacket* dp) override;
    bool usesBehavior() override;
};
