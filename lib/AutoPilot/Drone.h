#include "Location.h"
#include "Propeller.h"
#include "PilotStatus.h"
#include <Arduino_LSM9DS1.h>
// #include <ArduinoSTL.h>

/*
 * Time Scheduler
 * 
 * @param t_start unsigned long; Defines the time last or first instantiated for the task
 * @param t_timeout unsigned long; Defines the duration between or till action call
 * @param t_volatile boolean; Should the call be restacked (t/f)
 * @param t_back void; Function Completion Callback
 */
struct t {
    unsigned long t_start;
    unsigned long t_timeout;
    bool t_volatile;
    // std::function<bool(int)> t_back;

    // Task Schedulers
    void tAlter(unsigned long t_timeout);
    bool tCheck();
    void tRun();
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
 * [YOKAI] ~ 2021
 * 
 * (A↻) (B↺)                
 *    \ /             y       z (vertical)
 *    |+|             \     ↑
 *    |-|               _  |
 *    / \              \+\
 * (C↺) (D↻)           \-\  ----→ x (horisontal left-right)
 *                       ‾
 *                     
 * Motors A & D run clockwise.
 * Motors B & C run counter-clockwise.
 * 
 * Board: [+-] 
 *      + indicates BLE sensor (Board Front)  
 *      - indicates USB port (Board Rear)
 * 
 * The z axis is vertical to the horisontal plane of the board  ~>  up/down.
 * The y axis indicates forward and backward as proportional to the board +/-   ~>   forward/backward.
 * The x axis is perpendicular to the y axis and indicates movements  ~>  left/right.
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

        // Accelerometer    X, Y, Z Direction
        

        // Gyroscope        X, Y, Z Direction
        float gx, gy, gz;

        // Magnetometer     X, Y, Z Direction
        float mx, my, mz;

        PilotStatus status;

        // std::vector<t> tasks;

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

        bool updateAcceleration();

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