//
//  Logger.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 12/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include <stdio.h>
#include "common.h"
#include "../core/RoboroboMain/roborobo.h"

class Logger{
  private:
    std::ofstream _logFile;
    LogManager *_logManager = NULL;
    std::string _resultFile;
    bool firstInLine = true;
public:
    Logger(std::string name);
    ~Logger();
    void log(double value);
    void log(std::string text);
    void newLine();
};

#endif /* Logger_hpp */
