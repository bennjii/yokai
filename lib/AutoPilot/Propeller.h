#ifndef Propeller_h
	#define Propeller_h
	#define EARTH_MEAN_RADIUS_METRES 6371000
	
	class Propeller
	{
		public: 
            float rpm = 0;
            int location; // 1, 2, 3, 4
            int pin;

            /*
            * (A) (B)     x  
            *   \ /     z ↑
            *    X       \|
            *   / \       +----→ y
            * (C) (D)
            *
            * Motors A & D run clockwise.
            * Motors B & C run counter-clockwise.
            */
    
            Propeller(int _location, int _pin);

            void setPropellerSpeed(float _rpm);
            void setPin(int _pin);

            int getLocation() const;
	};
#endif