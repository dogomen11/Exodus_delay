/*
  ==============================================================================

    MyFilter.cpp
    Created: 28 Dec 2021 4:34:10pm
    Author:  97252

  ==============================================================================
*/

#include "MyFilter.h"


MyFilter::MyFilter()
{
    logic = MOOG_FILTER;
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

void MyFilter::applyFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    if (logic == TWO_TERM_DIFF_FILTER)
    {
        twoTermDifferenceFilter(channel, buffer, tmp, buffer_length);
    }
    else if (logic == TWO_TERM_AVG_FILTER)
    {
        twoTermAverageFilter(channel, buffer, tmp, buffer_length);
    }
    else if (logic == THREE_TERM_AVG_FILTER)
    {
        threeTermAverageFilter(channel, buffer, tmp, buffer_length);
    }
    else if (logic == CENTRAL_DIFF_FILTER)
    {
        centeralDifferenceFilter(channel, buffer, tmp, buffer_length);
    }
    else if (logic == RECURSIVR_FILTER)
    {
        recursiveFilter(channel, buffer, tmp, buffer_length);
    }
    else if (logic == MOOG_FILTER)
    {
        moogFilter(channel, buffer, tmp, buffer_length);
    }
}


float MyFilter::getFrequency()
{
    return frequency;
}

float MyFilter::getResonance()
{
    return resonance;
}

float MyFilter::getDrive()
{
    return drive;
}

void MyFilter::setSampleRate(int new_sample_rate)
{
    f_sample_rate = new_sample_rate;
}

void MyFilter::setFrequency(float new_freq)
{
    if (new_freq > 12000.0f) new_freq = 12000.0f;
    if (new_freq < 0.0f) new_freq = 0.0f;
    frequency = new_freq;
    g = 1 - expf(-2 * tanf(2 * M_PI * frequency / (2 * f_sample_rate)));
}

void MyFilter::setResonance(float new_res)
{
    if (new_res > 5.0f) new_res = 5.0f;
    if (new_res < 0.0f) new_res = 0.0f;
    resonance = new_res;
}

void MyFilter::setDrive(float d)
{
    if (d > 10.0f) d = 10.0f;
    if (d < 0.1f) d = 0.1f;
    drive = d;
}

/////****************************************************************************************************************************//

void MyFilter::twoTermDifferenceFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    for (int i = 1; i < buffer_length; i++)
    {
        buffer[i] = (tmp[i] - tmp[i - 1]);
    }
}

void MyFilter::twoTermAverageFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    for (int i = 1; i < buffer_length; i++)
    {
        buffer[i] = ((tmp[i] + tmp[i - 1]) / 2);
    }
}


void MyFilter::threeTermAverageFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    for (int i = 2; i < buffer_length; i++)
    {
        buffer[i] = (tmp[i] + tmp[i - 1] + tmp[i - 2]) / 3;
    }
}

void MyFilter::centeralDifferenceFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    for (int i = 2; i < buffer_length; i++)
    {
        buffer[i] = ((tmp[i] - tmp[i - 2]) / 3);
    }
}

void MyFilter::recursiveFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    for (int i = 2; i < buffer_length; i++)
    {

        buffer[i] = ((tmp[i] + tmp[i - 1] + tmp[i - 2] - buffer[i - 1] - buffer[i]) / 3);
    }
}



void MyFilter::moogFilter(int channel, float* buffer, float* tmp, int buffer_length)
{
    //float tmp, current_sample;
    /*
    for (int i = 0; i < 2 * buffer.getNumSamples(); i++)
    {
        current_sample = buffer.getSample(channel, (i / 2));
        tmp = tanhf(current_sample * drive);
        if (tmp > 0)
        {
            tmp = tmp;
        }
        y_a = y_a + g * (tanhf(tmp - resonance * ((y_d_1 + y_d) / 2) - tanhf(y_a)));
        y_b = y_b + g * (tanhf(y_a) - tanhf(y_b));
        y_c = y_c + g * (tanhf(y_b) - tanhf(y_c));
        y_d_1 = y_d;
        y_d = y_d + g * (tanhf(y_c) - tanhf(y_d));
        buffer.setSample(channel, (i / 2), y_d);
    }*/
    for (int i = 0; i < 2 * buffer_length; i++) 
    {
        buffer[i / 2] = tanhf(buffer[i / 2] * drive);
        y_a = y_a + g * (tanhf(buffer[i / 2] - resonance * ((y_d_1 + y_d) / 2) - tanhf(y_a)));
        y_b = y_b + g * (tanhf(y_a) - tanhf(y_b));
        y_c = y_c + g * (tanhf(y_b) - tanhf(y_c));
        y_d_1 = y_d;
        y_d = y_d + g * (tanhf(y_c) - tanhf(y_d));
        buffer[i / 2] = y_d;
    }
}
