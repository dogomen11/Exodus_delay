/*
  ==============================================================================

    MyDelay.cpp
    Created: 16 Dec 2021 12:17:40am
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

float calculatePanMargin(float pan, int channel);

MyDelay::MyDelay()
{
    delay_buffer.clear();
}


void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = delay_buffer.getNumSamples();
}

void MyDelay::setSampleRate(double new_sample_rate)
{
    sample_rate = new_sample_rate;
}

void MyDelay::setDelayTime(float new_delay_time)
{
    delay_time = new_delay_time;
}

void MyDelay::setDelayFeedback(float new_delay_feedback)
{
    delay_feedback = new_delay_feedback;
}

void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data, int buffer_write_position)
{
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_length, delay_feedback, delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        delay_buffer.copyFromWithRamp(channel, buffer_write_position, buffer_data, buffer_remaining, delay_feedback, delay_feedback);
        delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), delay_feedback, delay_feedback);
    }
}


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position)
{
    jassert(delay_buffer.getArrayOfReadPointers() != nullptr);
    const int read_position = static_cast<int> (delay_buffer_length + buffer_write_position - (sample_rate * delay_time / 1000)) % delay_buffer_length;
    const float* delay_buffer_data = delay_buffer.getReadPointer(channel);
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, delay_buffer_data + read_position, buffer_length);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        buffer.copyFrom(channel, 0, delay_buffer_data + read_position, buffer_remaining);
        buffer.copyFrom(channel, buffer_remaining, delay_buffer_data, buffer_length - buffer_remaining);
    }
}


void MyDelay::feedbackDelay(int channel, const int buffer_length, float* dry_buffer, int buffer_write_position)
{
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        delay_buffer.addFromWithRamp(channel, buffer_write_position, dry_buffer, buffer_length, delay_feedback, delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        delay_buffer.addFromWithRamp(channel, buffer_remaining, dry_buffer, buffer_remaining, delay_feedback, delay_feedback);
        delay_buffer.addFromWithRamp(channel, 0, dry_buffer, buffer_length - buffer_remaining, delay_feedback, delay_feedback);
    }
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


void MyDelay::addMark(int instence) {
    d_on_off[instence] = true;
    marked++;
}
void MyDelay::subMark(int instence) {
    d_on_off[instence] = false;
    marked--;
}
