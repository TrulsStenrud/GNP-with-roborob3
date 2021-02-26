//
//  PheromoneObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 25/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "PheromoneObject.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include <Utilities/Graphics.h>



PheromoneObject::PheromoneObject(int __id) : CircleObject( __id ){
    setType(0);
    setFootprintColor(0xA0, 0xF0, 0xA0);
    unregisterObject();
    _radius=0;
    _footprintRadius = 4;
}


void PheromoneObject::step()
{
    stepPhysicalObject();
}

void PheromoneObject::isWalked(int __idAgent){

}


bool PheromoneObject::canRegister()
{
    
    //  test footprint (pixels from only ground image, not environment image must be empty)
    // Code stolen from CircleObject, and removed collision with enviroment image
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                Uint32 pixelFootprint = getPixel32_secured( gFootprintImage, xColor, yColor);
                if (
                        ( gFootprintImage_restoreOriginal == true  && pixelFootprint != getPixel32_secured( gFootprintImageBackup, xColor, yColor ) ) || // case: ground as initialized or rewritten (i.e. white)
                        ( gFootprintImage_restoreOriginal == false && pixelFootprint != SDL_MapRGBA( gFootprintImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) // case: only white ground
                   )
                    return false; // collision!
            }
        }
    }
    
    return true;
}
void PheromoneObject::isTouched(int __idAgent){
    
}

void PheromoneObject::isPushed(int __idAgent, std::tuple<double, double> __speed){
}

