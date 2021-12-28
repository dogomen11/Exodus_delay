/*
  ==============================================================================

	MyReverb.cpp
	Created: 10 Oct 2021 9:53:28pm
	Author:  dogom

  ==============================================================================
*/

#include "MyReverb.h"


//==============================================================================
MyReverb::MyReverb() :	num_delay_lines(12),
						b_gain(1.0),
						balance_dry(1.0),
						feedback_matrix(12, 12),
						delay_line_lengths{ 601, 1399, 1747, 2269, 2707, 3089, 3323, 3571, 3911, 4127, 4639, 4999 },     //try different occurences
						is_clipping(false),
						input_gain(0.0),
						output_gain(0.0),
						input_buffer_size(0),
						balance_current_value(0.0),
						time_current_value(1.5),
						dampening_current_value(75.0),
						init_current_value(0.0)
{
	setupGainC(0.0);

	dsp::Matrix <float> toeplitz_vector(12, 1);
	dsp::Matrix <float> circulant_matrix(12, 12);    //toeplitz matrix 12X12
	dsp::Matrix <float> u_vector(12, 1);
	dsp::Matrix <float> u_vector_transposed(1, 12);

	//initiate circulant_mat as toeplitz by a vector
	toeplitz_vector.clear();
	toeplitz_vector(1, 0) = 1.0f;
	circulant_matrix = circulant_matrix.toeplitz(toeplitz_vector, 12);
	for (int counter = 0; counter < 11; ++counter) // initialize
	{
		circulant_matrix(counter, counter + (int)1) = 0.0;
	}
	circulant_matrix(0, 11) = 1.0;
	for (int counter = 0; counter < 12; ++counter)
	{
		u_vector(counter, 0) = 1.0;
		u_vector_transposed(0, counter) = 1.0;
	}
	//assign
	feedback_matrix = circulant_matrix - ((u_vector * (2.0 / num_delay_lines)) * u_vector_transposed);

	for (int counter = 0; counter < 12; ++counter)
		feedback_matrix_rows.add( new dsp::Matrix <float>(1, 12, feedback_matrix.getRawDataPointer() + counter * (int)12) );

	input_buffer.clear();
	output_buffer.clear();
	dampening_filters_tab.clear();
	dampening_filters_coeffs_tab.clear();
}

MyReverb::~MyReverb() {}

bool MyReverb::getIsClipping()
{
	return is_clipping;
}

void MyReverb::setBalanceCurrentValue(float balance_value)
{
	balance_current_value = balance_value;
}

void MyReverb::setTimeCurrentValue(float time_value)
{
	time_current_value = time_value;
}

void MyReverb::setDampeningCurrentValue(float dampening_value)
{
	dampening_current_value = dampening_value;
}

void MyReverb::setInitCurrentValue(float init_value)
{
	init_current_value = init_value;
}

void MyReverb::setInputGain(float in_gain_chosen)
{
	input_gain = in_gain_chosen;
}

void MyReverb::setOutputGain(float out_gain_chosen)
{
	output_gain = out_gain_chosen;
}

void MyReverb::setInputBuffer(AudioBuffer <float>& new_buffer)
{
	input_buffer.makeCopyOf(new_buffer);
	input_buffer_size = input_buffer.getNumSamples();
	is_clipping = false;
}

