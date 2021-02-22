/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef MYTESTEEAGENTOBSERVER_H
#define MYTESTEEAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class MyTestEEAgentObserver : public TemplateEEAgentObserver
{
	public:
		MyTestEEAgentObserver(RobotWorldModel *wm);
		~MyTestEEAgentObserver();
    
        void stepPre() override;
};

#endif

