#include "MPFAController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/Utilities/Misc.h"
#include "../../../prj/MyTestEE/include/MyTestEEWorldObserver.h"
#include "../../../include/core/World/PhysicalObject.h"

using namespace std;

MPFAController::MPFAController(RobotWorldModel *wm):
    DistanceTolerance(5.0),
    SearchStepSize(10.0),
    RobotForwardSpeed(1.0),
    RobotRotationSpeed(1.0),
    isInformed(false),
    searchTime(0),
    resourceDensity(0),
    fidelity(10000,10000), //qilu 07/04
    updateFidelity(false), //qilu 07/29
    MPFA(INACTIVE),
    probSwitchToSearch(1), // 0.99999
	ProbabilityOfReturningToNest(0.0), // 0.00001
	UninformedSearchVariation(0.231256126), // 0.231256126
	RateOfInformedSearchDecay(0.05),
    RateOfSiteFidelity(10.0),
    RateOfLayingPheromone(10.0),
    MyTestEEController(wm)
{
	gProperties.checkAndGetPropertyValue("gScreenWidth", &x_max, true);
	gProperties.checkAndGetPropertyValue("gScreenHeight", &y_max, true);
	trailToShare = new vector<tuple<double,double>>();
	trailToFollow = new vector<tuple<double,double>>();
	prevPos = make_tuple(_wm->getXReal(), _wm->getYReal());
}

MPFAController::~MPFAController(){

}

void MPFAController::setParameters(MPFAGenome params){
	probSwitchToSearch = params.values[0];
	ProbabilityOfReturningToNest = params.values[1];
	UninformedSearchVariation = params.values[2];
	RateOfInformedSearchDecay = params.values[3];
	RateOfSiteFidelity = params.values[4];
	RateOfLayingPheromone = params.values[5];
}

void MPFAController::reset() {
    isInformed      = false;
    searchTime      = 0;
    resourceDensity = 0;
    MPFA            = INACTIVE;

    target   = getClosesNestPos();
    fidelity = getClosesNestPos();

    trailToShare->clear();
    trailToFollow->clear();
    prevPos = make_tuple(_wm->getXReal(), _wm->getYReal());
    MyTestEEController::reset();
}


void MPFAController::step(){

	switch(MPFA) {
        /* state at the start or reset of a simulation, start departing() */
        case INACTIVE:
            inactive();
            break;
        /* depart from nest after food drop off (or at simulation start) */
        case DEPARTING:
            departing();
            break;
        /* after departing(), once conditions are met, begin searching() */
        case SEARCHING:
            searching();
            break;
        /* return to nest after food pick up or giving up searching() */
        case RETURNING:
            returning();
            break;
        /* all food is picked up, return to the nest and turn off motors */
        case SHUTDOWN:
            shutdown();
    }
    prevPos = make_tuple(_wm->getXReal(), _wm->getYReal());
}

/*****
 * The initial state of the MPFA controller.
 * [1] pick a random target location at the edge of the arena
 * [2] start robot motors and begin MPFA-departing state
 *****/
void MPFAController::inactive(){
	SetRandomSearchLocation();
	MPFA = DEPARTING;
}

void MPFAController::departing(){
	int x = _wm->getXReal(), y = _wm->getYReal();
	int diffx = get<0>(target)-x, diffy = get<1>(target)-y;

    /* Are we informed? I.E. using site fidelity or pheromones. */
    if(isInformed) {
        /* When informed, proceed to the target location. */
        if(hypot(diffx,diffy) < DistanceTolerance){
            searchTime = 0;
            MPFA = SEARCHING;
		}
    } else {
        /* When not informed, continue to travel until randomly switching
           to the searching state. */
        double randomNumber = rand()%10000 / 10000.0;
        if(_wm->getWorld()->getIterations() % 300 == 0 && randomNumber < probSwitchToSearch){
            searchTime = 0;
    	    MPFA = SEARCHING;
    	    target = make_tuple(x,y);
    	    /*qilu 07/04/2015 this sets the current location to be the search location,
    	     then the robot can start to search*/
    	}
    	else if(hypot(diffx,diffy) < DistanceTolerance){
            SetRandomSearchLocation();
        }
    }
	FindFood();
    /* Adjust motor speeds and direction based on the target position. */
    ApproachTheTarget();
}

