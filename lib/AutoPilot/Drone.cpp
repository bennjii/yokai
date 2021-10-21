#include "Drone.h"

/*
 * Drone Initialization
 * @param _ta Transmission Address
 * @param m_pins Motor Pins (4)
*/
Drone::Drone(char _ta[7], int m_pins[4])
{
    transmissionAddress = *_ta;
    Serial.println("[TASK]:\tInitializing Drone");

    int i = 0;
    for(auto prop : propellers)
    {
        prop.setPin(m_pins[i]);
        i++;
    }

    status = new PilotStatus();
    status->setStatusPins(22, 23, 24, 25);
    status->setStatus(1, "YOKAI Initialized");
}

/*
 * This function converts the updated, digital version of the drones propeller values towards the actual, physical values on the drone.
 * 
 * Often called after a piloting update or PID change, to update the values for the copter's hardware.
*/
void Drone::update()
{
    for(auto prop : propellers)
    {
        digitalWrite(prop.pin, prop.rpm);
    }
}

void Drone::hover()
{
    hovering = true;
}

void Drone::land()
{
    // do all calculations for landing...
    
}

Location Drone::getLocation() const
{
    // Relative location is 0,0.
    return Location(0.00f, 0.00f);
}

void Drone::setPropellerSpeed(const int _prop, const int _rpm) const {
    Propeller prop = propellers[_prop];

    prop.setPropellerSpeed(_rpm);
}

void Drone::setAllPropellerSpeeds(const int _rpm) const {
    for(auto prop : propellers)
    {
        prop.setPropellerSpeed(_rpm);
    }
}

float Drone::getVelocity() const {
    const float velocity = 5; // RETRIEVE VELOCITY FROM HARDWARE
    return velocity;                                                                                                                                                                                                                                                                                                                                                                                       
}

float Drone::getAltitude() const {
    const float altitude = 5; // RETRIEVE ALTITUDE FROM HARDWARE
    return altitude;                                                                                                                                                                                                                                                                                                                                                                                       
}

float Drone::getBearing() const {
    const float bearing = 5; // RETRIEVE BEARING FROM HARDWARE
    return bearing;                                                                                                                                                                                                                                                                                                                                                                                       
}