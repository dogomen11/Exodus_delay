/*
  ==============================================================================

    MyFilter.h
    Created: 28 Dec 2021 4:34:10pm
    Author:  97252

  ==============================================================================
*/

#pragma once


#ifndef MYFILTER
#define MYFILTER

#include <math.h>
#include <stdio.h>
#include <JuceHeader.h>

using namespace juce;
#define M_PI 3.14159265358979323846264338327950288

enum filter_logic
{
    TWO_TERM_DIFF_FILTER,
    TWO_TERM_AVG_FILTER,
    THREE_TERM_AVG_FILTER,
    CENTRAL_DIFF_FILTER,
    RECURSIVR_FILTER,
    MOOG_FILTER
};


class MyFilter
{
private:
    int logic;

    float frequency;
    float g;
    float resonance;
    float drive;
    int f_sample_rate;

    float y_a;
    float y_b;
    float y_c;
    float y_d;
    float y_d_1;

public:
    MyFilter();
    ~MyFilter() = default;
    void setSampleRate(int new_sample_rate);
    void applyFilter(int channel, float* buffer, float* tmp, int buffer_length);

    float getFrequency();
    float getResonance();
    float getDrive();

    void setFrequency(float new_freq);
    void setResonance(float new_res);
    void setDrive(float d);

    void setFilterLogic(int new_logic) { logic = new_logic; }
    void twoTermDifferenceFilter(int channel, float* buffer, float* tmp, int buffer_length);
    void twoTermAverageFilter(int channel, float* buffer, float* tmp, int buffer_length);
    void threeTermAverageFilter(int channel, float* buffer, float* tmp, int buffer_length);
    void centeralDifferenceFilter(int channel, float* buffer, float* tmp, int buffer_length);
    void recursiveFilter(int channel, float* buffer, float* tmp, int buffer_length);
    void moogFilter(int channel, float* buffer, float* tmp, int buffer_length);

};

#endif