#include "MPFAController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/Utilities/Misc.h"
#include "../../../prj/MyTestEE/include/MyTestEEWorldObserver.h"
#include "../../../include/core/World/PhysicalObject.h"

using namespace std;

MPFAController::MPFAController(RobotWorldModel *wm):
//	compass(NULL),
//    motorActuator(NULL),
//    proximitySensor(NULL),
//	m_pcLEDs(NULL), //qilu 09/04
//    ClosestNest(NULL),//qilu 07/13
    DistanceTolerance(05.0),
    SearchStepSize(010.0),
    RobotForwardSpeed(1.0),
    RobotRotationSpeed(1.0),
//    RNG(NULL),
//    data(NULL),
//    isHoldingFood(false),
    isInformed(false),
    //isUsingSiteFidelity(false),
    searchTime(0),
//    waitTime(0),
//    searchingTime(0),//qilu 10/22
//    travelingTime(0),//qilu 10/22
//    collisionDelay(0),
    resourceDensity(0),
    fidelity(10000,10000), //qilu 07/04
    updateFidelity(false), //qilu 07/29
    AngleTolerance(0.1),
    MPFA(INACTIVE),
    probSwitchToSearch(1), // 0.99999
	ProbabilityOfReturningToNest(0.0), // 0.00001
	UninformedSearchVariation(0.231256126), // 0.231256126
	RateOfInformedSearchDecay(0.05),
    RateOfSiteFidelity(10.0),
    RateOfLayingPheromone(10.0),
    RateOfPheromoneDecay(0.05),
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

void MPFAController::reset() {
//    isHoldingFood   = false;
    isInformed      = false;
    searchTime      = 0;
//    waitTime        = 0;
    resourceDensity = 0;
//    collisionDelay  = 0;
    MPFA            = INACTIVE;

//	data->CollisionTime=0; //qilu 10/30
//	data->currCollisionTime =0; //qilu 10/30
//    data->lastCollisionTime =0; //qilu 10/30
    /* Set LED color */
//    m_pcLEDs->SetAllColors(CColor::BLACK); //qilu 09/04
    target   = getClosesNestPos();
    fidelity = getClosesNestPos();

    trailToShare->clear();
    trailToFollow->clear();
    prevPos = make_tuple(_wm->getXReal(), _wm->getYReal());
    MyTestEEController::reset();
}


