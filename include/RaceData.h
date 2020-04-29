#include <estd/array.h>
#include <estd/algorithm.h>
#include "MarbleData.h"



//Handling the data of the Race
class RaceData
{
    uint8_t marbleMaxNumber = 0;        //how many marbles are participating
    uint8_t currentMarble = 0;          //current running marble
    estd::array<MarbelData,20> marbleTimes = estd::array<MarbelData,20>(); //the race data
    estd::array<MarbelData,20> marbleRanking = estd::array<MarbelData,20>(); //the ranking data
    MarbelData bestLapMarble;       // marble with the best lap time
    bool newBestLap = false;        // do we have a new best lap time



public:
    RaceData(){}; //default constructor

    //Konstruktor with Marble data
    RaceData(const uint8_t marbleNumber_) : marbleMaxNumber(marbleNumber_)
    {
        for(auto i = 0; i < marbleMaxNumber; ++i) //set marble number one higher than index
        {
            marbleTimes[i].marbleNumber = i+1;
        }
    }

    //how many marbles are participating
    uint8_t getmarbleNumber() const
    {
        return marbleMaxNumber;
    }

    //current running marble
    uint8_t getCurrentMarble() const
    {
        return marbleTimes[currentMarble].marbleNumber;
    }

    //we have a new time for a lap
    bool newTime( const uint32_t marbleTime)
    {
        //check if the marble has a new best lap time
        if(marbleTimes[currentMarble].marbleBestTime > marbleTime)
        {
            marbleTimes[currentMarble].marbleBestTime = marbleTime;

            //check if we have a new overall beset lap time
            if(marbleTime < bestLapMarble.marbleBestTime)
            {
                bestLapMarble.marbleBestTime = marbleTime;
                bestLapMarble.marbleNumber = currentMarble;
                newBestLap = true;
            }
            return true;
        }
        return false;
    }

    // do we have a new best lap time
    bool getNewHighScore()
    {
        if(newBestLap == true)
        {
            newBestLap = false;
            return true;
        }
        return newBestLap;
    }

    //get to the next marble. If this was the last one, return false.
    bool nextMarble()
    {
        if(marbleTimes[currentMarble].marbleNumber == marbleMaxNumber)
        {
            return false;
        }
        ++currentMarble;
        return true;
    }

    void generateRanking()
    {
        //iterate over all best marbles
        for(auto i {0}; i < marbleMaxNumber; ++i)
        {
            //get the fastest time
            auto element = estd::min_element(marbleTimes.begin(), marbleTimes.end(),
            [](const MarbelData& lhs, const MarbelData& rhs){ return lhs.marbleBestTime < rhs.marbleBestTime;});
            //set on the current place
            marbleRanking[i] = *element;
            //erase data
            element->marbleBestTime = -1;
        }
    }

    const estd::array<MarbelData,20>& getRanking()
    {
        return marbleRanking;
    }

};
