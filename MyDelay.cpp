/*
  ==============================================================================

    MyDelay.cpp
    Created: 16 Dec 2021 12:17:40am
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

MyDelay::MyDelay() 
{
    Timer::startTimerHz(1);
}

MyDelay::~MyDelay()
{
    Timer::stopTimer();
}

void MyDelay::timerCallback()
{
    //getFromDelayBuffer();
}