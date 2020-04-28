
//class for timeing
class TimeMeasurement
{

    static const int photoResistorPin = A0;

    uint32_t runtime = 0; //current runtime of the marble
    const int32_t lightThreshold = 20; //light threshold for laser gate

public:
    TimeMeasurement(){} // default Constrructor

    //start measurement
    void startTime()
    {
        runtime = millis();
    }

    //stop measurement
    void stopTime()
    {
        runtime = millis() - runtime;
    }

    //get measured time
    uint32_t getTime()
    {
        return runtime;
    }

    //if there is something blocking the laser
    bool obstacleDetected()
    {
        return analogRead(photoResistorPin) > lightThreshold;
    }
};
