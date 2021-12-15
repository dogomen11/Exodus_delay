/*
  ==============================================================================

    MyDelay.h
    Created: 16 Dec 2021 12:17:40am
    Author:  dogom

  ==============================================================================
*/
#ifndef MY_DELAY
#define MY_DELAY
#pragma once

#include <JuceHeader.h>
using namespace juce;
#define NUM_OF_INSTENCES 16

class MyDelay : public Timer
{
    private:
        AudioBuffer<float> delay_buffer;
        void timerCallback() override;
    public:
        MyDelay();
        ~MyDelay();
        void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data);
        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float* dry_buffer, float vol_dials[], float m_pan_dials[]);
        void feedbackDelay(int channel, const int buffer_length, float* dry_buffer);

 
};

#endif // MY_DELAY




