#include "PID.h"

float PID::limit(float val, float limit) 
{
    if(val > limit) return limit;
    else if(val < -limit) return -limit;
    return val;
}

float PID::gain(float timePassed, float aciP, float aciD, bool updateIntegral) 
{
    if(updateIntegral)
    Integral = limit(Integral + KI * timePassed * aciP, LimitI);
    return limit(KP * aciP, LimitP) + Integral + limit(KD * aciD, LimitD);
}