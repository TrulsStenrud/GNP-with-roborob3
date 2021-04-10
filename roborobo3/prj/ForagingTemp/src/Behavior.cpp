//
//  Behavior.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "../include/Behavior.h"

Behavior::Behavior(std::vector<std::vector<double>> data){
    m_Data = data;
}

Behavior::~Behavior(){
}

bool  Behavior::Acquire(NEAT::Genome *aGenome){
    return true;
}

double Behavior::Distance_To(PhenotypeBehavior *a_Other){
    double distance = 0;

    auto a = m_Data;
    auto b = a_Other->m_Data;

    if(a.size()!=b.size()){
        std::cout << "[ALERT] behavior data of different size can not be compared" << std::endl;
        exit(-1);
    }

    for(int i = 0; i < a.size(); i++){
        auto d = a[i];
        auto e = b[i];

        if(d.size()!=e.size()){
            std::cout << "[ALERT] behavior data of different size can not be compared" << std::endl;
            exit(-1);
        }

        for(int j = 0; j < d.size(); j++){
            distance+= abs( d[j] - e[j] );
        }
    }


    return distance;
}
