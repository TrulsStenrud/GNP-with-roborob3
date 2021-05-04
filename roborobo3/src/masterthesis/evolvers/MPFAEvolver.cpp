#include "MPFAEvolver.h"
#include "MPFAController.h"
#include "../../../include/core/Utilities/Misc.h"
#include "../MscMain.h"

using namespace std;

MPFAEvolver::MPFAEvolver(){
	_logger = new Logger("MPFA" + std::to_string(gInitialNumberOfRobots));

    gProperties.checkAndGetPropertyValue("gEvolutionPopulationSize", &_popSize, true);

    _pop = new vector<MPFAGenome>();
    _evalIndex = 0;
    initPopulation();
    _logger->log("Generation " + std::to_string(_generation));
}

MPFAEvolver::~MPFAEvolver(){
	delete _logger;
}

Controller* MPFAEvolver::make_Controller(RobotWorldModel* wm){
	MPFAController* cont = new MPFAController(wm);
	cont->setParameters(_pop->at(_evalIndex));
	return cont;
}


void MPFAEvolver::evalDone(DataPacket* dp){
	_pop->at(_evalIndex).fitness = dp->fitness;
	_logger->log(dp->foragingPercentage);
	_evalIndex++;
	if(_evalIndex >= _popSize){
		newGeneration();
		_evalIndex = 0;
		double best = -1;
		for(auto g : *_pop){
			best = max(best, g.fitness);
		}
		cout<<"New generation. Best fitness: "<<best<<endl;
		_logger->newLine();
		_logger->log("Generation " + std::to_string(_generation));
	}
	for(Robot* rob : *(dp->robots)){
		MPFAController* cont = static_cast<MPFAController*>(rob->getController());
		cont->setParameters(_pop->at(_evalIndex));
		cont->reset();
	}
}

void MPFAEvolver::newGeneration(){
	vector<MPFAGenome>* newPop = new vector<MPFAGenome>();

	//Elitism
	MPFAGenome elite;
	double eliteFitness = -1;
	for(auto g : *_pop){
		if(g.fitness > eliteFitness){
			elite = g;
			eliteFitness = g.fitness;
		}
	}
	newPop->push_back(elite);

	while(newPop->size()<_popSize){
		MPFAGenome p[2]; // parents
		// Tournament selection
		for(int i=0; i<2; i++){
			MPFAGenome candidates[_tournamentSize];
			for(int j=0; j<_tournamentSize; j++){
				candidates[j] = _pop->at(rand()%_pop->size());
			}
			double best = -1;
			for(auto g : candidates){
				if(g.fitness > best){
					p[i] = g;
					best = g.fitness;
				}
			}
		}

		MPFAGenome child = p[0];
		if(rand()%1000/1000.0 < _crossoverChance)
			child = crossover(p[0], p[1]);

		if(rand()%1000/1000.0 < _mutationChance)
			child = mutate(child);

		newPop->push_back(child);
	}
	delete _pop;
	_pop = newPop;
	_generation++;
}

// single point crossover
MPFAGenome MPFAEvolver::crossover(MPFAGenome p1, MPFAGenome p2){
	int point = rand()%p1.limits.size();
	MPFAGenome c;
	for(int i=0; i<p1.limits.size(); i++){
		if(i < point){
			c.values.push_back(p1.values[i]);
		} else{
			c.values.push_back(p2.values[i]);
		}
	}
	return c;
}

MPFAGenome MPFAEvolver::mutate(MPFAGenome g){
	int gene = rand()%g.limits.size();
	double range = get<1>(g.limits[gene]) - get<0>(g.limits[gene]);
	double change = randgaussian() * _mutationModifier * range;
	g.values[gene] += change;

	// Ensuring the value stays within bounds.
	if(g.values[gene] < get<0>(g.limits[gene]))
		g.values[gene] = get<0>(g.limits[gene]);
	else if (g.values[gene] > get<1>(g.limits[gene]))
		g.values[gene] = get<1>(g.limits[gene]);

	return g;
}

void MPFAEvolver::initPopulation(){
	for(int i=0; i<_popSize; i++){
		MPFAGenome g;
		for(size_t j=0; j<g.limits.size(); j++){
			g.values.push_back(getRandBetween(get<0>(g.limits[j]), get<1>(g.limits[j])));
		}
		_pop->push_back(g);
	}
}


double MPFAEvolver::getRandBetween(double low, double high){
	double diff = high - low;
	double toReturn = rand()%((int)round(diff*1000));
	return low+toReturn/1000.0;
}


bool MPFAEvolver::usesBehavior(){
	return false;
}
