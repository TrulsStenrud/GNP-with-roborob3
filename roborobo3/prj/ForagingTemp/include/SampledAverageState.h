//
//  SampledAverageState.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 10/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef SampledAverageState_hpp
#define SampledAverageState_hpp
#include <vector>

#include <stdio.h>

class SampledAverageState{
private:
    std::vector<std::vector<double>> _behaviour;
    int _counter;
    int _timeWindow;
    
public:
    SampledAverageState();
    SampledAverageState(int timewindow);
    ~SampledAverageState();
    void addState(std::vector<double> state);
    std::vector<std::vector<double>> getSampledAverage();
    void reset();
};


#endif /* SampledAverageState_hpp */
