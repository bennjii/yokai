#include "Drone.h"

t drone_status = { 0, 250, false };
t acel_update = { 0, 3, false };
t gyro_update = { 1, 3, false };
t magn_update = { 2, 3, false };
t t_update = { 0, 100, false };
t bt_update = { 0, 1, false };

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

int throttle = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

bool connected = false;
bool idle = true;

BLEService positionService("19b10000-e8f2-537e-4f6c-d104768a1214"); 
BLECharacteristic throttleCharacteristic("eb1e2061-c554-4307-9309-94144c5c09b1", BLERead | BLEWrite | BLENotify, 4);
BLECharacteristic yawCharacteristic("42ebfd9a-0faf-40f2-b03a-aaba86a90c18", BLERead | BLEWrite | BLENotify, 4);
BLECharacteristic rollCharacteristic("ee05112e-d0dd-44c9-be7a-3a04f57c2640", BLERead | BLEWrite | BLENotify, 4);
BLECharacteristic pitchCharacteristic("237f9fba-52c5-4e23-826d-d3f7581c5e4c", BLERead | BLEWrite | BLENotify, 4);

/*
 * Drone Initialization
 * @param _ta Transmission Address
 * @param m_pins Motor Pins (4)
 */
Drone::Drone(const char* _ta, const char* ch, int m_pins[4])
{
    deviceServiceUuid = _ta;
    deviceServiceCharacteristicUuid = ch;

    Serial.println("[TASK]:\tInitializing Drone");

    // for(int i = 0; i < 4; i++) {
    //     propellers[i].setPin(m_pins[i]);
    // }

    if (!IMU.begin()) {
        Serial.println("[ERR]: Error initializing IMU sensor failed!");
        status.setStatus(5);
        while(1);
    }

    if (!BLE.begin()) {
        Serial.println("[ERR]: Error initializing BLE module failed!");
        while (1);
    }

    BLE.setLocalName("YOKAI"); 
    BLE.setDeviceName("YOKAI");

    BLEService connectionService(deviceServiceUuid); 

    BLE.setAdvertisedService(positionService);
    positionService.addCharacteristic(throttleCharacteristic);
    positionService.addCharacteristic(yawCharacteristic);
    positionService.addCharacteristic(rollCharacteristic);
    positionService.addCharacteristic(pitchCharacteristic);

    BLE.addService(positionService);
    
    BLE.advertise();

    // Recieve 10ms of information MAXIMUM,
    // Reduced delay timeout when awating packet fufilment
    Serial.setTimeout(10);

    // 22 - R,     23 - G,     24 - B,     25 - POW
    status.setStatusPins(22, 23, 24, 25);

    // Set Status - Operational
    status.setStatus(7);
}

Drone::Drone() {}

long start_time = micros();
long running_avg = 0;
size_t runs = 0;

/*
 * aprox. 16runs / ms in std.arduino.clockcycle
 * Function Run cyclically. 
 * Should not be called >1 per loop, or outside of loop.
 * 
 * If AutoPilot is included, run secondary to pilot update.
 */
