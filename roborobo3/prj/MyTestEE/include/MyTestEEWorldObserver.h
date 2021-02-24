/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef MYTESTEEWORLDOBSERVER_H
#define MYTESTEEWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class MyTestEEWorldObserver : public TemplateEEWorldObserver
{
public:
    MyTestEEWorldObserver(World *world);
    ~MyTestEEWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:    
    void monitorPopulation( bool localVerbose = true ) override;
    
private:
    void placeGridOfObjects(int x, int y, int columns, int rows);
    void placeObject(double x, double y);
};

#endif
