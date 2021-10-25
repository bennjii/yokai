#include "Drone.h"

t drone_status = { 0, 250, false };
t acel_update = { 0, 1, false };

float ax;
float ay;
float az;

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

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        status.setStatus(5);
        while(1);
    }

    // Recieve 10ms of information MAXIMUM,
    // Reduced delay timeout when awating packet fufilment
    Serial.setTimeout(10);

    // 22 - R,     23 - G,     24 - B,     25 - POW
    status.setStatusPins(22, 23, 24, 25);

    // Set Status - Operational
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
    if (acel_update.tCheck()) {
        updateAcceleration();
        acel_update.tRun();
    }

    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");


    int degreesX = 0;
    int degreesY = 0;

    if (ax > 0.1) {
        Serial.print("U");

        degreesX = map(100 * ax, 0, 97, 0, 90);
    }else if (ax < -0.1) {
        Serial.print("D");
    
        degreesX = map(100 * ax, 0, -100, 0, 90);
    }

    if (ay > 0.1) {
        Serial.print("L");

        degreesY = map(100 * ay, 0, 97, 0, 90);
    }else if (ay < -0.1) {
        Serial.print("R");

        degreesY = map(100 * ay, 0, -100, 0, 90);
    }

    Serial.print("↑ "); Serial.println(degreesX);
    Serial.print("↻ "); Serial.println(degreesY);
    Serial.println("");

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

    // if(Serial.available())
    // {
    //     int stat = Serial.parseInt();
    //     if(stat >=0 && stat <= 7) { 
    //         status.setStatus(stat);
    //         drone_status.tRun();
    //     }
    // }

    // END - Status Demo

    // Drone Update Function, Run every LOOP.
    // for(auto prop : propellers)
    // {
    //     digitalWrite(prop.pin, prop.rpm);
    // }
}

/*
 * Checks if task should be run (boolean response)
 */
bool t::tCheck() {
  if (millis() > t_start + t_timeout) return true;    
  return false;
}

/*
 * Should be called after task complete. (void)
 */
void t::tRun() {
    t_start = millis();
}

/*
 * Change timeout duration of task
 * @param time new timeout in ms (unsigned long)
 */
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

bool Drone::updateAcceleration() {
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
    }

    return true;
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