void MPFAController::step(){
	// todo: oppdater sensorer. buildInputVector og sleng den videre?

	switch(MPFA) {
        /* state at the start or reset of a simulation, start departing() */
        case INACTIVE:
            inactive();
//            cout<<"inactive"<<endl;
            break;
        /* depart from nest after food drop off (or at simulation start) */
        case DEPARTING:
            departing();
//            cout<<"departing"<<endl;
            break;
        /* after departing(), once conditions are met, begin searching() */
        case SEARCHING:
            searching();
//            cout<<"searching"<<endl;
            break;
        /* return to nest after food pick up or giving up searching() */
        case RETURNING:
            returning();
//            cout<<"returning"<<endl;
            break;
        /* all food is picked up, return to the nest and turn off motors */
        case SHUTDOWN:
//        	cout<<"shutdown"<<endl;
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
	startTime = 0;
	MPFA = DEPARTING;
}

void MPFAController::departing(){
	int x = _wm->getXReal(), y = _wm->getYReal();
	int diffx = get<0>(target)-x, diffy = get<1>(target)-y;


//	CVector2 distance = (GetPosition() - target);
    /* Are we informed? I.E. using site fidelity or pheromones. */
    if(isInformed) {
        /* When informed, proceed to the target location. */
        if(hypot(diffx,diffy) < DistanceTolerance){
//        if(distance.SquareLength() < DistanceTolerance) {
            searchTime = 0;
            MPFA = SEARCHING;
//            travelingTime+=data->SimTime-startTime;//qilu 10/22
//            startTime = data->SimTime;//qilu 10/22
		}
    } else {
        /* When not informed, continue to travel until randomly switching
           to the searching state. */
//        Real randomNumber = RNG->Uniform(CRange<Real>(0.0, 1.0));
        double randomNumber = rand()%10000 / 10000.0;
        if(_wm->getWorld()->getIterations() % 300 == 0 && randomNumber < probSwitchToSearch){
//        if(data->SimTime % (5*data->TicksPerSecond) ==0 && randomNumber < data->ProbabilityOfSwitchingToSearching) { //qilu 01/20/2016
            searchTime = 0;
    	    MPFA = SEARCHING;
    	    target = make_tuple(x,y);
    	    /*qilu 07/04/2015 this sets the current location to be the search location,
    	     then the robot can start to search*/
//            travelingTime+=data->SimTime-startTime;//qilu 10/22
//            startTime = data->SimTime;//qilu 10/22
    	}
    	else if(hypot(diffx,diffy) < DistanceTolerance){
//    	else if(distance.SquareLength() < DistanceTolerance) {
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
//        CVector2 distance = GetPosition() - target;
//        Real     random   = RNG->Uniform(CRange<Real>(0.0, 1.0));
        /* randomly give up searching */
//		if(data->SimTime % (5*data->TicksPerSecond) ==0 && random < data->ProbabilityOfReturningToNest) {
		if(_wm->getWorld()->getIterations()%300 == 0 && random < ProbabilityOfReturningToNest){
//            target =ClosestNest->GetLocation();//qilu 07/17
            target = getClosesNestPos();
//           	data->FidelityList.erase(controllerID); //qilu 07/28
			fidelity= make_tuple(10000,10000); //qilu 07/28
			updateFidelity = false; //qilu 07/29
            //LOG<<"give up and return...."<<endl;
            MPFA = RETURNING;
//            searchingTime+=data->SimTime-startTime;//qilu 10/22
//            startTime = data->SimTime;//qilu 10/22
        }
        /* If we reached our target search location, set a new one. The
           new search location calculation is different based on wether
           we are currently using informed or uninformed search. */
		else if(true){
//		else if(hypot(diffx, diffy) < DistanceTolerance){
//        else if(distance.SquareLength() < DistanceTolerance) {//qilu, every 8 ticks in this branch since it takes 8 ticks to reach a new target
            //LOG<<"data->SimTime="<<data->SimTime<<endl;
//            Real USCV = data->UninformedSearchVariation.GetValue();
//            Real rand = RNG->Gaussian(USCV);
            double rand = randgaussian() * UninformedSearchVariation; //randgaussian er fra misc.
            //CRadians rotation(rand);
            /* uninformed search */
            if(!isInformed) {
				//LOG<<"uninformed search...."<<endl;
//               Real USCV = data->UninformedSearchVariation.GetValue();
//                Real rand = RNG->Gaussian(USCV);
//                CRadians rotation(rand);
//			    CRadians angle1(rotation.UnsignedNormalize());
//				CRadians angle2(GetHeading().UnsignedNormalize());
//			    CRadians turn_angle(angle1 + angle2);
//                CVector2 turn_vector(SearchStepSize, turn_angle);
//                SetTargetInBounds(turn_vector + GetPosition());

//                double turn_angle = GetHeading()+rand;
                double turn_angle = getOrientation()+rand;
                double movex = cos(turn_angle*PI) * SearchStepSize;
                double movey = sin(turn_angle*PI) * SearchStepSize;

                SetTargetInBounds(make_tuple(_wm->getXReal()+movex, _wm->getYReal()+movey));
			}
            /* informed search */
            else{
				//size_t   t           = searchTime++;
                //Real   t           = searchTime++;
                //LOG<<"informed search"<<data->SimTime<<endl;
                double t           = searchTime++;// qilu 01/20/2016 time in 5 seconds for search decay
			    //Real     twoPi       = (CRadians::TWO_PI).GetValue();
//                Real     pi          = (CRadians::PI).GetValue();
                double pi = 3.1415;
                double isd = RateOfInformedSearchDecay;
				//Real     correlation = GetExponentialDecay(twoPi, t, isd);
				double correlation = GetExponentialDecay(rand, t, isd);
				double rotation = GetBound(correlation, -pi, pi);
//				CRadians angle1(rotation.UnsignedNormalize());
//				CRadians angle2(GetHeading().UnsignedNormalize());
//				CRadians turn_angle(angle1 + angle2);
//                CVector2 turn_vector(SearchStepSize, turn_angle);
//				SetTargetInBounds(turn_vector + GetPosition());
//                double turn_angle = GetHeading() + rotation;
                double turn_angle = getOrientation()+rotation;
				double movex = cos(turn_angle*PI) * SearchStepSize;
                double movey = sin(turn_angle*PI) * SearchStepSize;
                SetTargetInBounds(make_tuple(_wm->getXReal()+movex, _wm->getYReal()+movey));
            }
        }
    }
    /* Food has been found, change state to RETURNING and go to the nest */
    else {
//        SetClosestNest();
//        target =ClosestNest->GetLocation();//qilu 07/05
        target = getClosesNestPos();
        SetLocalResourceDensity();    //qilu 07/17
        MPFA = RETURNING;
//        searchingTime+=data->SimTime-startTime;//qilu 10/22
//        startTime = data->SimTime;//qilu 10/22
    }
    /* Adjust motor speeds and direction based on the target position. */

    FindFood();

    ApproachTheTarget();

}

void MPFAController::returning(){
//    CVector2 distance = GetPosition() - target;
	FindFood();
    /* Are we there yet? (To the nest, that is.) */
    if(isInNest()){
//   	if(distance.SquareLength() < data->NestRadiusSquared) {
        /* Based on a Poisson CDF, the robot may or may not create a pheromone
           located at the last place it picked up food. */
        double poissonCDF_pLayRate    = GetPoissonCDF(resourceDensity, RateOfLayingPheromone);
        double poissonCDF_sFollowRate = GetPoissonCDF(resourceDensity, RateOfSiteFidelity);
        double r1 = rand()%1000 / 1000.0;  //RNG->Uniform(CRange<Real>(0.0, 1.0));
        double r2 = rand()%1000 / 1000.0;  //RNG->Uniform(CRange<double>(0.0, 1.0));
        //create a pheromone trail if there is a new site fidelity

		if(poissonCDF_pLayRate > r1 && updateFidelity){//qilu 07/29
//			double timeInSeconds = (double)(data->SimTime / (5*data->TicksPerSecond));//qilu 01/20/2016 change to each 5 seconds
//            iAnt_pheromone sharedPheromone(fidelity,
//                                           trailToShare,
//                                           timeInSeconds,
//                                           data->RateOfPheromoneDecay);

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
			//LOG<<"Use site fidelity..."<<endl;
			target = fidelity;
			isInformed = true;
//			cout<<"following fidelity"<<endl;
			//isUsingSiteFidelity = true;
		}
        /* use pheromone waypoints */
        else if(SetTargetPheromone()) {
            //LOG<<"Use pheromone waypoints..."<<endl;
			isInformed = true;
//			cout<<"following imaginary nest pheromone"<<endl;
			//isUsingSiteFidelity = false; //qilu 07/28
		}
        /* use random search */
        else {
            //LOG<<"use random search..."<<endl;
			SetRandomSearchLocation();
			isInformed = false;
			//isUsingSiteFidelity = false; //qilu 07/28
		}
		MPFA = DEPARTING;
//        travelingTime+=data->SimTime-startTime;//qilu 10/22
//        startTime = data->SimTime;//qilu 10/22

	}
    /* Adjust motor speeds and direction based on the target position. */
    ApproachTheTarget();
    //LOG<<"return: ApproachTheTarget...."<<endl;
}

/*****
 * SHUTDOWN: If all food items have been collected, the simulation is over
 * and all iAnts are instructed to return to the nest and shut down.
 *****/
void MPFAController::shutdown(){
//	target = getClosesNestPos();
//    searchTime++;
//    if((GetPosition() - target).SquareLength() < (data->ArenaX+data->ArenaY)/4.0 ||
//       searchTime % (data->TicksPerSecond * 30) == 0) {
//        motorActuator->SetLinearVelocity(0.0, 0.0);
//        searchTime--;
//    } else if(data->FoodList.size() > 0) {
//        MPFA = INACTIVE;
//    } else {
//        ApproachTheTarget();
//    }
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
	//LOG<<"set holding food..."<<endl;
    /* Is the iAnt already holding food? */
    if(!isCarrying()) {
	    /* No, the iAnt isn't holding food. Check if we have found food at our
           current position and update the food list if we have. */


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


//        vector<CVector2> newFoodList;
//        vector<CColor>   newFoodColoringList;
//        size_t i = 0, j = 0;
//		if(MPFA!= RETURNING){ //qilu 07/04/2015  remove the case of finding food when the robot returns to the nest
//			for(i = 0; i < data->FoodList.size(); i++) {
//				if((GetPosition() - data->FoodList[i]).SquareLength() < data->FoodRadiusSquared) {
//					/* We found food! Calculate the nearby food density. */
//					isHoldingFood = true;
//					//LOG<<"Calculate the nearby food density"<<endl;
//					j = i + 1;
//					break;
//				} else {
//					/* Return this unfound-food position to the list */
//					newFoodList.push_back(data->FoodList[i]);
//					newFoodColoringList.push_back(CColor::BLACK);
//				}
//			} //end of for
//		}//end of if MPFA
//
//		if(j>0){ //qilu 07/19 avoid redundant iteration
//			for( ; j < data->FoodList.size(); j++) {
//				newFoodList.push_back(data->FoodList[j]);
//				newFoodColoringList.push_back(CColor::BLACK);
//			}
//		}
//        /* We picked up food. Update the food list minus what we picked up. */
//        if(IsHoldingFood()) {
//			//LOG<<"We picked up food. Update the food list minus what we picked up..."<<endl;
//            data->FoodList = newFoodList;
//            data->currNumCollectedFood++; //qilu 08/19
//            //SetLocalResourceDensity(); //qilu 07/19 move this to the branch of discovered food in searching()
//        }
        /* We dropped off food. Clear the built-up pheromone trail. */
        //else trailToShare.clear(); //qilu 07/17
    }
    /* Drop off food: We are holding food and have reached the nest. */
//    else if((GetPosition() - ClosestNest->GetLocation()).SquareLength() < data->NestRadiusSquared) {
//        isHoldingFood = false;
//    }
    /* We are carrying food and haven't reached the nest, keep building up the
       pheromone trail attached to this found food item. */
	else if(_wm->getWorld()->getIterations() % TrailDensityRate == 0){
//    else if(data->SimTime % data->TrailDensityRate == 0) {
            trailToShare->push_back(make_tuple(_wm->getXReal(), _wm->getYReal()));
    }

//    /* If all food is collected, return to the nest and shutdown. */
//    if(data->FoodList.size() == 0 && !IsHoldingFood()) {
//        searchTime = 0;
//        MPFA = SHUTDOWN;
//    }
}


/*****
 * Update the pheromone list and set the target to a pheromone position.
 * return TRUE:  pheromone was successfully targeted
 *        FALSE: pheromones don't exist or are all inactive
 *****/
bool MPFAController::SetTargetPheromone() {
	double maxStrength = 0.0, randomWeight = 0.0;
    bool isPheromoneSet = false;
    //LOG<<"ClosestNest->PheromoneList.size()="<<ClosestNest->PheromoneList.size()<<endl;
    if(getClosestNest()->PheromoneList->size()==0) return false; //qilu 07/04 the case of no pheromone.
    /* update the pheromone list and remove inactive pheromones */

    /* default target = nest; in case we have 0 active pheromones */
	target = getClosesNestPos();

    /* Calculate a maximum strength based on active pheromone weights. */
	for(size_t i = 0; i < getClosestNest()->PheromoneList->size(); i++) {
//        if(getClosestNest()->PheromoneList->at(i)->isRegistered()) { //qilu 07/05
            maxStrength += getClosestNest()->PheromoneList->at(i).strength; //qilu 07/05
//        }
    }

    /* Calculate a random weight. */
//    randomWeight = RNG->Uniform(CRange<double>(0.0, maxStrength));
    randomWeight = rand()%((int)(maxStrength*1000.0))/1000.0;

    /* Randomly select an active pheromone to follow. */
    for(size_t i = 0; i < getClosestNest()->PheromoneList->size(); i++) {
	    if(randomWeight < getClosestNest()->PheromoneList->at(i).strength) {
            /* We've chosen a pheromone! */
//            target = getClosestNest()->PheromoneList->at(i)->getPosition();//qilu 07/07
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


//    /* Calculate resource density based on the global food list positions. */
//	for(size_t i = 0; i < data->FoodList.size(); i++) {
//        distance = GetPosition() - data->FoodList[i];
//
//		if(distance.SquareLength() < data->SearchRadius) {
//			resourceDensity++;
//            data->FoodColoringList[i] = CColor::BLUE;
//            data->ResourceDensityDelay = data->SimTime + data->TicksPerSecond * 10;
//		} else {
//            data->FoodColoringList[i] = CColor::BLACK;
//        }
//	}

    /* Set the fidelity position to the robot's current position. */
    //LOG<<"create a fidelity="<<fidelity<<endl;
//    fidelity = GetPosition();
//    updateFidelity = true; //qilu 07/29
    //LOG<<"updateFidelity = true"<<endl;
//    trailToShare->push_back(fidelity);//qilu 07/19
    /* Add the robot's new fidelity position to the global fidelity list. */
    //fidelity = newFidelity; //qilu 07/16
//    data->FidelityList[controllerID] =fidelity; //qilu 07/16
    /* Delay for 4 seconds (simulate iAnts scannning rotation). */
//    waitTime = (data->SimTime) + (data->TicksPerSecond * 4);
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


tuple<double,double> MPFAController::getClosesNestPos(){
	if(gNestObjects.size() == 0){
        std::cout << "[ERROR] Impossible call to MPFAController::getClosestNestPos(.)\n";
        exit(-1);
    }

    double inf = std::numeric_limits<double>::infinity();
    double nest_x = 0, nest_y = 0;

    int x = _wm->_xReal, y = _wm->_yReal;
    for(auto nest : gNestObjects){
        double n_x = nest->getXReal(), n_y = nest->getYReal();
        double delta = getEuclideanDistance(x, y, n_x, n_y);

        if(delta < inf){
            inf = delta;
            nest_x = n_x;
            nest_y = n_y;
        }
    }
    return make_tuple(nest_x, nest_y);
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


/*****
 * If the robot is heading towards the target position set by the step()
 * function then no correction is made to the robot's motor speeds.
 *
 * Otherwise, the robot will calculate whether turn left or right such that it
 * is facing its intended target and then move forward.
 *****/
void MPFAController::ApproachTheTarget() {
    /* angle of the robot's direction relative to the arena's origin */
//    double angle1  = GetHeading();

	/* angle from the target to the robot's position */
//    CRadians angle2  = (target - GetPosition()).Angle();
	/* heading = angle1 - angle2 = 0.0 when the robot is facing its target */
//	CRadians heading = (angle1 - angle2).SignedNormalize();



	double heading = getAngleToTarget(_wm->getXReal(), _wm->getYReal(), getOrientation()*180.0, get<0>(target), get<1>(target)) / 180.0;
	// det funker slik det er nå. testet i geogebra.


//	if(get<0>(target) - _wm->getXReal() >= 5){
//		cout<<"Robot: ["<<_wm->getXReal()<<", "<<_wm->getYReal()<<"]\tTarget: ["<<get<0>(target)<<", "<<get<1>(target)<<"]\tHeading: "<<heading<<"\tOrientation: "<<getOrientation()<<endl;
//		exit(8);
//	}

	// oppsummering: hvis holder på å krasje i vegg, snu deg vekk fra den. fra første else if, så roterer den for å prøve å sikte på målet, innen en viss toleranse.
	// sensor 0 er venstre, 1 er mellom, 2 er rett frem, 3 er mellom, 4 er høyre.
	bool wallDetected = false;
//	for(int i=0; i<=4; i++){
//		int objectId = _wm->getObjectIdFromCameraSensor(i);
//		if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ){ // not empty, but cannot be identified: this is a wall.
//			if(i<=2){
//				// wall on left. steer right.
//				setRotation(RobotRotationSpeed);
//			} else{
//				// wall on right. steer left.
//				setRotation(-RobotRotationSpeed);
//			}
//			setTranslation(0.5);
//			wallDetected = true;
//			break;
//		}
//	}

	for(int i  = 0; i < 4; i++)
	{
		int objectId = _wm->getObjectIdFromCameraSensor(i);
		if ( gSensoryInputs_physicalObjectType )
		{
			if ((isCarrying()&&(PhysicalObject::isInstanceOf(objectId) || false)) || objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset)
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
//		if(heading <= -AngleTolerance){
//			setRotation(RobotRotationSpeed); // steer right.
//			setTranslation(0.5);
//		}
//		else if(heading >= AngleTolerance){
//			setRotation(-RobotRotationSpeed); // steer left.
//			setTranslation(0.5);
//		}
//		else{
//			setRotation(0);
//			setTranslation(RobotForwardSpeed);
//		}
		if(heading == heading){ // if heading is NaN, this will return false.
			setRotation(heading*RobotRotationSpeed);
			setTranslation(1-0);
		}
	}
//	if(IsCollisionDetected()!=0) {
//		//LOG<<"Collision detected..."<<endl;
//		//if (data->SimTime - collisionOccurTime >=data->TicksPerSecond * 2 && !isCollisionOnWall()){ //qilu 09/03
//        if (!isCollisionOnWall()) {
//            data->currCollisionTime++; //qilu 10/30
//            data->CollisionTime++;//qilu 10/30
//        }
//        if(IsCollisionDetected()==1){
//			//LOG<<"turn right..."<<endl;
//			motorActuator->SetLinearVelocity(RobotRotationSpeed, 0.5*RobotRotationSpeed);//qilu 09/05 //qilu 01/08/2016
//		}
//        else{
//            motorActuator->SetLinearVelocity(0.5*RobotRotationSpeed, RobotRotationSpeed);//qilu 09/05 //qilu 01/08/2016
//            //LOG<<"turn left..."<<endl;
//		}
//        Real randomNumber = RNG->Uniform(CRange<Real>(0.0, 1.0));
//		collisionDelay = data->SimTime + (size_t)(randomNumber*data->TicksPerSecond);//qilu 01/08/2016
//
//	} else if((heading <= AngleToleranceInRadians.GetMin()) &&
//              (collisionDelay < data->SimTime)) {
//		/* turn left */
//		motorActuator->SetLinearVelocity(-RobotRotationSpeed, RobotRotationSpeed);
//	} else if((heading >= AngleToleranceInRadians.GetMax()) &&
//              (collisionDelay < data->SimTime)) {
//		/* turn right */
//		motorActuator->SetLinearVelocity(RobotRotationSpeed, -RobotRotationSpeed);
//	} else {
//		/* go straight */
//		motorActuator->SetLinearVelocity(RobotForwardSpeed, RobotForwardSpeed);
//	}
}

bool MPFAController::isCollision(){
	return prevPos == make_tuple(_wm->getXReal(), _wm->getYReal());
}

/*****
 * Return the angle the robot is facing relative to the arena's origin. [-1, 1]
 *****/
double MPFAController::GetHeading(){
	return getAngleToTarget(x_max/2.0, y_max/2.0, getOrientation()*180.0, _wm->getXReal(), _wm->getYReal()) / 180.0;
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

//	tuple<double,double> p = make_tuple(_wm->getXReal(), _wm->getYReal());
//	int x = _wm->getXReal(), y = _wm->getYReal();
//
//	int x_min = 0, y_min = 0;
//	int newX = x_min + rand() % x_max;
//	int newY = y_min + rand() % y_max;
//
//    bool set_y_max = false;
//
//
//    /* if I'm @ x_max side of arena, newX = opposite side */
//    if((x - x_max) * (x - x_max) < DistanceTolerance) {
//        newX = x_min;
//    }
//    /* if I'm @ x_min side of arena, newX = opposite side */
//    else if((x - x_min) * (x - x_min) < DistanceTolerance) {
//        newX = x_max;
//    }
//    /* middle of arena, randomly pick newX at plus or minus x-axis edge */
//    else if(rand()%2==0) {
//        newX = x_min;
//    }
//    else if(rand()%2==0) {
//        newX = x_max;
//    }
//    /* if newX != the edge of the x-axis: force movement to y-axis edge */
//    else {
//        set_y_max = true;
//    }
//
//    /* if I'm @ y_max side of arena, newY = opposite side */
//    if((y - y_max) * (y - y_max) < DistanceTolerance) {
//        newX = y_min;
//    }
//    /* if I'm @ y_max side of arena, newY = opposite side */
//    else if((y - y_min) * (y - y_min) < DistanceTolerance) {
//        newX = y_max;
//    }
//    else if(rand()%2==0) {
//        newY = y_min;
//    }
//    /* middle of arena, randomly pick newY at plus or minus y-axis edge */
//    else if(rand()%2==0) {
//        newY = y_max;
//    }
//    /* if set_y_max = true: guarantee newY is at plus or minus y-axis edge */
//    else if(set_y_max == true) {
//    	if(rand()%2==0)
//			newY = y_min;
//        else newY = y_max;
//    }

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
    //if(t.GetX() > data->ForageRangeX.GetMax()) //qilu 09/06
    if(x_max - x<=40)
//        t = CVector2(data->ForageRangeX.GetMax()-0.20, t.GetY()); //qilu 09/06, add 0.05 to make sure no target position on the boundary
        t = make_tuple(x_max-40, y);

    //if(t.GetX() < data->ForageRangeX.GetMin())
    if(x - 0<=40)
//        t = CVector2(data->ForageRangeX.GetMin()+0.20, t.GetY());
        t = make_tuple(40, y);

    /* Bound the Y value based on the forage range. */
    //if(t.GetY() > data->ForageRangeY.GetMax())
    if(y_max - y<=40)
//        t = CVector2(t.GetX(), data->ForageRangeY.GetMax()-0.20);
        t = make_tuple(x, y_max-40);

    //if(t.GetY() < data->ForageRangeY.GetMin())
    if(y - 0<=40)
//        t = CVector2(t.GetX(), data->ForageRangeY.GetMin()+0.20);
		t = make_tuple(x, 40);
    /* Set the robot's target to the bounded t position. */
    target = t;
}
