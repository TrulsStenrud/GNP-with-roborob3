//
//  Logger.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 12/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "Logger.h"
#include "RoboroboMain/roborobo.h"


Logger::Logger(std::string name){
    std::string resultFile = gLogDirectoryname + "/" + name + "_" + gStartTime + "_" + getpidAsReadableString() + ".csv";
    
    _logFile.open(resultFile);
    
    _logManager = new LogManager();
    _logManager->setLogFile(_logFile);
}
Logger::~Logger(){
    _logFile.close();
    delete _logManager;
}

void Logger::log(double value){
    auto str = std::to_string(value);
    // remove trailing zeroes
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    _logManager->write(";" +  str);
    
}

void Logger::log(std::string text){
    _logManager->write(text);
}

void Logger::newLine(){
    _logManager->write("\n");
    _logManager->flush();
    
}

