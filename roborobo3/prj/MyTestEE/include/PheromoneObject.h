//
//  PheromoneObject.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 25/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef PheromoneObject_hpp
#define PheromoneObject_hpp

#include <stdio.h>
#include "World/CircleObject.h"



class PheromoneObject : public CircleObject {

public:
    PheromoneObject( int __id );
    ~PheromoneObject() {}

    double getStrength();
    void evaporate();
    void makeVisible();
    void updateStrength();
    bool isRegistered();

    void step() override;
    void isWalked ( int __idAgent ) override; // callback, triggered by agent
    void isTouched ( int __idAgent ) override; // callback, triggered by agent
    void isPushed( int __id, std::tuple<double, double> __speed ) override; // callback, triggered by collision w/ agent/object
    bool canRegister() override; // test if register object is possible (use only footprint)

    // MPFA methods.
    std::vector<std::tuple<double,double>>* getTrail();
    void setTrail(std::vector<std::tuple<double,double>>* trail);
    std::tuple<double,double> getPosition();
private:
    double _decay;
    double _evaporationTreshold;
    double _strength;

    // MPFA fields
    std::vector<std::tuple<double,double>>* _trail;
};
#endif /* PheromoneObject_hpp */
