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
#define INSTENCE_ON 1
#define INSTENCE_OFF 0


class MyDelay
{
    public: 
        struct Parameters
        {
            float delay_mix = 0.5f;
            float delay_time = 0.5f;
            float delay_feedback = 0.33f;
        };

    private:
        AudioBuffer<float> delay_buffer;
        int delay_buffer_length;
        double sample_rate;
        int on_off_marked = INSTENCE_OFF;
        int reverb_marked = INSTENCE_OFF;
        Parameters parameters;
        
    public:
        bool d_on_off[NUM_OF_INSTENCES] = { INSTENCE_OFF };
        bool d_reverb[NUM_OF_INSTENCES] = { INSTENCE_OFF };

        MyDelay();
        ~MyDelay() = default;
        void setSampleRate(double new_sample_rate);
        void setSize(int new_num_channels, int new_num_samples);
        void setParameters(const Parameters& new_params);
        void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data, int buffer_write_position);
        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position);
        void feedbackDelay(int channel, const int buffer_length, float* dry_buffer, int buffer_write_position);
        void applyPan(AudioBuffer<float>& temp, bool instence, float* channelData, int channel, float volume, float pan);
        float calculatePanMargin(float pan, int channel);
        int getOnOffMarked()                { return on_off_marked; }
        int getReverbMarked()               { return reverb_marked; }
        int getNumSamples()                 { return delay_buffer.getNumSamples(); }
        float getDelayTime()                { return parameters.delay_time; }
        float getDelayMix()                 { return parameters.delay_mix; }
        void addOnOffMarked(int instence);
        void subOnOffMarked(int instence);
        void addReverbMarked(int instence);
        void subReverbMarked(int instence);
};

#endif // MY_DELAY
