/*
  ==============================================================================

    MyDistortion.cpp
    Created: 7 Sep 2022 3:41:29pm
    Author:  Dogom

  ==============================================================================
*/

#include "MyDistortion.h"
using namespace juce;

MyDistortion::MyDistortion()
{
    dist_buffer.clear();
    brightness_filter.reset();
    balance = parameters.dist_dry / parameters.dist_wet;
}

void MyDistortion::setSampleRate(double new_sample_rate)
{
    sample_rate = new_sample_rate;
}

void MyDistortion::setSize(int new_num_channels, int new_num_samples)
{
    dist_buffer.setSize(new_num_channels, new_num_samples);
    dist_buffer_length = dist_buffer.getNumSamples();
}

void MyDistortion::setParameters(const Parameters& new_params) 
{
    parameters.dist_drive = new_params.dist_drive;
    parameters.dist_dry = new_params.dist_dry;
    parameters.dist_wet = new_params.dist_wet;
    parameters.dist_brightness = new_params.dist_brightness;
    setBalance();
}

void MyDistortion::setBalance()
{
    balance = parameters.dist_dry / parameters.dist_wet;
}

void MyDistortion::prepareFilter(const dsp::ProcessSpec& spec)
{
    brightness_filter.prepare(spec);
}

void MyDistortion::reset() 
{
    dist_buffer.clear();
}

float MyDistortion::distorter(float to_distort, float balance)
{
    float dry_sample = to_distort, sign = to_distort/abs(to_distort);
    float exponant = expf(to_distort);
    to_distort = 1 - exponant;
    return (to_distort * sign);
}

void MyDistortion::process(AudioBuffer<float>& buffer, int channel, int buffer_write_position, dsp::AudioBlock<float> audio_block)
{
    brightness_filter.setCutoffFrequency(exp(log(20) + (log(20000) - log(20)) * parameters.dist_brightness));
    for (int i = 0; i < dist_buffer_length; i++)
    {
        float processed_sample = distorter(buffer.getSample(channel, i), balance);
        buffer.setSample(channel, i, processed_sample);
    }
    //buffer.copyFrom(channel, buffer_write_position, dist_buffer.getReadPointer(channel), dist_buffer_length);
    //dsp::ProcessContextReplacing<float> ctx(audio_block);
    //brightness_filter.process(ctx);
}