/*
  ==============================================================================

    MyDelay.h
    Created: 24 Aug 2021 12:47:21pm
    Author:  dogom

  ==============================================================================
*/
#ifndef MYDELAY
#define MYDELAY
#pragma once
#include <JuceHeader.h>
#include "MyReverb.h"

using namespace juce;

#define NUM_OF_INSTENCES 16

//==============================================================================

class MyDelay
{
private:

    // Delay initial parameters

    int write_position;
    int sample_rate;
    bool instences[NUM_OF_INSTENCES] = {};
    bool reverb_instences[NUM_OF_INSTENCES] = {};
    int marked_instences;
    float delay_mix;
    int delay_time;
    float input_gain;
    float output_gain;
    AudioBuffer<float> delay_buffer;
    MyReverb delay_reverb;
    int delay_buffer_length = 0;
    int outputing_stage = 0;
    int time_strecher = 0;

    float calculatePanMargin(float debug_3, int channel);
    void addDelayinstenceToBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>&  temp, int channel, int buffer_lenth, const int read_position);
    void applyFX(AudioBuffer<float>& temp, bool instence, float* channelData, int channel, float volume, float pan);

public:

    MyDelay();
    ~MyDelay();

    MyReverb debug_4;      //TODO delete after debug

    void setDelayMix(float delay_mix_chosen);
    void setDelayTime(int delay_time_chosen);
    void setInputGain(float in_gain_chosen);
    void setOutputGain(float out_gain_chosen);

    void setInputBuffer(AudioBuffer <float>& new_buffer);

    void updateArgs(int m_write_position, int m_sample_rate, bool m_on_off_button_array[], bool m_reverb_array[], float m_delay_feedback, int m_delay_time);
    void setSize(int new_num_channels, int new_num_samples);
    int getNumSamples();
    const float* getReadPointer(int channelNumber);

    void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data);

    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float* dry_buffer, float vol_dials[], float m_pan_dials[]);

    void feedbackDelay(int channel, const int buffer_length, float* dry_buffer);

    

    void addInstence(int instance_num);
    void decreseInstence(int instance_num);
    int isMarked();
    int debugReturnOutStage() { return outputing_stage; }
};

#endif     // MYDELAY