#include "Location.h"
#include "Propeller.h"
#include "PilotStatus.h"

struct t {
    unsigned long t_start;
    unsigned long t_timeout;
};

class Target
{
    private:
        float bearing;
        float velocity;
        float altitude;

    public:
        bool setTarget(const float b, const float v, const float a) const;
        Target getTarget() const;
};

/*
 * Drone Class
*/

class Drone
{ 
    private:
        char transmissionAddress;

        Location *location;
        Target target;

        bool hovering = false;
        bool landed = true;
        bool landing = false;
        bool flying = false;

        PilotStatus status;

        // Task Schedulers
        bool tCheck(struct t *t);
        void tRun(struct t *t);
        void tAlter(struct t *t, unsigned long t_timeout);
    
    public:
        // Takes transmission address
        Drone(char _transmissionAddress[7], int _motor_pins[4]); 
        Drone();

        // Template Props
        Propeller propellers[4] = {
            Propeller(0, 5), // Location 0, pin 5 
            Propeller(1, 4), // Location 1, pin 4
            Propeller(2, 3), // Location 2, pin 3
            Propeller(3, 2)  // Location 3, pin 2
        };

        void setPropellerSpeed(const int prop, const int rpm) const;
        void setAllPropellerSpeeds(const int rpm) const;
    
        Location getLocation() const;

        float getVelocity() const;
        float getAltitude() const;
        float getBearing() const;

        float getPitch() const;
        float getRoll() const;
        float getYaw() const;

        float setHeading() const;

        void update();
        void hover();
        void land();
};