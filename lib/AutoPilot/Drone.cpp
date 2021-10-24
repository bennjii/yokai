#include "Drone.h"

t drone_status = { 0, 250, false };

/*
 * Drone Initialization
 * @param _ta Transmission Address
 * @param m_pins Motor Pins (4)
 */
Drone::Drone(char _ta[7], int m_pins[4])
{
    transmissionAddress = *_ta;
    Serial.println("[TASK]:\tInitializing Drone");

    // for(int i = 0; i < 4; i++) {
    //     propellers[i].setPin(m_pins[i]);
    // }

    Serial.setTimeout(10);

    status.setStatusPins(22, 23, 24, 25);
    status.setStatus(7);
}

Drone::Drone() {}

/*
 * aprox. 16runs / ms in std.arduino.clockcycle
 * Function Run cyclically. 
 * Should not be called >1 per loop, or outside of loop.
 * 
 * If AutoPilot is included, run secondary to pilot update.
 */
void Drone::update()
{
    // Run any queued tasks in tTaskManager
    // for(size_t i = 0; i < tasks.size(); i++)
    // {
    //     // Verify Function SHOULD be run.
    //     if(tasks[i].tCheck()) {
    //         // Execute Function
    //         // tasks[i].t_back();

    //         // Propogate Run Changes
    //         tasks[i].tRun();

    //         // Remove tasks as neccesary
    //         if(tasks[i].t_volatile) {
    //             tasks.erase(i);
    //         } 
    //     }
    // }

    // START - Status DEMO
    if (drone_status.tCheck()) {
        status.updateStatus();
        drone_status.tRun();
    }

    if(Serial.available())
    {
        int stat = Serial.parseInt();
        if(stat >=0 && stat <= 7) { 
            status.setStatus(stat);
            drone_status.tRun();
        }
    }

    // END - Status Demo

    // Drone Update Function, Run every LOOP.
    // for(auto prop : propellers)
    // {
    //     digitalWrite(prop.pin, prop.rpm);
    // }
}

bool t::tCheck() {
  if (millis() > t_start + t_timeout) return true;    
  return false;
}

void t::tRun() {
    t_start = millis();
}

void t::tAlter(unsigned long time) {
    t_timeout = time;
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