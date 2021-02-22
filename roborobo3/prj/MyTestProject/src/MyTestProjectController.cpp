/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "MyTestProject/include/MyTestProjectController.h"
#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"

// Load readable sensor names
#define NB_SENSORS 8 // should be coherent with gRobotSpecsImageFilename value read from the property file.
#include "Utilities/Sensorbelt.h"

MyTestProjectController::MyTestProjectController( RobotWorldModel *__wm ) : Controller ( __wm )
{
    if ( _wm->_cameraSensorsNb != NB_SENSORS )
    {
        std::cerr << "[CRITICAL] This project assumes robot specifications with " << NB_SENSORS << " sensors (provided: " << _wm->_cameraSensorsNb << " sensors). STOP.\n";
        exit(-1);
    }
    
}

MyTestProjectController::~MyTestProjectController()
{
	// nothing to do.
}

void MyTestProjectController::reset()
{
	// nothing to do.
}

// a basic obstacle avoidance behavior
void MyTestProjectController::step()
{
    // QUICK HELP
    //
    // setRotation(double value) and setTranslation(double value) take values in [-1.0,+1.0]
    // rotation: positive value means clock-wise rotation.
    // note that these are *desired* rotation/translation. The robot may not be able to comply if it is stuck.
    //
    // getDistanceAt returns a value in [0,1], with 1 as the maximal value (i.e. see nothing within (limited) eyesight)
    //
    // To access all information available to the robot, check <roborobo3>/include/core/Controllers/Controller.h - section "Accessing methods"
    
    
    setTranslation( sin( ( ( getNoEnergyitemDistance(SENSOR_FFL) + getNoEnergyitemDistance(SENSOR_FFR) ) / 2.0 )* M_PI / 2.0) );
    
    double distanceOnMyLeft = getNoEnergyitemDistance(SENSOR_L) + getNoEnergyitemDistance(SENSOR_FL) + getNoEnergyitemDistance(SENSOR_FFL);
    double distanceOnMyRight = getNoEnergyitemDistance(SENSOR_R) + getNoEnergyitemDistance(SENSOR_FR) + getNoEnergyitemDistance(SENSOR_FFR);

    double rotationDelta = 0.3;
    double noiseAmplitude = 0.01;
    
    if(distanceOnMyLeft < 3)
        setRotation(rotationDelta);
    else if(distanceOnMyRight < 3)
        setRotation(-rotationDelta);
    else
        setRotation(0);
    
//    if ( distanceOnMyLeft < distanceOnMyRight )
//		setRotation( +rotationDelta );
//	else
//		if ( distanceOnMyRight < distanceOnMyLeft )
//			setRotation( -rotationDelta );
//		else
//            //setRotation( 0.1 - (double)(random01()*0.2));
//            setRotation( noiseAmplitude * ( 1.0 - (double)(random01()*2.0) ) );

}


int MyTestProjectController::getNoEnergyitemDistance(int sensorId){
    double distance = getDistanceAt(sensorId);
    if(distance == 1)
        return 1;
    
    std::cout << "Distance: " << distance << "\n";
    if(getObjectAt(sensorId) == 0){
        std::cout << "No object\n";
        return distance;
    }
    
    int objectType = getObjectTypeAt(sensorId);
    std::cout << "Object type: " << objectType << "\n";
    
    if(objectType == 1){
        std::cout << "\n";
        return 1;
    }
    
    return distance;
}
