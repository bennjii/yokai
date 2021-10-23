#include "AutoPilot.h"

bool AutoPilot::setTarget(float bearing, float velocity, float altitude) const
{
    return target->setTarget(bearing, velocity, altitude);
}

Location AutoPilot::getTargetLocation() const
{
    return *targetLocation;
}

void AutoPilot::testFlightControls() const 
{
    Serial.println("Running Pre-flight checks");
    Serial.println("[WARNING] Please disconnect all propeller blades for safety.");

    Serial.println("Press any key once ALL propellers are disconnected...");
    
    // if(Serial.available()) {
    //     const int input = Serial.read();

    //     Serial.println("Running Checks...");

    //     //...
    // }
}

void AutoPilot::setCourse(const Location &dest)
{
    targetLocation = new Location(dest);
    keepOnCourse();
}

void AutoPilot::hover() const
{
    for(Propeller &prop : drone->propellers) {
        // [EDIT] Setting Hover Revolutions Per Minute:
        prop.rpm = 500;
    }

    drone->update();
}

float AutoPilot::getHeading(const Location &from, const Location &to) const
{
    float deltaLatitude = from.getLatitude() - to.getLatitude();
    float deltaLongitude = from.getLongitude() - to.getLongitude();

    // A is *from*, B is *to*.

    float x = cos(to.getLatitude()) * sin(deltaLatitude);
    float y = cos(from.getLatitude()) * sin(to.getLatitude()) - sin(from.getLatitude()) * cos(to.getLatitude()) * cos(deltaLongitude);

    float bearing = atan2(x, y);  // Radians
    return degrees(bearing);
}

/**
 * Abort, decends to 1m, hovers. Waits 2s. Lands.
 *
 * @return void
 */
void AutoPilot::abort()
{
    const float new_altitude = 1;
    setAltitude(new_altitude);

    hover();
    delay(2000);
    land();
}

void AutoPilot::beginFlight()
{
    setAltitude(1);
    hover();

    //... Calculate flight path -> bearing, direction and start moving.
}

/**
 * Drone is initialised, is run in setup. Ensures safe boot and setup, so that motors are not running, minimal power usage and no risk of injury.
 * After command to start is retrieved, drone will begin flight. 
 * @return void 
*/
void AutoPilot::awaitCommand() const
{
    // Connect IMU
    Serial.println("[CONNECTION]: Inertial Measurement Unit");
    
    // Connect Radio
    Serial.println("[CONNECTION]: Radio");

    // Set all Motors to 0% Speed
    drone->setAllPropellerSpeeds(0);

    // Check everything is working
    // Throw ANY errors and refuse priming if there are any.

    // Initialise
    Serial.println("[TASK]: Initializing");
    

    // Instantiate
    // Prime

    // Finished, Await Flight Start
    Serial.println("Initialization Sucessful, awating your command.");
}

bool AutoPilot::keepOnCourse() const
{
    float bearing = getHeading(drone->getLocation(), *targetLocation);
    return setBearing(bearing);
}

void AutoPilot::setAltitude(float _altitude)
{
    if(_altitude >= 0 && _altitude < 10000) targetAltitude = _altitude;
    else Serial.println("Invalid Altitude ( >10,000 or < 0)");
}

/**
 * Calculate motor speed for each motor of an X quadcopter depending on received instructions and measures from sensor
 * by applying PID control.
 *
 * (A) (B)     x  
 *   \ /     z ↑
 *    X       \|
 *   / \       +----→ y
 * (C) (D)
 *
 * Motors A & D run clockwise.
 * Motors B & C run counter-clockwise.
 *
 * @return void
 */
void AutoPilot::update()
{
    drone->update();

    // for(Propeller prop : drone->propellers) {
    //     // Set RPM to whatever will reach destination...
    // }
}

float AutoPilot::getETA() const
{   
    const int distance = targetLocation->distanceTo(drone->getLocation());
    return (distance) / drone->getVelocity();
}

void AutoPilot::turn(const float degree) const 
{
    setBearing(drone->getBearing() + degree);
}

bool AutoPilot::setBearing(const float bearingTarget) const
{
    // const int bearingInitial = drone->getBearing();
    // const int deltaBearing = bearingTarget - bearingInitial;

    return true;

    // Go from here to adjust positional data... 
    // Once you have bearing, change the drone to face the direction...
}