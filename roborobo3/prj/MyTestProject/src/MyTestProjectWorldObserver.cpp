/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "MyTestProject/include/MyTestProjectWorldObserver.h"
#include "World/World.h"


MyTestProjectWorldObserver::MyTestProjectWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

MyTestProjectWorldObserver::~MyTestProjectWorldObserver()
{
	// nothing to do.
}


void MyTestProjectWorldObserver::initPre()
{
    // nothing to do.
}

void MyTestProjectWorldObserver::initPost()
{
    // nothing to do.
}

void MyTestProjectWorldObserver::stepPre()
{
	// nothing to do.
}

void MyTestProjectWorldObserver::stepPost()
{
    // nothing to do.
}
