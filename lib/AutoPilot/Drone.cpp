#include "Drone.h"

t drone_status = {0, 250};

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
 * This function converts the updated, digital version of the drones propeller values towards the actual, physical values on the drone.
 * 
 * Often called after a piloting update or PID change, to update the values for the device's hardware.
 */
void Drone::update()
{
    Serial.println(millis());

    if (tCheck(&drone_status)) {
        status.updateStatus();
        tRun(&drone_status);
    }

    if(Serial.available())
    {
        int stat = Serial.parseInt();
        if(stat >=0 && stat <= 7) { 
            status.setStatus(stat);
            tRun(&drone_status);
        }
    }

    // Drone Update Function, Run every LOOP.
    // for(auto prop : propellers)
    // {
    //     digitalWrite(prop.pin, prop.rpm);
    // }
}

bool Drone::tCheck(struct t *t) {
  if (millis() > t->t_start + t->t_timeout) return true;    
  return false;
}

void Drone::tRun(struct t *t) {
    t->t_start = millis();
}

void Drone::tAlter(struct t *t, unsigned long time) {
    t->t_timeout = time;
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