void MyReverb::setupMyReverb()
{
	int new_num_samples = input_buffer_size + int(480.00 * time_current_value) + int(std::ceil(48.0 * init_current_value));
	output_buffer.setSize(input_buffer.getNumChannels(), new_num_samples, false, true, false);
	input_buffer.setSize(input_buffer.getNumChannels(), output_buffer.getNumSamples(), true, true, false);

	b_gain = Decibels::decibelsToGain(input_gain);
	output_gain = Decibels::decibelsToGain(output_gain);

	// take care of balanced gain
	if (balance_current_value <= 0.0)
	{
		setupGainC(std::pow(balance_current_value + 1.0, 4));
		balance_dry = 1.0;
	}
	else if (balance_current_value > 0.0)
	{
		setupGainC();
		balance_dry = std::pow(1.0 - balance_current_value, 4);
	}

	//damp and correction clear
	dampening_filters_tab.clear();
	dampening_filters_coeffs_tab.clear();
	tonal_correction_filter_ptr.clear();
	tonal_correction_filter_coeffs_ptr.reset();

	float min_alpha_const = (4.0 * time_current_value) / (-3.0 * 4999.0 * (1.0 / 48000.0) * std::log(10.0));
	float min_alpha = std::sqrt(1.0 / (1.0 - min_alpha_const));
	float alpha = min_alpha + ((100.0 - dampening_current_value) * ((1.0 - min_alpha) / 100.0));

	float g_coeff = 0.0;
	float p_coeff = 0.0;
	float const_element1 = std::log(10.0) / 4.0;
	float const_element2 = 1.0 - (1.0 / std::pow(alpha, 2));

	for (int counter = 0; counter < 12; ++counter)
	{
		g_coeff = std::pow(10.0, (-3.0 * delay_line_lengths[counter] * (1.0 / 48000.0)) / time_current_value);
		p_coeff = const_element1 * const_element2 * std::log10(g_coeff);
		dampening_filters_coeffs_tab.add(new dsp::IIR::Coefficients <float>(g_coeff * (1.0 - p_coeff), 0.0, 1.0, -p_coeff));
		dampening_filters_tab.add(new dsp::IIR::Filter <float>(dampening_filters_coeffs_tab.getLast()));
	}

	float beta = (1.0 - alpha) / (1.0 + alpha);
	tonal_correction_filter_coeffs_ptr = new dsp::IIR::Coefficients <float>(1.0, -beta, 1.0 - beta, 0.0);
	tonal_correction_filter_ptr.set(new dsp::IIR::Filter <float>(tonal_correction_filter_coeffs_ptr.get()), true);

	is_clipping = false;
}

AudioBuffer <float>& MyReverb::addReverb(int channel)
{
	ScopedNoDenormals noDenormals;

	int init_delay_line_length = 1;

	if (init_current_value != 0.0)
		init_delay_line_length = int(std::round(init_current_value * 48.0));

	std::deque <float> init_delay_line(init_delay_line_length, 0.0);
	OwnedArray <std::deque <float>> delay_lines;
	dsp::Matrix <float> temp_matrix(12, 1);
	float out_sample = 0.0;
	float current_sample = 0.0;
	dsp::Matrix <float> current_matrix_product(1, 1);

	for (int counter = 0; counter < 12; ++counter)
	{
		delay_lines.add(new std::deque <float>(4999, 0.0));
	}

	
		float* output_write = output_buffer.getWritePointer(channel);
		const float* input_read = input_buffer.getReadPointer(channel);

		for (auto sample = 0; sample < output_buffer.getNumSamples(); ++sample)
		{
			out_sample = 0.0;
			for (int counter = 0; counter < 12; ++counter)
			{
				temp_matrix(counter, 0) = (*delay_lines[counter])[delay_line_lengths[counter] - (int)1];
				out_sample += temp_matrix(counter, 0) * c_gain[counter][channel];
				current_matrix_product = *feedback_matrix_rows[counter] * temp_matrix;
				current_sample = (input_read[sample] * b_gain) + *(current_matrix_product.getRawDataPointer());
				current_sample = dampening_filters_tab[counter]->processSample(current_sample);
				delay_lines[counter]->push_front(current_sample);
				delay_lines[counter]->pop_back();
			}

			out_sample = tonal_correction_filter_ptr.get()->processSample(out_sample);
			init_delay_line.push_front(out_sample);
			init_delay_line.pop_back();
			output_write[sample] = ((input_read[sample] * b_gain * balance_dry)
				+ init_delay_line.back()) * output_gain;

			if (is_clipping == false)
			{
				if (output_write[sample] > 1.0 || output_write[sample] < -1.0)
					is_clipping = true;
			}

		for (int counter = 0; counter < 12; ++counter)
			dampening_filters_tab[counter]->reset();
		tonal_correction_filter_ptr->reset();
	}

	auto a = output_buffer.getMagnitude(0, output_buffer.getNumSamples());
	return output_buffer;
}

void MyReverb::setupGainC(float coeff)
{
	//set up the coeffs to output normalized value
	unsigned short flag = 1;
	for (int counter_1 = 0; counter_1 < 12; ++counter_1)
	{
		for (int counter_2 = 0; counter_2 < 2; ++counter_2)
		{
			if (flag % 3 != 0 && flag != 7 && flag != 8)
				c_gain[counter_1][counter_2] = 1.0 * coeff;
			else
				c_gain[counter_1][counter_2] = -1.0 * coeff;
			++flag;
			if (flag == 9) flag = 1;
		}
	}
}

