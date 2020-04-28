#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <estd/array.h>
#include <MarbleData.h>


class Display
{
 LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

public:
    Display()
        {

        }
    void init()
    {
        lcd.init();                      // initialize the lcd
        lcd.backlight();
    }

    void startScreen()
    {
        lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("It's MARBLE TIME");
            lcd.setCursor(4,1);
            lcd.print("MARBLE TIME ");
            lcd.setCursor(1,2);
            lcd.print("MESSUREMENTS V 0.1");
            lcd.setCursor(5,3);
            lcd.print("RO-LIP (TG)");
    }

    void marbleCount(const uint8_t number)
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select Marble Count:");
        lcd.setCursor(8,1);
        lcd.print(number);

        lcd.setCursor(0,3);
        lcd.print("Press to start");
    }

    void selectRounds(const uint8_t number)
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select  Rounds:");
        lcd.setCursor(8,1);
        lcd.print(number);

        lcd.setCursor(0,3);
        lcd.print("Press to start");
    }

    void marbleStart()
    {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("H: ");
        lcd.setCursor(3,1);
        lcd.print("Run marble ");
        lcd.print(1);
        lcd.setCursor(0,2);
        lcd.print("Current: ");
        lcd.setCursor(0,3);
        lcd.print("Best: ");
    }

    void changeMarbleNumber(const uint8_t marbleNumber)
    {
        lcd.setCursor(14,1);
        lcd.print("    ");
        lcd.setCursor(14,1);
        lcd.print(marbleNumber);
        lcd.setCursor(0,2);
        lcd.print("Current:          ");
        lcd.setCursor(0,3);
        lcd.print("Best:             ");
    }

    void newMarbleTime(const uint32_t timeInMillis,const bool newBest)
    {
        auto timeText = MilliToText(timeInMillis);
        lcd.setCursor(9,2);
        lcd.print(timeText);
        if(newBest)
        {
            lcd.setCursor(9,3);
            lcd.print(timeText);
        }
    }

    void newHighScore(const uint32_t timeInMillis,const uint8_t marbleNumber)
    {
        lcd.setCursor(6,0);
        lcd.print("          ");
        lcd.setCursor(6,0);
        lcd.print("No ");
        lcd.print(marbleNumber);
        lcd.print(" T ");
        lcd.print(MilliToText(timeInMillis));

    }

    void showHighScore(const estd::array<MarbelData,9>& ranking,const uint8_t position)
    {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("HighScores ");
        for(int i = 0; i < 3; ++i)
        {
            //ignore invalid time
            if(ranking[(position+i)-1].marbleBestTime == 0xffffffff)
                continue;
            lcd.setCursor(0,i+1);
            lcd.print(position + i);
            lcd.print(". Marb. ");
            lcd.print(ranking[(position+i)-1].marbleNumber);
            lcd.print(" T: ");
            lcd.print(MilliToText(ranking[(position+i)-1].marbleBestTime));
        }

    }

    String MilliToText(const uint32_t timeInMillis)
    {
        uint32_t seconds = timeInMillis / 1000;
        uint32_t milliseconds = timeInMillis - (seconds * 1000);
        String s = "";
        if(seconds >= 10)
        {
            s = "0";
        }
        s += String(seconds)+ ":";

        if(milliseconds < 100)
        {
            s += "0";
        }
        if(milliseconds < 10)
        {
            s += "0";
        }
        s +=  String(milliseconds);
        s +=  "s";
        Serial.println(s);
        return s;
    }

};
