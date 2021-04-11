#pragma once

#include "../../../include/core/Controllers/Controller.h"
#include "ControllerEvolver.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../../prj/MyTestEE/include/MyTestEEController.h"
#include "GNPGenome.h"
#include "Network.h"
#include "GNPEvolver.h"


class GNPController : public MyTestEEController{
    
private:
    std::vector<std::function<void(double)>>* getProcesses();
    std::vector<std::function<double()>>* getJudgements();
    double normalize(double num); // normalizes a number to [-1, 1]
    GNP::Network* _gnpNetwork;
public:
    GNPController(RobotWorldModel *wm, GNP::Genome* genome);
    ~GNPController();
    void reset() override;
    void step() override;
    void buildBrain(GNP::Genome genome);
    
    static NodeInformation getNodeLibrary();
};