void Drone::update()
{
    // long time = micros() - start_time;
    // double scaling = 1. / (double)(runs + 1);
    // running_avg = time * scaling + running_avg * (1. - scaling);
    // runs++;

    // start_time = micros();

    /*
        BLE VALUES
        
        Externally Controlled
            Left Stick (Default 0'd)
            - Trottle (Right Stick Up & Down) +ve only
            - Yaw (Rudder - Left Stick Left & Right) +ve and -ve

            Right Stick (Default Centered)
            - Roll (Right Stick Left & Right) +ve and -ve
            - Elevator (Left Stick Up & Down) +ve and -ve (climb and fall)

        Externally Viewed
            - Velocity (X,Y,Z)
            - Acellerometer (X,Y,Z)
            - Motor RPM's (1,2,3,4)

    Actions:
                Raise                           Forward
                  |                                |
    Spin Left   -[|]-  Spin Right   Shift Left   -[|]-   Shift Right
                  |                                |
                Lower                           Backward

    Programation:
             Throttle Up                        Pitch Up
                  |                                |
    Yaw Left    -[|]-  Yaw Right     Roll Left   -[|]-   Roll Right
                  |                                |
             Throttle Down                      Pitch Down
    */

    BLEDevice central = BLE.central();

    if(bt_update.tCheck()) { 
        if(central && central.connected()) {
            status.setStatus(3);
            connected = true;

            throttle = readCharacteristic(throttleCharacteristic, throttle);
            yaw = readCharacteristic(yawCharacteristic, yaw);
            roll = readCharacteristic(rollCharacteristic, roll);
            pitch = readCharacteristic(pitchCharacteristic, pitch);

            idle = false;
        } else {
            connected = false;
            // Drone is NOT connected to external control authority - app.
            status.setStatus(8);
        }

        bt_update.tRun();
    }

    if(t_update.tCheck()) { 
        Serial.print(throttle);
        Serial.print("\t");
        Serial.print(yaw);
        Serial.print("\t");
        Serial.print(roll);
        Serial.print("\t");
        Serial.print(pitch);
        Serial.println("");
        t_update.tRun();
    }

    if (acel_update.tCheck()) {
        updateAcceleration();
        acel_update.tRun();
    }

    if (gyro_update.tCheck()) {
        updateGyrosphere();
        gyro_update.tRun();
    }

    if (magn_update.tCheck()) {
        updateMagnetometer();
        magn_update.tRun();
    }

    // Run any queued tasks in tTaskManager
    // for(size_t i = 0; i < tasks.size(); i++)
    // {
    //     // Verify Function SHOULD be run.
    //     if(tasks[i].tCheck()) {
    //         // Execute Function
    //         // tasks[i].t_back();
    //
    //         // Propogate Run Changes
    //         tasks[i].tRun();
    //
    //         // Remove tasks as neccesary
    //         if(tasks[i].t_volatile) {
    //             tasks.erase(i);
    //         } 
    //     }
    // }

    // START - Status DEMO
    // if (drone_status.tCheck()) {
    //     status.updateStatus();

    //     Serial.print("\tavg\t"); Serial.print(running_avg); 
    //     Serial.print("\tin \tms"); Serial.println((double) time / 1000);

    //     drone_status.tRun();
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

int Drone::readCharacteristicValue(BLECharacteristic characteristic) {
    uint8_t value[4] = {};
    int res = 0;

    characteristic.readValue(value, 4);

    for(int i = 0; i< 4; i++) {
        int val = (int)value[i] - '0';

        if(val >= 0) {
            res *= 10;
            res += val;
        }
    }

    return res;
}

int Drone::readCharacteristic(BLECharacteristic characteristic, int prev_value) {
    if (characteristic.written()) {
        return readCharacteristicValue(characteristic) - 200;
    }else {
        return prev_value;
    }
}

float Drone::getPitch() const
{
    // if (ax > 0.1) {
    //     Serial.print("U");

    //     return map(100 * ax, 0, 97, 0, 90);
    // }else if (ax < -0.1) {
    //     Serial.print("D");
    
    //     return map(100 * ax, 0, -100, 0, 90);
    // }

    return 180 * atan(ax / sqrt(ay*ay + az*az)) / PI;
}

float Drone::getRoll() const
{
    // if (ay > 0.1) {
    //     Serial.print("L");

    //     return map(100 * ay, 0, 97, 0, 90);
    // }else if (ay < -0.1) {
    //     Serial.print("R");

    //     return map(100 * ay, 0, -100, 0, 90);
    // }

    return 180 * atan(ay /sqrt(ax*ax + az*az)) / PI;
}

float Drone::getYaw() const
{
    return 180 * atan(az / sqrt(ax*ax + az*az)) / PI;
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

void Drone::connectToPeripheral() {
    BLEDevice peripheral;

    do {
        BLE.scanForUuid(deviceServiceUuid);
        peripheral = BLE.available();
    } while (!peripheral);
        
    if (peripheral) {
        Serial.println("* Peripheral device found!");
        Serial.print("* Device MAC address: ");
        Serial.println(peripheral.address());
        Serial.print("* Device name: ");
        Serial.println(peripheral.localName());
        Serial.print("* Advertised service UUID: ");
        Serial.println(peripheral.advertisedServiceUuid());
        Serial.println(" ");
        BLE.stopScan();
    }
}

bool Drone::updateAcceleration() {
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
    }

    return true;
}

bool Drone::updateGyrosphere() {
    if (IMU.gyroscopeAvailable()) {
        return IMU.readGyroscope(gx, gy, gz);
    }

    return true;
}

bool Drone::updateMagnetometer() {
    if (IMU.magneticFieldAvailable()) {
        return IMU.readMagneticField(mx, my, mz);
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