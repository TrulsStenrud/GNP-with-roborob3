/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGTEMPAGENTOBSERVER_H
#define FORAGINGTEMPAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class ForagingTempAgentObserver : public TemplateEEAgentObserver
{
	public:
		ForagingTempAgentObserver(RobotWorldModel *wm);
		~ForagingTempAgentObserver();
    
        void stepPre() override;
};

#endif

