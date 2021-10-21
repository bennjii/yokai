#include "PilotStatus.h"

PilotStatus::PilotStatus()
{
   Serial.println("[STAT]: Status Initialized");
}

bool PilotStatus::setStatusPins(int _r_pin, int _g_pin, int _b_pin, int _pow_pin)
{
    r_pin = _r_pin;
    g_pin = _g_pin;
    b_pin = _b_pin;
    pow_pin = _pow_pin;

    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);
    pinMode(pow_pin, OUTPUT);

    digitalWrite(r_pin, HIGH);
    digitalWrite(g_pin, HIGH);
    digitalWrite(b_pin, HIGH);

    return true;
}

int PilotStatus::getStatus()
{
    return status;
}

/*
 * Status Values:
 *
 * Null:            0 (No Colour)
 * Ready:           1 (Green Blink 1s)
 * Running/Awating: 2 (Orange Repeated Blink 1s Interval)
 * Busy:            3 (Orange Repeated Blink 2s Interval)
 * Warning:         4 (Orange Hold)
 * Error:           5 (Red Hold)
 * Immediate:       6 (Red Repeated Blink 1s Interval)
 * Operational      7 (No Colour)
 */
bool PilotStatus::setStatus(int _status, String message)
{
    status = _status;

    Serial.println("[STAT]: " + message);

    switch(status) {
        case 0:
            break;
        case 1:
            digitalWrite(g_pin, LOW);
            delay(1000);
            digitalWrite(g_pin, HIGH);

            break;
        case 2:
            digitalWrite(g_pin, LOW);
            digitalWrite(r_pin, LOW);
            delay(1000);
            digitalWrite(g_pin, HIGH);
            digitalWrite(r_pin, HIGH);
            
            break;
        case 3:
            digitalWrite(g_pin, LOW);
            digitalWrite(r_pin, LOW);
            delay(1000);
            digitalWrite(g_pin, HIGH);
            digitalWrite(r_pin, HIGH);
            
            break;
        case 4:
            digitalWrite(g_pin, LOW);
            digitalWrite(r_pin, LOW);
            
            break;
        case 5:
            digitalWrite(r_pin, LOW);
            
            break;
        case 6:
            digitalWrite(r_pin, LOW);
            delay(1000);
            digitalWrite(r_pin, HIGH);
            
            break;
        case 7:
            break;
        default:
            break;
    }

    return true;
}