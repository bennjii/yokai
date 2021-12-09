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
         * Operational      7 (Purple)
         * Not Connected    8 (Red & Blue Alternate)
         */


    public:
        PilotStatus() {};

        unsigned long blink_time = 0;

        bool setStatusPins(int r_pin, int g_pin, int b_pin, int pow_pin);
        void runSwitch(int led_state, int led_status);
        void updateStatus();
        bool setStatus(int status);
        int getStatus();
};