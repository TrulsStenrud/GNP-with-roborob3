#include <stdlib.h>
#include <iostream>
#include "main.h"
#include "RoboroboMain/main2.h"
#include "../masterthesis/MscMain.h"

int main(int argc, char* argv[]){
	std::cout<<"hei"<<std::endl;
	MscMain* test;
	test = new MscMain();
	roboroboMain(argc, argv);
}
