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

enum PlacementConfiguration:int{Random=0, SemiCluster=1, Cluster=2, MIX=3};

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
    int _nbForagingObjects = 0;
    void clusteredForagingObjects();
    void semiClusteredForagingObjects();
    void randomForagingObjects(int n);
    void placeGridOfObjects(int x, int y, int columns, int rows);
    void placeRobotsInAllNests();
    void reset(PlacementConfiguration configuration);
};

#endif
