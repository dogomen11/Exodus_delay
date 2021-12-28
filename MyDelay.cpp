/*
  ==============================================================================

    MyDelay.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/
#include <math.h>
#include "MyDelay.h"

#define PI 3.141
static float calculatePanMargin(float pan, int channel);

MyDelay::MyDelay() : write_position(0.0),
                    marked_instences(0),
                    delay_mix(0.0),
                    delay_time(0.3),
                    input_gain(1.0),
                    output_gain(1.0),
                    debug_4()
{
   delay_buffer.clear();
}

MyDelay::~MyDelay() {}

void MyDelay::setDelayMix(float delay_mix_chosen)
{
    delay_mix = delay_mix_chosen;
}

void MyDelay::setDelayTime(int delay_time_chosen)
{
    delay_time = delay_time_chosen;
}

void MyDelay::setInputGain(float in_gain_chosen)
{
    input_gain = in_gain_chosen;
}

void MyDelay::setOutputGain(float out_gain_chosen)
{
    input_gain = out_gain_chosen;
}

void MyDelay::setInputBuffer(AudioBuffer<float>& new_buffer)
{
    setSize(new_buffer.getNumChannels(), new_buffer.getNumSamples());
}

void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = delay_buffer.getNumSamples();
}

void MyDelay::updateArgs(int m_write_position, int m_sample_rate, bool m_on_off_button_array[], bool m_reverb_array[], float m_delay_feedback, int m_delay_time)
{
    write_position = m_write_position;
    sample_rate = m_sample_rate;
    delay_mix = m_delay_feedback;
    delay_time = m_delay_time;
    marked_instences = 0;
    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        instences[i] = m_on_off_button_array[i];
        if (m_on_off_button_array[i] == true)
        {
            marked_instences++;
        }
        reverb_instences[i] = m_reverb_array[i];
    }
}

int MyDelay::getNumSamples()
{
    return delay_buffer.getNumSamples();
}

const float* MyDelay::getReadPointer(int channelNumber)
{
    return delay_buffer.getReadPointer(channelNumber);
}


void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data)
{
    //TODO changed copy to add ----> maybe crash! or worse clip
    AudioBuffer<float> temp(delay_buffer);
    const float* temp_data = temp.getReadPointer(channel);
    float debug_4 = delay_mix * 0.6;
    if (delay_buffer_length > buffer_length + write_position)
    {
        delay_buffer.copyFromWithRamp(channel, write_position, buffer_data, buffer_length, delay_mix, delay_mix);
        delay_buffer.addFromWithRamp(channel, write_position, temp_data, buffer_length, debug_4, debug_4);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - write_position;
        delay_buffer.copyFromWithRamp(channel, write_position, buffer_data, buffer_remaining, delay_mix, delay_mix);
        delay_buffer.addFromWithRamp(channel, write_position, temp_data, buffer_remaining, debug_4, debug_4);
        delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), delay_mix, delay_mix);
        delay_buffer.addFromWithRamp(channel, 0, temp_data, (buffer_length - buffer_remaining), debug_4, debug_4);
    }
}


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float* dry_buffer, float vol_dials[], float m_pan_dials[])
{
    AudioBuffer<float> temp(delay_buffer);
    delay_reverb.setInputBuffer(temp);     //TODO: check if need to set input buffer after FX
    const int read_position = static_cast<int> (delay_buffer_length + write_position - (sample_rate * delay_time / 1000)) % delay_buffer_length;
    auto* channelData = temp.getWritePointer(channel);
    applyFX(temp, instences[outputing_stage], channelData, channel, vol_dials[outputing_stage], m_pan_dials[outputing_stage]);
    if (reverb_instences[outputing_stage] == true)
    {
        //TODO: make the reverb work
        debug_4.addReverb(channel);
        //temp = delay_reverb.addReverb(channel);
        //const dsp::AudioBlock<float> block_1(delay_buffer);
        //dsp::AudioBlock<float> block_2(temp);
        //debug_4.setEnabled(true);
        //debug_4.process(dsp::ProcessContextNonReplacing<float>(block_1, block_2));
    }
    else
    {
        //debug_4.setEnabled(false);
    }
    addDelayinstenceToBuffer(buffer, temp, channel, buffer_length, read_position);
    //feedbackDelay(channel, buffer_length, dry_buffer);  //TODO: check if feedback before reverb and fX?? relevant??
    //TODO change time strech formula
    time_strecher++;
    float debug_2 = sample_rate/600;     
    if (channel == 1  &&  time_strecher >= debug_2 )
    {
        outputing_stage = (outputing_stage + 1) % 16;
        time_strecher = 0;
    }
    
}

void MyDelay::feedbackDelay(int channel, const int buffer_length, float* dry_buffer)
{
    float debug_1 = delay_mix * 0.6;       //TODO change debug to a reasonable value
    if (delay_buffer_length > buffer_length + write_position)
    {
        delay_buffer.addFromWithRamp(channel, write_position, dry_buffer, buffer_length, debug_1, debug_1);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - write_position;
        delay_buffer.addFromWithRamp(channel, buffer_remaining, dry_buffer, buffer_remaining, debug_1, debug_1);
        delay_buffer.addFromWithRamp(channel, 0, dry_buffer, buffer_length - buffer_remaining, debug_1, debug_1);
    }
}


void MyDelay::addInstence(int instance_num)
{
    marked_instences++;
    instences[instance_num] = 1;
}

void MyDelay::decreseInstence(int instance_num)
{
    marked_instences--;
    instences[instance_num] = 0;
}

int MyDelay::isMarked() { return marked_instences;}

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

void MyDelay::addDelayinstenceToBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& temp, int channel, int buffer_length, const int read_position)
{
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, temp.getReadPointer(channel) + read_position, buffer_length);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        const float* delay_buffer_data = temp.getReadPointer(channel);
        buffer.copyFrom(channel, 0, delay_buffer_data + read_position, buffer_remaining);
        buffer.copyFrom(channel, buffer_remaining, delay_buffer_data, buffer_length - buffer_remaining);
    }
}