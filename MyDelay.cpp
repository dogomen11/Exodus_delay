/*
  ==============================================================================

    MyDelay.cpp
    Created: 16 Dec 2021 12:17:40am
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

float calculatePanMargin(float pan, int channel);

MyDelay::MyDelay() : delay_buffer(),
                     marked(0)
{
    Timer::startTimerHz(1);
}

MyDelay::~MyDelay()
{
    Timer::stopTimer();
}

void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = delay_buffer.getNumSamples();
}

void MyDelay::timerCallback()
{
    //getFromDelayBuffer();
}

void MyDelay::applyFX(AudioBuffer<float>& temp, bool instence, float* channelData, int channel, float volume, float pan)
{
    float pan_mult = 1;
    if (pan != 0)
    {
        pan_mult = calculatePanMargin(pan, channel);
    }
    for (int sample = 0; sample < temp.getNumSamples(); ++sample)
    {
        if (instence == 1)
        {
            channelData[sample] = temp.getSample(channel, sample) * volume * pan_mult;
        }
        else
        {
            channelData[sample] = temp.getSample(channel, sample) * 0;
        }
    }
}

float MyDelay::calculatePanMargin(float pan, int channel)
{
    if (channel == 0)
    {
        if (pan < 0)
        {
            return 1;
        }
        else if (pan > 0)
        {
            return 1 - square(pan);
        }
    }
    else if (channel == 1)
    {
        if (pan > 0)
        {
            return 1;
        }
        else if (pan < 0)
        {
            return 1 - square(pan);
        }
    }
    return 1;
}