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
    void dropPheromone();
    bool isPheromone();
    
    void step();
    void reset();
    
protected:
    
private:
    int _carriedObjectId;
    PheromoneObject* _testPheromone;
};


#endif

