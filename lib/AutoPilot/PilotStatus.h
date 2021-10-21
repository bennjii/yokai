#include <Arduino.h>

class PilotStatus
{ 
    private: 
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
        int status = 0;

        int r_pin;
        int g_pin;
        int b_pin;
        int pow_pin;

    public: 
        PilotStatus();

        bool setStatusPins(int r_pin, int g_pin, int b_pin, int pow_pin);
        bool setStatus(int status, String message);
        int getStatus();
};