void MPFAController::searching(){
	if(!isCarrying()) {
		int x = _wm->getXReal(), y = _wm->getYReal();
		int diffx = x - get<0>(target), diffy = y - get<1>(target);
		double random = rand()%10000 / 10000.0;
        /* randomly give up searching */
		if(_wm->getWorld()->getIterations()%300 == 0 && random < ProbabilityOfReturningToNest){
            target = getClosesNestPos();
			fidelity= make_tuple(10000,10000); //qilu 07/28
			updateFidelity = false; //qilu 07/29
            MPFA = RETURNING;
        }
        /* If we reached our target search location, set a new one. The
           new search location calculation is different based on wether
           we are currently using informed or uninformed search. */
		else{
            double rand = randgaussian() * UninformedSearchVariation;
            /* uninformed search */
            if(!isInformed) {
                double turn_angle = getOrientation()+rand;
                double movex = cos(turn_angle*PI) * SearchStepSize;
                double movey = sin(turn_angle*PI) * SearchStepSize;

                SetTargetInBounds(make_tuple(_wm->getXReal()+movex, _wm->getYReal()+movey));
			}
            /* informed search */
            else{
                double t           = searchTime++;// qilu 01/20/2016 time in 5 seconds for search decay
                double isd = RateOfInformedSearchDecay;
				double correlation = GetExponentialDecay(rand, t, isd);

				double rotation = GetBound(correlation, -PI, PI);
                double turn_angle = getOrientation()+rotation;
				double movex = cos(turn_angle*PI) * SearchStepSize;
                double movey = sin(turn_angle*PI) * SearchStepSize;
                SetTargetInBounds(make_tuple(_wm->getXReal()+movex, _wm->getYReal()+movey));
            }
        }
    }
    /* Food has been found, change state to RETURNING and go to the nest */
    else {
        target = getClosesNestPos();
        SetLocalResourceDensity();    //qilu 07/17
        MPFA = RETURNING;
    }
    /* Adjust motor speeds and direction based on the target position. */

    FindFood();

    ApproachTheTarget();

}

void MPFAController::returning(){
	FindFood();
    /* Are we there yet? (To the nest, that is.) */
    if(isInNest()){
        /* Based on a Poisson CDF, the robot may or may not create a pheromone
           located at the last place it picked up food. */
        double poissonCDF_pLayRate    = GetPoissonCDF(resourceDensity, RateOfLayingPheromone);
        double poissonCDF_sFollowRate = GetPoissonCDF(resourceDensity, RateOfSiteFidelity);
        double r1 = rand()%1000 / 1000.0;
        double r2 = rand()%1000 / 1000.0;
        //create a pheromone trail if there is a new site fidelity

		if(poissonCDF_pLayRate > r1 && updateFidelity){//qilu 07/29
			ImaginaryPheromone phero;
			phero.trail = trailToShare;
			phero.strength = 1;
			getClosestNest()->PheromoneList->push_back(phero);
		}
		trailToShare->clear();//qilu 07/07 reset the temporary variable
        /* Determine probabilistically wether to use site fidelity, pheromone
           trails, or random search. */

        /* use site fidelity */
        if(poissonCDF_sFollowRate > r2 && fidelity!=make_tuple(10000, 10000)) { // qilu 07/07, consider the case of no site fidelity
			target = fidelity;
			isInformed = true;
		}
        /* use pheromone waypoints */
        else if(SetTargetPheromone()) {
			isInformed = true;
		}
        /* use random search */
        else {
			SetRandomSearchLocation();
			isInformed = false;
		}
		MPFA = DEPARTING;
	}
    /* Adjust motor speeds and direction based on the target position. */
    ApproachTheTarget();
}

/*****
 * SHUTDOWN: If all food items have been collected, the simulation is over
 * and all iAnts are instructed to return to the nest and shut down.
 *****/
void MPFAController::shutdown(){
	target = getClosesNestPos();
	searchTime++;
    MPFA = INACTIVE;
}


/*****
 * Check if the iAnt is finding food. This is defined as the iAnt being within
 * the distance tolerance of the position of a food item. If the iAnt has found
 * food then the appropriate boolean flags are triggered.
 *****/
void MPFAController::FindFood() {
    if(!isCarrying()) {
		for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
		{
			int objectId = _wm->getObjectIdFromCameraSensor(i);
			if ( gSensoryInputs_physicalObjectType )
			{
				if ( PhysicalObject::isInstanceOf(objectId) )
				{
					if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
					{
						PhysicalObject* obj = gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset];
						target = make_tuple(obj->getXReal(), obj->getYReal());
					}
				}
			}
		}
    }
    /* We are carrying food and haven't reached the nest, keep building up the
       pheromone trail attached to this found food item. */
	else if(_wm->getWorld()->getIterations() % TrailDensityRate == 0){
            trailToShare->push_back(make_tuple(_wm->getXReal(), _wm->getYReal()));
    }
}


