
// YOKAI @ 2021

#include <Arduino.h>
#include <mbed.h>
#include <AutoPilot.h>

// Initialise the remote adress refrence for transmission.
int motor_pins[4] = {5, 6, 7, 8};

const char* remote_adress = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* characteristic = "19b10001-e8f2-537e-4f6c-d104768a1214";

Drone drone;
AutoPilot pilot;

void setup()
{
	// Create drone, and autopilot pilot.
	Drone __drone(remote_adress, characteristic, motor_pins);
	AutoPilot __pilot(&__drone);

	drone = __drone;
	pilot = __pilot;

	// Set the destination
	// pilot->setCourse(Location(1728, 1920));

	// Ensures everything is primed, but not running - awaits command for flight begin.
	pilot.awaitCommand();  
}

void loop()
{    
	// Run PID Calculations
	pilot.update(); 
	
	// const float distance_to_destination = drone->location->distanceTo(pilot.getTargetLocation());
	// Serial.println(distance_to_destination);
}