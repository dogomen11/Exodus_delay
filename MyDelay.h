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
        int delay_buffer_length;
        void timerCallback() override;
        int marked = 0;
        
    public:
        bool d_on_off[NUM_OF_INSTENCES] = { 0 };
        bool d_reverb[NUM_OF_INSTENCES] = { 0 };

        MyDelay();
        ~MyDelay();
        void setSize(int new_num_channels, int new_num_samples);
        //void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data);
        //void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float* dry_buffer, float vol_dials[], float m_pan_dials[]);
        //void feedbackDelay(int channel, const int buffer_length, float* dry_buffer);
        void applyFX(AudioBuffer<float>& temp, bool instence, float* channelData, int channel, float volume, float pan);
        float calculatePanMargin(float pan, int channel);
        int getMarked() { return marked; }
        void addMark(int instence) {
            d_on_off[instence] = true;
            marked++;
        }
        void subMark(int instence) {
            d_on_off[instence] = false;
            marked--;
        }

};

#endif // MY_DELAY




