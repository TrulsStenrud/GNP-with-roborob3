/*
 *  CircleObject.h
 *  roborobo
 *
 *  Created by Nicolas on 25/4/2014.
 *
 */

#ifndef CIRCLEOBJECT_H
#define CIRCLEOBJECT_H

#include "World/PhysicalObject.h"

class CircleObject : public PhysicalObject
{

protected:
    
    double _radius; // radius. In pixels.
    double _footprintRadius; // radius of footprint, accessible to ground sensors. In pixels.
    Uint8 _footprintColorRed = 0xF0;
    Uint8 _footprintColorGreen = 0xF0;
    Uint8 _footprintColorBlue = 0xF0;
    
public :
    
    CircleObject( int __id ); // use PhysicalObjectFactory instead!
    ~CircleObject() { }

    void setFootprintColor( Uint8 r, Uint8 g, Uint8 b )
    {
        _footprintColorRed = r;
        _footprintColorGreen = g;
        _footprintColorBlue = b;
    }
    
    bool canRegister(); // test if register object is possible (use both shape or footprints)
    void registerObject(); // register object in the world (write images)
    void unregisterObject(); // unregister object in the world (write blank pixels)
    void show(SDL_Surface *surface = gScreen); // wrt. screen-rendering
    void hide();    // wrt. screen-rendering
    
};

#endif
