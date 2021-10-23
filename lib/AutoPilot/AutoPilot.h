#include "Drone.h"
#include "Location.h"
class AutoPilot
{
	private:
		Drone *drone;

		Location *targetLocation;
		Target *target;
		float targetAltitude = 0.0f;

		float errors[3];					// Measured Errors
		float error_sum[3] = {0, 0, 0};		// Integral Errors
		float previous_error[3] = {0, 0, 0};// Derivative Erroring

		float Kp[3] = 	{0, 0.9, 0.9}; 		// P coefficient YAW, PITCH, ROLL
		float Ki[3] = 	{0.00, 0.00, 0.00}; // I coefficient YAW, PITCH, ROLL
		float Kd[3] = 	{0, 13, 13}; 		// D coefficient YAW, PITCH, ROLL	

		bool hovering = false;
		bool stationary = false;
		bool activeAvoidance = false; // nac
		
	public:
		AutoPilot(Drone* _drone) { drone = _drone; };
		AutoPilot() {};

		void update();
		void awaitCommand() const;
		
		bool setTarget(const float bearing, const float velocity, const float altitude) const;
		Location getTargetLocation() const;
		Location getLocation() const;

		void testFlightControls() const;
		void beginFlight();
		void land() const;
		void hover() const;
		void abort();

		void enableActiveAvoidance() const;
		void disableActiveAvoidance() const;

		void setCourse(const Location&);
		bool setBearing(const float target) const;
		float getHeading(const Location &from, const Location &to) const;

		bool keepOnCourse() const;
		void setAltitude(const float alt);
		bool maintainAltitude() const;

		void turn(const float) const;				
		float getETA() const;								
};