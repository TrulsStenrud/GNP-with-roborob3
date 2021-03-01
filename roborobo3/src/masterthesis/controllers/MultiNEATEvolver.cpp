#include "MultiNEATEvolver.h"
#include "ControllerEvolver.h"

MultiNEATEvolver::MultiNEATEvolver(ControllerEvolver::CONTROLLER controllerType){
	_contType = controllerType;
}

MultiNEATEvolver::~MultiNEATEvolver(){

}

void MultiNEATEvolver::generationalUpdate(std::vector<Robot*> robots){
	std::cout<<"Evolver genUpdate called. Type: "<<_contType<<std::endl;
}
