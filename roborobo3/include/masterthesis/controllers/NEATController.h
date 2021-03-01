#pragma once

#include "../../../include/core/Controllers/Controller.h"

class NEATController : public Controller{
	// hente nn fra evolveren. kjøre den. rapportere resultatet til evolveren.
public:
	NEATController(RobotWorldModel *wm);
	~NEATController();
	void reset() override;
	void step() override;
};