/*****
 * Update the pheromone list and set the target to a pheromone position.
 * return TRUE:  pheromone was successfully targeted
 *        FALSE: pheromones don't exist or are all inactive
 *****/
bool MPFAController::SetTargetPheromone() {
	double maxStrength = 0.0, randomWeight = 0.0;
    bool isPheromoneSet = false;
    if(getClosestNest()->PheromoneList->size()==0) return false; //qilu 07/04 the case of no pheromone.
    /* update the pheromone list and remove inactive pheromones */

    /* default target = nest; in case we have 0 active pheromones */
	target = getClosesNestPos();

    /* Calculate a maximum strength based on active pheromone weights. */
	for(size_t i = 0; i < getClosestNest()->PheromoneList->size(); i++) {
		maxStrength += getClosestNest()->PheromoneList->at(i).strength;
    }

    /* Calculate a random weight. */
    randomWeight = rand()%((int)(maxStrength*1000.0))/1000.0;

    /* Randomly select an active pheromone to follow. */
    for(size_t i = 0; i < getClosestNest()->PheromoneList->size(); i++) {
	    if(randomWeight < getClosestNest()->PheromoneList->at(i).strength) {
            /* We've chosen a pheromone! */
            trailToFollow = getClosestNest()->PheromoneList->at(i).trail;
            target = trailToFollow->back();
            isPheromoneSet = true;
            /* If we pick a pheromone, break out of this loop. */
            break;
	    }
        /* We didn't pick a pheromone! Remove its weight from randomWeight. */
        randomWeight -= getClosestNest()->PheromoneList->at(i).strength;
    }
    return isPheromoneSet;
}


/*****
 * Return the Poisson cumulative probability at a given k and lambda.
 *****/
double MPFAController::GetPoissonCDF(double k, double lambda) {
    double sumAccumulator       = 1.0;
    double factorialAccumulator = 1.0;

    for (size_t i = 1; i <= floor(k); i++) {
        factorialAccumulator *= i;
        sumAccumulator += pow(lambda, i) / factorialAccumulator;
    }

    return (exp(-lambda) * sumAccumulator);
}


bool MPFAController::isInNest(){
	int v = _wm->getGroundSensorValue();
    if(PhysicalObject::isInstanceOf(v)){
        auto nest = dynamic_cast<NestObject*>(gPhysicalObjects[v-gPhysicalObjectIndexStartOffset]);

        if(nest != NULL){
            return true;
        }
    }
    return false;
}


/*****
 * If the robot has just picked up a food item, this function will be called
 * so that the food density in the local region is analyzed and saved. This
 * helps facilitate calculations for pheromone laying.
 *
 * Ideally, given that: [*] is food, and [=] is a robot
 *
 * [*] [*] [*] | The maximum resource density that should be calculated is
 * [*] [=] [*] | equal to 9, counting the food that the robot just picked up
 * [*] [*] [*] | and up to 8 of its neighbors.
 *
 * That being said, the random and non-grid nature of movement will not
 * produce the ideal result most of the time. This is especially true since
 * item detection is based on distance calculations with circles.
 *****/
void MPFAController::SetLocalResourceDensity() {

    tuple<double,double> distance;
	resourceDensity = 1; // remember: the food we picked up is removed from the foodList before this function call
                         // therefore compensate here by counting that food (which we want to count)

	for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        if ( gSensoryInputs_physicalObjectType )
        {
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
                {
                	resourceDensity++;
                }
            }
        }
    }
    fidelity = make_tuple(_wm->getXReal(), _wm->getYReal());
    updateFidelity = true;
    trailToShare->push_back(fidelity);
}


/*****
 * Provides a bound on the value by rolling over a la modulo.
 *****/
double MPFAController::GetBound(double value, double low, double high) {
    /* Calculate an offset. */
    double offset = abs(low) + abs(high);
    /* Increment value by the offset while it's less than low. */
    while (value < low) {
        value += offset;
    }
    /* Decrement value by the offset while it's greater than high. */
    while (value > high) {
        value -= offset;
    }
    /* Return the bounded value. */
    return value;
}


/*****
 * Calculate and return the exponential decay of "value."
 *****/
