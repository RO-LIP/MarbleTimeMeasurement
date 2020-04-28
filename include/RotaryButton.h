#pragma once

#include <TimerOne.h>                //https://github.com/PaulStoffregen/TimerOne
#include <RotaryEncoderAdvanced.h>
#include <RotaryEncoderAdvanced.cpp> //for some reason linker can't find the *.cpp :(

//This class handels a rotary encoder  with push button KY-040



uint8_t PIN_CLK{5};
uint8_t DT_PIN{4};
uint8_t SW_PIN{3};

//Set pins and step, min max value for the encoder
RotaryEncoderAdvanced<uint8_t> encoder(PIN_CLK, DT_PIN, SW_PIN, 1, 1, 20);

//for debouncing
bool toggleButten = false;
bool newEncoderValue = false;
uint8_t lastEncoderValue = -1;

class RotaryButton
{
    //ISR for encoder
    static void encoderISR()
    {
        encoder.readAB();
        if(lastEncoderValue != encoder.getValue()) //update if we have a new value
        {
            newEncoderValue = true;
            lastEncoderValue = encoder.getValue();
        }

    }

    //ISR for button
    static void encoderButtonISR()//debouncing. Because we get an event by push and release button
    {
        if(toggleButten == false)
        {
            encoder.readPushButton();
            toggleButten = true;
            delay(20);
        }
        else
        {
            toggleButten = false;
            delay(20);
        }
    }

public:
    //default constructor
    RotaryButton()
    {
    }

     void init() //initialize
    {
        Timer1.initialize();                                                       //optionally timer's period can be set here in usec, default 1 sec. this breaks analogWrite() for pins 9 & 10

        encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

        Timer1.attachInterrupt(encoderISR, 1000);                                 //call encoderISR()    every 10000 microseconds/0.01 seconds
        attachInterrupt(digitalPinToInterrupt(SW_PIN), encoderButtonISR, FALLING); //call pushButtonISR() every high to low changes
    }

    //return true, if the button was pushed
    bool isButtonPushed()
    {
        return encoder.getPushButton() == true;
    }

    //set a new max range for the enconder [1 .. val]
    void setMaxVal(const uint8_t val)
    {
        encoder.setMaxValue(val);
    }

    //return true, if we have a new vaule on the encoder
    bool isnewEncoderValue()
    {
        return newEncoderValue;
    }

    //get the current encuoder value
    uint8_t getEncoderValue()
    {
        if(newEncoderValue)
        {
            newEncoderValue = false;
        }
        return encoder.getValue();
    }

    //return the encoder value to one
    void setToOne()
    {
        encoder.setValue(1);
    }

    //get the current max value for the encoder [1 .. maxVal]
    uint8_t getMaxValue()
    {
        return encoder.getMaxValue();
    }

    //reset properties
    void reset()
    {
        setToOne();
        setMaxVal(20);
    }
};
