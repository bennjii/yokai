#include "Propeller.h"
#include <Arduino.h>

/*
 * Propeller Class Initialiser
 * @param _location The location of the motor on the schematic
 * @param _pin The PWM pin assigned to the control over that motor
*/
Propeller::Propeller(int _location, int _pin)
{
    location = _location;
    pin = _pin;
    rpm = 5;

    pinMode(pin, OUTPUT);
}

int Propeller::getLocation() const
{
    return location;
}

void Propeller::setPropellerSpeed(float _rpm) 
{
    rpm = _rpm;
    digitalWrite(pin, rpm);
}

void Propeller::setPin(int _pin)
{
    pin = _pin;
}