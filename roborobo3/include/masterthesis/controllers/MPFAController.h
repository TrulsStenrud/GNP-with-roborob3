#pragma once

#include "../../../prj/MyTestEE/include/MyTestEEController.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../core/World/World.h"
#include "../../../prj/MyTestEE/include/NestObject.h"

using namespace std;

class MPFAController : public MyTestEEController{
	int x_max, y_max;
	tuple<double,double> getClosesNestPos();
	NestObject* getClosestNest();
	bool isInNest();
	void FindFood();
	bool isCollision();
	tuple<double,double> prevPos;
	const double PI = 3.141592;
public:
	MPFAController(RobotWorldModel *wm);
	~MPFAController();
	void reset() override;
	void step() override;
	double probSwitchToSearch;
	double ProbabilityOfReturningToNest;
	double UninformedSearchVariation;
	double RateOfInformedSearchDecay;
    double RateOfSiteFidelity;
    double RateOfLayingPheromone;
    double RateOfPheromoneDecay;
    int TrailDensityRate = 5;


	// Copied stuff. (https://github.com/BCLab-UNM/iAnt-ARGoS/tree/lukey_development/)
public:
	/* public helper functions */
//	bool       IsInTheNest();
//	void       SetData(iAnt_data* dataPointer);
//	iAnt_data* GetData();
//	tuple<double,double>   GetPosition();
	tuple<double,double>   GetTarget();
//	size_t     GetSearchingTime();//qilu 10/22
//	size_t     GetTravelingTime();//qilu 10/22
//	string     GetStatus();//qilu 10/22
//	void       AddTravelingTime(size_t remainderTime);//qilu 10/22
//	void       AddSearchingTime(size_t remainderTime);//qilu 10/22
	size_t     startTime;//qilu 10/22

private:
	/* iAnt controller parameters */
	double DistanceTolerance;
	double SearchStepSize;
	double RobotForwardSpeed;
	double RobotRotationSpeed;
	double AngleTolerance; // [0,1]

	/* robot internal variables & statistics */
//	CRandom::CRNG*   RNG;
//	iAnt_data*       data;
	tuple<double,double>         target;
	tuple<double,double>         fidelity;
	bool			 updateFidelity; //qilu 07/29
	vector<tuple<double,double>>* trailToShare;
	vector<tuple<double,double>>* trailToFollow;
	bool             isInformed;
	size_t           searchTime;
//	size_t           waitTime;
//	size_t           collisionDelay;
	size_t           resourceDensity;
//	size_t           searchingTime; //qilu 10/22
//	size_t           travelingTime;

	/* iAnt MPFA state variable */
	enum MPFA { INACTIVE, DEPARTING, SEARCHING, RETURNING, SHUTDOWN } MPFA;

	 /* iAnt MPFA state functions */
	void inactive();
	void departing();
	void searching();
	void returning();
	void shutdown();

	/* MPFA helper functions */
	void SetRandomSearchLocation();
	void SetLocalResourceDensity();
	bool SetTargetPheromone();
	double GetExponentialDecay(double w, double time, double lambda);
	double GetBound(double x, double min, double max);
	double GetPoissonCDF(double k, double lambda);

	/* navigation helper functions */
	double GetHeading(); // CRadians
	void     ApproachTheTarget();
	void     SetTargetInBounds(tuple<double,double> newTarget);
};
