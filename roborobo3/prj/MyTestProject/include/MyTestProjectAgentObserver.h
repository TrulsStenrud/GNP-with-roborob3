/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef MYTESTPROJECTAGENTOBSERVER_H
#define MYTESTPROJECTAGENTOBSERVER_H 

#include "Observers/AgentObserver.h"

class RobotWorldModel;

class MyTestProjectAgentObserver : public AgentObserver
{
	public:
		MyTestProjectAgentObserver( );
		MyTestProjectAgentObserver( RobotWorldModel *__wm );
		~MyTestProjectAgentObserver();
				
		void reset() override;
		void stepPre() override;
		
};


#endif

