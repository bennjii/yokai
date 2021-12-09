#ifndef Location_h
	#define Location_h

	#include <Arduino.h>
	
	#define EARTH_MEAN_RADIUS_METRES 6371000
	
	class Location
	{
		private:
			float latitude;
			float longitude;
			
		public:
			Location(const Location&);
			Location(const float, const float);
			float distanceTo(const Location&) const;				// returns distance in metres
			float getLatitude() const;
			float getLongitude() const;
            float degToRadians(const float &deg) const;
			String toString() const;
	};

	struct LandingZone
	{
		public:
			const Location *location;
			float bearing;
			float altitude;

			LandingZone(const Location*, const float, const float);
	};
#endif