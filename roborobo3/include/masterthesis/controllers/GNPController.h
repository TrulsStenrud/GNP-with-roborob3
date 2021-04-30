#ifndef GNPController_h
#define GNPController_h

#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../../prj/MyTestEE/include/MyTestEEController.h"
#include "../../GNP/Network.h"
#include "../../MultiNEAT/NeuralNetwork.h"
#include "../../GNP/GNPGenome.h"
#include "../../MultiNEAT/Genome.h"


class GNPController : public MyTestEEController{

private:
    
    std::vector<NEAT::NeuralNetwork*> _neatNetworks;
    std::vector<double> buildInputVector();
    void applyOutputVector(std::vector<double> output);
    GNP::Network* _gnpNetwork;
    int judgeObjectTypeForSensors(std::vector<int> sensors);
    
public:
    GNPController(RobotWorldModel *wm, GNP::Genome& genome, std::vector<NEAT::Genome> neatGenomes={});
    ~GNPController();
    void reset() override;
    void step() override;
    
    void buildBrain(GNP::Genome& genome, std::vector<NEAT::Genome> neatGenomes={});
    GNP::Network* getNetwork();
    
    int judge(int judgeIndex);
    void process(int processIndex, double value);
    void processNeat(int neatIndex);

    static GNP::NodeInformation getNodeLibrary();
};

#endif /* GNPController_h */
