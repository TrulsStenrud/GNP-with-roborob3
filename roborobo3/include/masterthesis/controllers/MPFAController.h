#pragma once

#include "../../../prj/MyTestEE/include/MyTestEEController.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../core/World/World.h"
#include "../../../prj/MyTestEE/include/NestObject.h"
#include "../evolvers/MPFAEvolver.h"

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
    int TrailDensityRate = 5;

	void setParameters(MPFAGenome params);

	// Copied stuff. (https://github.com/BCLab-UNM/iAnt-ARGoS/tree/lukey_development/)
private:
	/* iAnt controller parameters */
	double DistanceTolerance;
	double SearchStepSize;
	double RobotForwardSpeed;
	double RobotRotationSpeed;

	/* robot internal variables & statistics */
	tuple<double,double>         target;
	tuple<double,double>         fidelity;
	bool			 updateFidelity; //qilu 07/29
	vector<tuple<double,double>>* trailToShare;
	vector<tuple<double,double>>* trailToFollow;
	bool             isInformed;
	size_t           searchTime;
	size_t           resourceDensity;

	/* MPFA state variable */
	enum MPFA { INACTIVE, DEPARTING, SEARCHING, RETURNING, SHUTDOWN } MPFA;

	 /* MPFA state functions */
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
	void     ApproachTheTarget();
	void     SetTargetInBounds(tuple<double,double> newTarget);
};
