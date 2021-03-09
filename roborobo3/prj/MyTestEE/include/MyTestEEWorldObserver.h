/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef MYTESTEEWORLDOBSERVER_H
#define MYTESTEEWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"
#include "MyTestEE/include/NestObject.h"

class World;

extern std::vector<NestObject*> gNestObjects; // list of nestObjects added to gPhysicalObjects

class MyTestEEWorldObserver : public WorldObserver
{
public:
    MyTestEEWorldObserver(World *world);
    ~MyTestEEWorldObserver();
    
    void addNestObject(double x, double y);
    
    void initPre() override;
    void extracted();
    
    void initPost() override;

    void stepPre() override;
    void stepPost() override;
    
protected:
    void placeGridOfObjects(int x, int y, int columns, int rows);
    void placeObject(double x, double y);
    void placeRobotsInAllNests();
    void reset();
};

#endif
