//
//  PheromoneObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 25/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "../include/PheromoneObject.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include <Utilities/Graphics.h>
#include "MyTestEE/include/ObjectFactory.h"



PheromoneObject::PheromoneObject(int __id) : CircleObject( __id ){
    setType(0);
    setFootprintColor(0xA0, 0xF0, 0xA0);
    unregisterObject();
    _radius=0;
    _footprintRadius = 4;
    _trail = nullptr;

    gProperties.checkAndGetPropertyValue("gPheromoneDecay", &_decay, true);
	gProperties.checkAndGetPropertyValue("gPheromoneEvaporationTreshold", &_evaporationTreshold, true);


    _strength = 1;
    regrowTime = -1;

}

std::vector<std::tuple<double,double>>* PheromoneObject::getTrail(){
	if(_trail == nullptr){
		_trail = new std::vector<std::tuple<double,double>>();
	}
	return _trail;
}

void PheromoneObject::setTrail(std::vector<std::tuple<double,double>>* trail){
	_trail = trail;
}

std::tuple<double,double> PheromoneObject::getPosition(){
	return std::make_tuple(_xReal, _yReal);
}

void PheromoneObject::step()
{
    if(_strength > _evaporationTreshold){
        _strength *= (1 - _decay);
        if(_strength <= _evaporationTreshold){
            if(registered && _visible){
                evaporate();
            }
        }

    }
}

void PheromoneObject::evaporate(){
    _strength = 0;
    _visible = false;
    registered = false;
    delete _trail;
    unregisterObject();
    hide();
    ObjectFactory::recyclePheromoneObject(this);
}

void PheromoneObject::makeVisible(){
    _strength = 1;
    _visible = true;
    registered = true;
    registerObject();
}

bool PheromoneObject::isRegistered(){
    return registered;
}
void PheromoneObject::updateStrength(){
    if(!registered){
        // TODO what to do in the one frame where pheroone is evaporated, but still readable through the groundsensor, just read strength as zero maybe
    }
    else{
        _strength = 1;
    }
}

double PheromoneObject::getStrength(){
    if(registered){ // the pheromones is readable through the ground sensor for the remaining of the frame where it is evaporated... this is a "fix" for this.
        return _strength;
    }
    else{
        return 0;
    }
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

