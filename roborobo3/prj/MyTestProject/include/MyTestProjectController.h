/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef MYTESTPROJECTCONTROLLER_H
#define MYTESTPROJECTCONTROLLER_H

#include "Controllers/Controller.h"
#include "RoboroboMain/common.h"

class RobotWorldModel;

class MyTestProjectController : public Controller
{
	public:
		MyTestProjectController( RobotWorldModel *__wm );
		~MyTestProjectController();
		
		void reset();
		void step();

};


#endif

