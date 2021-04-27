/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef MYTESTEECONTROLLER_H
#define MYTESTEECONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"
#include "Utilities/Packet.h"
#include "MyTestEE/include/PheromoneObject.h"

using namespace Neural;

class RobotWorldModel;

class MyTestEEController : public Controller
{
public:

    MyTestEEController(RobotWorldModel *wm);
    ~MyTestEEController();

    void setCarrying(int objectId);
    bool isCarrying();
    void unLoad();
    PheromoneObject* dropPheromone();
    double getPheromoneValue();

    // returns the robots orientation relative to the closest nest mappen to [ -1, +1 ]
    double getNestRelativeOrientation();

    void step();
    void reset();

protected:
    int _carriedObjectId;
private:
    PheromoneObject* _testPheromone;
};


#endif

