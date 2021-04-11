/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGTEMPWORLDOBSERVER_H
#define FORAGINGTEMPWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"
#include "SampledAverageState.h"
#include "../../../include/masterthesis/DataListener.h"
#include "../../../include/masterthesis/evolvers/ControllerEvolver.h"
#include <vector>
#include "../../MyTestEE/include/MyTestEEWorldObserver.h"

class World;

class ForagingTempWorldObserver : public MyTestEEWorldObserver
{
private:
	int _evalTime; //how many time steps per evaluation.
	DataPacket* constructDataPacket();
	ControllerEvolver* _evolver;
    bool _captureBehavior = true;
    SampledAverageState* _sampledState = NULL;

public:
    ForagingTempWorldObserver(World *world);
    ~ForagingTempWorldObserver();
    void setControllerEvolver(ControllerEvolver* evolver);
    void stepPre() override;
    
    double getFitness();
};

#endif
