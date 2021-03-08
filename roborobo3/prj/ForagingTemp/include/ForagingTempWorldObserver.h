/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGTEMPWORLDOBSERVER_H
#define FORAGINGTEMPWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"
#include "../../../include/masterthesis/DataListener.h"
#include "../../../include/masterthesis/controllers/ControllerEvolver.h"
#include <vector>

class World;

class ForagingTempWorldObserver : public TemplateEEWorldObserver
{
private:
	int _evalTime; //how many time steps per evaluation.
	DataPacket* constructDataPacket();
	ControllerEvolver* _evolver;

public:
    ForagingTempWorldObserver(World *world);
    ~ForagingTempWorldObserver();
    void setControllerEvolver(ControllerEvolver* evolver);

    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:
    void monitorPopulation( bool localVerbose = true ) override;
};

#endif
