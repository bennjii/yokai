class PID
{
    public:
        float KP, KI, KD;
        float LimitP, LimitI, LimitD;
        float Integral;

        PID(float k1, float k2, float k3, float l1, float l2, float l3, float integralbase = 0)
        {
            KP = k1;
            KI = k2;
            KD = k3;
            LimitP = l1;
            LimitI = l2;
            LimitD = l3;
            Integral = integralbase;
        }
        
        float limit(float val, float limit);
        float gain(float timePassed, float aciP, float aciD, bool updateIntegral);
};