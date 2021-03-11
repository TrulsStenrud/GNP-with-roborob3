//
//  SampledAverageState.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 10/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "ForagingTemp/include/SampledAverageState.h"
#include "RoboroboMain/roborobo.h"

SampledAverageState::SampledAverageState(int timewindow){
    _timeWindow = timewindow;
    _counter = 0;
}

SampledAverageState::SampledAverageState(){
    gProperties.checkAndGetPropertyValue("gSampledAverageTimeWindow", &_timeWindow, true);
    _counter = 0;
}

SampledAverageState::~SampledAverageState(){
    
}

void SampledAverageState::addState(std::vector<double> state){
    if(_counter == 0){
        _behaviour.push_back(state);
    }
    else{
        for(int i = 0; i < state.size(); i++){
            _behaviour.back()[i] += state[i];
        }
    }
        
    _counter++;
    
    if (_counter % _timeWindow == 0){
        for(int i = 0; i < state.size(); i++){
            _behaviour.back()[i] /= _timeWindow;
        }
        _counter = 0;
    }
}

std::vector<std::vector<double>> SampledAverageState::getSampledAverage(){
    auto result = _behaviour;
    if(_counter != 0){
        for(int i = 0; i < result.back().size(); i++){
            result.back()[i] /= _counter;
        }
    }
    
    return result;
}
