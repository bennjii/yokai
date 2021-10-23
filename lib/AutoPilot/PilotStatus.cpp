#include "PilotStatus.h"

unsigned long blink_period = 1000;
bool state = false;
int status = 0; 

int r_pin;
int g_pin;
int b_pin;
int pow_pin;

void PilotStatus::updateStatus()
{
    runSwitch(!state, status);
    state = !state;
}

void PilotStatus::runSwitch(int led_state, int led_status) 
{
    switch(led_status) {
        case 0:
            break;
        case 1:
            digitalWrite(g_pin, led_state ? HIGH : LOW);

            break;
        case 2:
            digitalWrite(g_pin, LOW);

            blink_period = 1000;
            
            break;
        case 3:
            digitalWrite(g_pin, led_state ? HIGH : LOW);
            digitalWrite(r_pin, led_state ? HIGH : LOW);

            blink_period = 2000;

            break;
        case 4:
            digitalWrite(g_pin, LOW);
            digitalWrite(r_pin, LOW);
            
            break;
        case 5:
            digitalWrite(r_pin, LOW);
            
            break;
        case 6:
            digitalWrite(r_pin, led_state ? HIGH : LOW);

            blink_period = 1000;

            break;
        case 7:
            digitalWrite(b_pin, led_state ? HIGH : LOW);
            digitalWrite(r_pin, led_state ? HIGH : LOW);

            break;
        default:
            Serial.print(led_status);
            Serial.print("  -  ");
            Serial.print(led_state);
            Serial.println("");

            break;
    }
}

bool PilotStatus::setStatusPins(int _r_pin, int _g_pin, int _b_pin, int _pow_pin)
{
    // Allocate Variables
    r_pin = _r_pin;
    g_pin = _g_pin;
    b_pin = _b_pin;
    pow_pin = _pow_pin;

    // Initialize all outputs
    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);
    pinMode(pow_pin, OUTPUT);

    // Write all BLANK. (OFF State)
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
bool PilotStatus::setStatus(int _status)
{
    // Set status for read in update function.
    status = _status;   

    // When state change occurs, visible immediately.
    state = true;

    // Reset All Pins to OFF State.
    digitalWrite(g_pin, HIGH);
    digitalWrite(r_pin, HIGH);
    digitalWrite(b_pin, HIGH);

    // Apply Changes Immediately
    updateStatus();

    return true;
}