/*
  ==============================================================================

	MyReverb.h
	Created: 10 Oct 2021 9:53:28pm
	Author:  dogom

  ==============================================================================
*/
#ifndef MYREVERB
#define MYREVERB
#pragma once
#include <JuceHeader.h>
#include <deque>
using namespace juce;

//==============================================================================
class MyReverb
{
private:

	//  Coefficients to set up the reverb architecture

	const int num_delay_lines;
	float b_gain;
	float c_gain[12][2];
	float balance_dry;
	const int delay_line_lengths[12];
	bool is_clipping;
	float input_gain;
	float output_gain;
	int input_buffer_size;
	dsp::Matrix <float> feedback_matrix;
	OwnedArray <dsp::Matrix <float>> feedback_matrix_rows;
	OwnedArray <dsp::IIR::Filter <float>> dampening_filters_tab;
	ReferenceCountedArray <dsp::IIR::Coefficients <float>> dampening_filters_coeffs_tab;
	OptionalScopedPointer <dsp::IIR::Filter <float>> tonal_correction_filter_ptr;
	dsp::IIR::Coefficients <float>::Ptr tonal_correction_filter_coeffs_ptr;

	//  Input and output buffers 

	AudioBuffer <float> input_buffer;
	AudioBuffer <float> output_buffer;

	//  Values read from sliders

	float balance_current_value;
	float time_current_value;
	float dampening_current_value;
	float init_current_value;

	//  Inside class methods

	void setupGainC(float coeff = 1.0);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyReverb)

public:

	MyReverb();
	~MyReverb();

	bool getIsClipping();

	void setBalanceCurrentValue(float balance_value);
	void setTimeCurrentValue(float time_value);
	void setDampeningCurrentValue(float dampening_value);
	void setInitCurrentValue(float init_value);

	void setInputGain(float in_gain_chosen);
	void setOutputGain(float out_gain_chosen);

	void setInputBuffer(AudioBuffer <float>& new_buffer);

	void setupMyReverb();
	AudioBuffer <float>& addReverb(int channel);


};

#endif     // MYREVERB