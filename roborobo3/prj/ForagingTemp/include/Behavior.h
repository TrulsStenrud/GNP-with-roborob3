//
//  Behavior.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Behavior_hpp
#define Behavior_hpp

#include <stdio.h>
#include "../../include/MultiNEAT/Genome.h"

class Behavior : public NEAT::PhenotypeBehavior {
private:
public:
    Behavior(std::vector<std::vector<double>> data);
    ~Behavior();
    bool Acquire(NEAT::Genome* aGenome);
    double Distance_To(PhenotypeBehavior* a_Other);
    
};


#endif /* Behavior_hpp */
