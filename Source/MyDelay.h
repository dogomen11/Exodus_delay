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
#include "MyDistortion.h"
using namespace juce;

#define NUM_OF_INSTENCES 16
#define INSTENCE_ON 1
#define INSTENCE_OFF 0


class MyDelay
{
    public: 
        struct Parameters
        {
            float delay_mix = 0.7f;
            float delay_time = 0.5f;
            float delay_feedback = 0.33f;
            float delay_volume = 1.0f;
            float delay_pan = 0.0f;
        };
        AudioBuffer<float> dry_delay_buffer, wet_delay_buffer;
        int delay_buffer_length;
        double sample_rate;
        int on_off_marked = INSTENCE_OFF;
        int reverb_marked = INSTENCE_OFF;
        int dist_marked = INSTENCE_OFF;
        Parameters parameters;
        bool d_on_off[NUM_OF_INSTENCES] = { INSTENCE_OFF };
        bool d_reverb[NUM_OF_INSTENCES] = { INSTENCE_OFF };
        bool d_dist[NUM_OF_INSTENCES] = { INSTENCE_OFF };
        dsp::Reverb reverb;
        dsp::Reverb::Parameters reverb_params;
        MyDistortion distortion;
        MyDistortion::Parameters dist_params;

        MyDelay();
        ~MyDelay() = default;
        void setSampleRate(double new_sample_rate);
        void setSize(int new_num_channels, int new_num_samples);
        void setParameters(const Parameters& new_params);
        void fillDelayBuffer(int channel, const int buffer_length, const float* read_pointer, int buffer_write_position);
        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position);
        void feedbackDelay(int channel, const int buffer_length, float* write_pointer, int buffer_write_position);
        void applyFX(int channel);
        void applyPan(int channel);
        void applyVolume(int channel);
        void applyReverb(int channel);
        void applyDist(int channel);
        float calculatePanMargin(float pan, int channel);
        int getOnOffMarked()                { return on_off_marked; }
        int getReverbMarked()               { return reverb_marked; }
        int getNumSamples()                 { return dry_delay_buffer.getNumSamples(); }
        float getDelayTime()                { return parameters.delay_time; }
        float getDelayMix()                 { return parameters.delay_mix; }
        void addOnOffMarked(int instence);
        void subOnOffMarked(int instence);
        bool addReverbMarked(int instence);
        bool subReverbMarked(int instence);
        bool addDistMarked(int instence);
        bool subDistMarked(int instence);
};

#endif // MY_DELAY
