/*
  ==============================================================================

    MyDistortion.h
    Created: 7 Sep 2022 3:41:29pm
    Author:  Dogom

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MyFilter.h"

using namespace juce;

class MyDistortion
{
    public:
        struct Parameters {
            float dist_drive        = 0.4f;
            float dist_dry          = 0.4f;
            float dist_wet          = 0.4f;
            float dist_brightness   = 0.4f;
        };

    private:
        AudioBuffer<float> dist_buffer;
        int dist_buffer_length;
        double sample_rate;
        float balance;
        float drive_mult;
        Parameters parameters;
        LowpassHighpassFilter brightness_filter;


    public:
        MyDistortion();

        void setSampleRate(double new_sample_rate);
        void setSize(int new_num_channels, int new_num_samples);
        int getNumSamples() { return dist_buffer.getNumSamples(); }
        const Parameters& getParameters() const { return parameters; }
        void setParameters(const Parameters& new_params);
        void setBalance();
        void setDriveMult();
        void prepareFilter(const dsp::ProcessSpec& spec);
        void reset();
        float distorter(float to_distort, float balance);
        void process(AudioBuffer<float>& buffer, int channel, int buffer_write_positions, dsp::AudioBlock<float> audio_block);

};