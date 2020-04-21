#include <Arduino.h>

#include "Display.h"
#include "RaceData.h"
#include "RotaryButton.h"
#include "TimeMeasurement.h"

RotaryButton button; //Rotary encoder with push button KY-040
Display display;     //4-line I2C display
RaceData raceTime;   //RaceData handle
TimeMeasurement timing; //Timing and laser barrier handling

uint8_t marbleCount = 1;   //how many marbles are in the race
uint8_t marbleRounds = 1;  //how many laps has each marble
uint8_t currentRound = 0;  //the current lap of the marble

const uint8_t maxRounds = 4;  //maximum of rounds to select

//handling the state machine 
enum class ProgramStates
{
   StartUp,       //Program starts
   selectMarbles, //Select how many marbles are takeing part
   selectRounds,  //Select how many round each marble has
   startMarble,   //Wait for the marble to start
   runMurble,     //Marble in on the track
   Ranking,     //show ranking
} m_states;




void setup() 
{
  m_states = ProgramStates::StartUp; //start into startup
  //init our classes
  button.init();  
  display.init();
  display.startScreen(); //show intro screen
}

void loop() 
{
  
  switch (m_states) //select state
  {
    case ProgramStates::StartUp:

        if (button.isButtonPushed()) //wait for button
        {
          display.marbleCount(marbleCount);        //display marble number select
          m_states = ProgramStates::selectMarbles; //change state
          delay(1000);                             //wait a little bit
        }
        break;
        
    case ProgramStates::selectMarbles: //Select how many marbles are takeing part
      {
        if(button.isnewEncoderValue())  //if the encoder has a new value
        {
          marbleCount = button.getEncoderValue(); //change count
          display.marbleCount(marbleCount);       //display change
        }
        if (button.isButtonPushed())    //if the button is pushed
        {

          display.selectRounds(1);               //change display
          raceTime = RaceData(marbleCount);      //set information in race data
          button.setMaxVal(maxRounds);           //for rounds selection. 
          m_states = ProgramStates::selectRounds;//change state
          delay(1000);                           //wait a little bit
          
        }
        
      } 
      break;


    case ProgramStates::selectRounds:               //Select how many round each marble has
       if(button.isnewEncoderValue())               //if the encoder has a new value
        {
          marbleRounds = button.getEncoderValue();  //change rounds
          display.selectRounds(marbleRounds);       //display change
        }

        if (button.isButtonPushed())                //if the button is pushed
        {
          display.marbleStart();                    //display change
          m_states = ProgramStates::startMarble; //change state
          delay(1000);                           //wait a little bit
        }
   
      break;

    case ProgramStates::startMarble:            //Wait for the marble to start
      
      if(timing.obstacleDetected())             //if we detect the marble
        {
          timing.startTime();                   //start time measurement
          currentRound = 0;                     //reset rounds to zero
          m_states = ProgramStates::runMurble;  //change state
          delay(1000);                          //wait a little bit so the marbel is not in the laser barrier anymore
        }
      break;

    case ProgramStates::runMurble:              //Marble in on the track
    {
      if(timing.obstacleDetected())             //if we detect the marble        
      {
          timing.stopTime();                    //meassure time
          bool newMarbleBesttime = raceTime.newTime(timing.getTime());  //write time into data set
          display.newMarbleTime(timing.getTime(),newMarbleBesttime);    //update display
          if(raceTime.getNewHighScore())         //if we have a new best lap time
          {
            display.newHighScore(timing.getTime(),raceTime.getCurrentMarble()); //update Display
          }
           ++currentRound; //go to next round

          if (currentRound != marbleRounds) //start next round
          {
             
              timing.startTime(); //start meassurement
              delay(1000);    //wait a little bit so the marbel is not in the laser barrier anymore
          }
          else //no more rounds for this marble
          {

            if(raceTime.nextMarble()) //start the next marble
            {
               delay(1500); //wait a little bit

              display.changeMarbleNumber(raceTime.getCurrentMarble()); //update Display
              m_states = ProgramStates::startMarble;    //wait for the marble to start
            }
            else //this was the last marble
            {
              raceTime.generateRanking(); //generate ranking
              button.setToOne();          //reset Encoder value

              if(marbleCount > 3) //set encoder values for ranking
              { 
                  button.setMaxVal(marbleCount -2); 
              }
              else
              {
                  button.setMaxVal(1);
              }
              delay(1500); //wait a little bit
              display.showHighScore(raceTime.getRanking(),button.getEncoderValue()); //show Highscore
              m_states = ProgramStates::Ranking; //change state to ranking
              
            }     

          }
      }
    }
    break;


    case ProgramStates::Ranking: //show ranking
    {
      if (button.isButtonPushed())  //if we get a button event, restart
      {
        display.startScreen(); //startup display
        button.reset();        //reset all button configuration
        m_states = ProgramStates::StartUp; //start from the beginning
      }

      if(button.isnewEncoderValue()) //if we get a new encoder Value
      {
        //change display
        display.showHighScore(raceTime.getRanking(),button.getEncoderValue());
        
      }
      delay(300);
      
    }
      break;
    
    default:
      break;
  }

}