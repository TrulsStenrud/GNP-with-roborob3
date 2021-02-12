/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#ifndef MYTESTPROJECTWORLDOBSERVER_H
#define MYTESTPROJECTWORLDOBSERVER_H

#include "Observers/WorldObserver.h"

class World;

class MyTestProjectWorldObserver : public WorldObserver
{
	protected:
		
	public:
		MyTestProjectWorldObserver( World *__world );
		~MyTestProjectWorldObserver();
				
		void initPre();
        void initPost();
    
		void stepPre();
		void stepPost();
    
};

#endif

