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

class MyDelay
{
    private:
        AudioBuffer<float> delay_buffer;
        int delay_buffer_length;
        int on_off_marked = 0;
        int reverb_marked = 0;
        double sample_rate;
        float delay_time;
        float delay_feedback;
        float delay_mix;

    public:
        bool d_on_off[NUM_OF_INSTENCES] = { 0 };
        bool d_reverb[NUM_OF_INSTENCES] = { 0 };

        MyDelay();
        ~MyDelay() = default;
        void setSize(int new_num_channels, int new_num_samples);
        void setSampleRate(double new_sample_rate);
        void setDelayTime(float new_delay_time);
        void setDelayFeedback(float new_delay_feedback);
        void setDelayMix(float new_delay_mix);
        void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data, int buffer_write_position);
        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position);
        void feedbackDelay(int channel, const int buffer_length, float* dry_buffer, int buffer_write_position);
        void applyFX(AudioBuffer<float>& temp, bool instence, float* channelData, int channel, float volume, float pan);
        float calculatePanMargin(float pan, int channel);
        int getOnOffMarked() { return on_off_marked; }
        int getReverbMarked() { return reverb_marked; }
        void addOnOffMarked(int instence);
        void subOnOffMarked(int instence);
        void addReverbMarked(int instence);
        void subReverbMarked(int instence);
        int getNumSamples() { return delay_buffer.getNumSamples(); }
        float getDelayTime() { return delay_time; }
        float getDelayMix() { return delay_mix; }
};

#endif // MY_DELAY




