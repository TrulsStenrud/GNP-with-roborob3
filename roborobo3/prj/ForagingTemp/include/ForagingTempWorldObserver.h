/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGTEMPWORLDOBSERVER_H
#define FORAGINGTEMPWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class ForagingTempWorldObserver : public TemplateEEWorldObserver
{
public:
    ForagingTempWorldObserver(World *world);
    ~ForagingTempWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:    
    void monitorPopulation( bool localVerbose = true ) override;
};

#endif