double MPFAController::GetExponentialDecay(double w, double time, double lambda) {//qilu 01/08/2016
	double twoPi = 6.283;
    return w + (twoPi-w)* exp(-lambda * time);
}


NestObject* MPFAController::getClosestNest(){
	if(gNestObjects.size() == 0){
        std::cout << "[ERROR] Impossible call to MPFAController::getClosestNestPos(.)\n";
        exit(-1);
    }

    double inf = std::numeric_limits<double>::infinity();
    double nest_x = 0, nest_y = 0;

    int x = _wm->_xReal, y = _wm->_yReal;
    NestObject* closest = nullptr;
    for(auto nest : gNestObjects){
        double n_x = nest->getXReal(), n_y = nest->getYReal();
        double delta = getEuclideanDistance(x, y, n_x, n_y);

        if(delta < inf){
            inf = delta;
            nest_x = n_x;
            nest_y = n_y;
            closest = nest;
        }
    }
    return closest;
}

tuple<double,double> MPFAController::getClosesNestPos(){
    auto nest = getClosestNest();
    return make_tuple(nest->getXReal(), nest->getYReal());
}


/*****
 * If the robot is heading towards the target position set by the step()
 * function then no correction is made to the robot's motor speeds.
 *
 * Otherwise, the robot will calculate whether turn left or right such that it
 * is facing its intended target and then move forward.
 *****/
void MPFAController::ApproachTheTarget() {

	double heading = getAngleToTarget(_wm->getXReal(), _wm->getYReal(), getOrientation()*180.0, get<0>(target), get<1>(target)) / 180.0;
	bool wallDetected = false;


	for(int i  = 0; i < 4; i++)
	{
		int objectId = _wm->getObjectIdFromCameraSensor(i);
		if ( gSensoryInputs_physicalObjectType )
		{
			if ((isCarrying()&&PhysicalObject::isInstanceOf(objectId)) || objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset)
			{
				if(i<=2){
					// wall on left. steer right.
					setRotation(RobotRotationSpeed);
				} else{
					// wall on right. steer left.
					setRotation(-RobotRotationSpeed);
				}
				setTranslation(0.5);
				wallDetected = true;
				break;
			}
		}
	}

	if(isCollision()){
		double rot = -1 + rand()%2000/1000.0;
		setRotation(rot);
		setTranslation(0.5);
		wallDetected = true;
	}

	if(wallDetected && _wm->getWorld()->getIterations()%120 == 0){
		SetRandomSearchLocation();
		isInformed = false;
		MPFA = DEPARTING;
	}


	if(!wallDetected){
		if(heading == heading){ // if heading is NaN, this will return false.
			setRotation(heading*RobotRotationSpeed);
			setTranslation(1-0);
		}
	}
}

bool MPFAController::isCollision(){
	return prevPos == make_tuple(_wm->getXReal(), _wm->getYReal());
}

/*****
 * Set the target to a random position along the edge of the arena.
 *****/
void MPFAController::SetRandomSearchLocation() {

	int newX, newY;
	int x_min = 0, y_min = 0;
	int lenx = x_max - x_min, leny = y_max - y_min;

	int pos = rand()%(2*lenx + 2*leny);
	if(pos < lenx){ // top
		newX = pos;
		newY = y_min;
	} else if(pos < (lenx+leny)){ // right
		newY = pos - lenx;
		newX = x_max;
	} else if(pos < (2*lenx + leny)){ // bottom
		newX = pos - lenx - leny;
		newY = y_max;
	} else{ // left
		newX = x_min;
		newY = pos - 2*lenx - leny;
	}

	SetTargetInBounds(make_tuple(newX,newY));
}


/*****
 * The MPFA random and correlated walks (in addition to other sources) may
 * generate new target points outside of the bounds of the arena. We will use
 * this function to adjust any targets such that they always fall within the
 * bounds of the forageRange X and Y values saved in the iAnt_data object.
 *****/
void MPFAController::SetTargetInBounds(tuple<double,double> t) {

	int x = get<0>(t), y = get<1>(t);

    /* Bound the X value based on the forage range. */
    if(x_max - x<=40)
        t = make_tuple(x_max-40, y);
    if(x - 0<=40)
        t = make_tuple(40, y);

    /* Bound the Y value based on the forage range. */
    if(y_max - y<=40)
        t = make_tuple(x, y_max-40);
    if(y - 0<=40)
		t = make_tuple(x, 40);
    /* Set the robot's target to the bounded t position. */
    target = t;
}
