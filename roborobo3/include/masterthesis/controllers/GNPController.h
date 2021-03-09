#pragma once

#include "../../../include/core/Controllers/Controller.h"
#include "ControllerEvolver.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../../prj/MyTestEE/include/MyTestEEController.h"

class GNPController : public MyTestEEController{
    
private:
    ControllerEvolver::CONTROLLER _controllerType;
    std::vector<std::function<void(double)>>* getProcesses();
    std::vector<std::function<double()>>* getJudgements();
    double normalize(double num); // normalizes a number to [-1, 1]
public:
    GNPController(RobotWorldModel *wm, ControllerEvolver::CONTROLLER controllerType);
    ~GNPController();
    void reset() override;
    void step() override;
};
