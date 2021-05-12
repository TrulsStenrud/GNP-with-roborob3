//
//  Logger.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 12/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "Logger.h"
#include "RoboroboMain/roborobo.h"
#include "../../include/masterthesis/MscMain.h"


Logger::Logger(std::string name){
    _resultFile = gLogDirectoryname + "/" + name + "_" + gStartTime + "_" + getpidAsReadableString() + ".notDone.csv";
    if(gMscLogName != "")
		_resultFile = gLogDirectoryname + "/" + gMscLogName + ".notDone.csv";

    _logFile.open(_resultFile);

    _logManager = new LogManager();
    _logManager->setLogFile(_logFile);
}
Logger::~Logger(){
    _logFile.close();
    delete _logManager;
    std::string oldName = _resultFile;
    _resultFile.erase(_resultFile.find(".notDone"), 8);
    std::cout<<oldName<<std::endl;
    std::cout<<_resultFile<<std::endl;
    rename(oldName.c_str(), _resultFile.c_str());
}

void Logger::log(double value){
    auto str = std::to_string(value);
    // remove trailing zeroes
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    log(str);
}

void Logger::log(std::string text){
    if(firstInLine){
        firstInLine = false;
    }else{
        text = ";" + text;
    }

    _logManager->write(text);
}

void Logger::newLine(){
    _logManager->write("\n");
    _logManager->flush();
    firstInLine = true;
}

