/*
  ==============================================================================

    MyDelay.cpp
    Created: 16 Dec 2021 12:17:40am
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

float calculatePanMargin(float pan, int channel);

MyDelay::MyDelay() : reverb(), distortion()
{
    dry_delay_buffer.clear();
    wet_delay_buffer.clear();
}


void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    dry_delay_buffer.setSize(new_num_channels, new_num_samples);
    wet_delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = dry_delay_buffer.getNumSamples();
}

void MyDelay::setParameters(const Parameters& new_params)
{
    parameters.delay_time = new_params.delay_time;
    parameters.delay_feedback = new_params.delay_feedback;
    parameters.delay_mix = new_params.delay_mix;
}

void MyDelay::setSampleRate(double new_sample_rate)
{
    sample_rate = new_sample_rate;
}

void MyDelay::applyFX(int channel)
{
    //applyVolume(channel);
    //applyDist(channel);
    //applyReverb(channel);
    //applyPan(channel);
}

void MyDelay::applyVolume(int channel)
{
    float* channelData = wet_delay_buffer.getWritePointer(channel);
    for (int sample = 0; sample < wet_delay_buffer.getNumSamples(); ++sample)
    {
        channelData[sample] = wet_delay_buffer.getSample(channel, sample) * parameters.delay_volume;
    }
}

void MyDelay::applyPan(int channel)
{
    float pan_mult = 1;
    float* channelData = wet_delay_buffer.getWritePointer(channel);
    if (parameters.delay_pan != 0)
    {
        pan_mult = calculatePanMargin(parameters.delay_pan, channel);

        for (int sample = 0; sample < wet_delay_buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = wet_delay_buffer.getSample(channel, sample) * pan_mult;
        }
    }
}

void MyDelay::applyReverb(int channel)
{
    dsp::AudioBlock<float> audio_block{ wet_delay_buffer };
    juce::dsp::ProcessContextReplacing<float> ctx(audio_block);
    reverb.process(ctx);
}

void MyDelay::applyDist(int channel)
{
    distortion.process(wet_delay_buffer, channel);
}


void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data, int buffer_write_position)
{
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        dry_delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_length, parameters.delay_feedback, parameters.delay_feedback);
        wet_delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_length, parameters.delay_feedback, parameters.delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        dry_delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
        wet_delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
        dry_delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), parameters.delay_feedback, parameters.delay_feedback);
        wet_delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), parameters.delay_feedback, parameters.delay_feedback);
    }
}


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position)
{
    const int read_position = static_cast<int> (delay_buffer_length + buffer_write_position - (sample_rate * parameters.delay_time / 1000)) % delay_buffer_length;
    const float* delay_buffer_data = wet_delay_buffer.getReadPointer(channel);
    //applyFX(channel);
    //applyReverb(channel);
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, delay_buffer_data + read_position, buffer_length, powf((parameters.delay_mix / 100), 1.5));  //TODO: change delay mix divider
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        buffer.copyFrom(channel, 0, delay_buffer_data + read_position, buffer_remaining, powf((parameters.delay_mix / 100), 1.5));
        buffer.copyFrom(channel, buffer_remaining, delay_buffer_data, buffer_length - buffer_remaining, powf((parameters.delay_mix / 100), 1.5));
    }
}


void MyDelay::feedbackDelay(int channel, const int buffer_length, float* dry_buffer, int buffer_write_position)
{
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        dry_delay_buffer.addFromWithRamp(channel, buffer_write_position, dry_buffer, buffer_length, parameters.delay_feedback, parameters.delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        dry_delay_buffer.addFromWithRamp(channel, buffer_remaining, dry_buffer, buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
        dry_delay_buffer.addFromWithRamp(channel, 0, dry_buffer, buffer_length - buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
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


void MyDelay::addOnOffMarked(int instence) 
{
    d_on_off[instence] = true;
    on_off_marked++;
}
void MyDelay::subOnOffMarked(int instence)
{
    d_on_off[instence] = false;
    on_off_marked--;
    d_reverb[instence] = false;
}

bool MyDelay::addReverbMarked(int instence)
{
    if (d_on_off[instence] == false)
    {
        return false;
    }

    d_reverb[instence] = true;
    reverb_marked++;
    return true;
}
bool MyDelay::subReverbMarked(int instence)
{
    d_reverb[instence] = false;
    reverb_marked--;
    return true;
}

bool MyDelay::addDistMarked(int instence)
{
    if (d_on_off[instence] == false)
    {
        return false;
    }

    d_dist[instence] = true;
    dist_marked++;
    return true;
}
bool MyDelay::subDistMarked(int instence)
{
    d_dist[instence] = false;
    dist_marked--;
    return true;
}