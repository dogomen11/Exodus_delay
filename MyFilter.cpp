/*
  ==============================================================================

    MyFilter.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/
#include "MyFilter.h"

/*
MyFilter::MyFilter(filter_logic new_logic) : filter_type(new_logic)
{
    for (int i = 0; i < 10; i++)
    {
        coefficients[i] = 0;
    }
}

void MyFilter::setFilterType(filter_type new_type)
{
    switch (new_type)
    {
    case (LOW_PASS):
        setLowPass();
        break;
    case (HIGH_PASS):
        setHighPass();
        break;
    case (BAND_PASS):
        setBandPass();
        break;
    }
}

void MyFilter::setSampleRate(int new_sample_rate)
{
    f_sample_rate = new_sample_rate;
}

void MyFilter::setLowPass()
{

}

void MyFilter::setHighPass()
{

}

void MyFilter::setBandPass()
{

}

AudioBuffer<float> MyFilter::applyFilter(int channel)
{
    AudioBuffer<float> gggg;
    return gggg;
}


/*
AudioBuffer<float> TwoTermDifferenceFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 1; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 1);
        second = in_buffer.getSample(channel, i);
        current_difference = second - first;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> TwoTermAverageFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 1; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 1);
        second = in_buffer.getSample(channel, i);
        current_difference = (second + first) / 2;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}


AudioBuffer<float> ThreeTermAverageFilter::applyFilter(int channel)
{
    float current_difference, first, second, third;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i - 1);
        third = in_buffer.getSample(channel, i);
        current_difference = (third + second + first) / 3;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> CenteralDifferenceFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i);
        current_difference = (second - first) / 2;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> RecursiveFilter::applyFilter(int channel)
{
    float current_difference, first, second, third;
    float rec_first, rec_second;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i - 1);
        first = in_buffer.getSample(channel, i);
        rec_first = out_buffer.getSample(channel, i - 1);
        rec_second = out_buffer.getSample(channel, i - 2);
        current_difference = (third + second + first - rec_first - rec_second) / 3;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

*/
MoogFilter::MoogFilter()
{
    y_a = 0;
    y_b = 0;
    y_c = 0;
    y_d = 0;
    y_d_1 = 0;

    frequency = 2000;
    resonance = 1;
    drive = 1.1;
    f_sample_rate = 0;
}

void MoogFilter::applyFilter(float* buffer)
{
    float tmp;
    for (int i = 0; i < 2 * f_sample_rate; i++)
    {
        tmp = tanhf(buffer[i / 2] * drive);
        if (tmp > 0)
        {
            tmp = tmp;
        }
        y_a = y_a + g * (tanhf(tmp - resonance * ((y_d_1 + y_d) / 2) - tanhf(y_a)));
        y_b = y_b + g * (tanhf(y_a) - tanhf(y_b));
        y_c = y_c + g * (tanhf(y_b) - tanhf(y_c));
        y_d_1 = y_d;
        y_d = y_d + g * (tanhf(y_c) - tanhf(y_d));
        buffer[i / 2] =  y_d;
    }
}


float MoogFilter::getFrequency()
{
    return frequency;
}

float MoogFilter::getResonance()
{
    return resonance;
}

float MoogFilter::getDrive()
{
    return drive;
}

void MoogFilter::setSampleRate(int new_sample_rate)
{
    f_sample_rate = new_sample_rate;
}

void MoogFilter::setFrequency(float new_freq)
{
    if (new_freq > 12000.0f) new_freq = 12000.0f;
    if (new_freq < 0.0f) new_freq = 0.0f;
    frequency = new_freq;
    g = 1 - expf(-2 * tanf(2 * M_PI * frequency / (2 * f_sample_rate)));
}

void MoogFilter::setResonance(float new_res)
{
    if (new_res > 5.0f) new_res = 5.0f;
    if (new_res < 0.0f) new_res = 0.0f;
    resonance = new_res;
}

void MoogFilter::setDrive(float d)
{
    if (d > 10.0f) d = 10.0f;
    if (d < 0.1f) d = 0.1f;
    drive = d;
}
