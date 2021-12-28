/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyDelay.h"
#include "MyReverb.h"
#include "MyFilter.h"

#define NUM_OF_INSTENCES 16
#define INPUT_GAIN_ID "input gain"
#define INPUT_GAIN_NAME "Input Gain"
#define OUTPUT_GAIN_ID "output gain"
#define OUTPUT_GAIN_NAME "Output Gain"
#define DELAY_MIX_ID "delay mix"
#define DELAY_MIX_NAME "Delay Mix"


using namespace juce;
using namespace juce::dsp;


class NewProjectAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;


    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================

    float m_input_gain = 0.0f;
    float m_output_gain = 0.0f;
    int m_delay_time = 400;
    float m_delay_feedback = 0.4f;
    float previous_gain;
    

    // TODO : move param to stateValueTree
    //void process(dsp::ProcessContextReplacing<float> context);
    //void updateParameters();
    juce::AudioProcessorValueTreeState parameters;

    class Visualiser : public AudioVisualiserComponent
    {
    public:
        Visualiser() : AudioVisualiserComponent(2)
        {
            setBufferSize(512);
            setSamplesPerBlock(64);
            setColours(Colours::black, Colours::darkred);
        }
    };
    Visualiser m_visualiser;

    AudioBuffer<float> m_delay_buffer;
    MyDelay current_delay;
    int instence_position = 0;

    bool m_on_off_button_array[NUM_OF_INSTENCES]{ false };
    float m_volume_dials[NUM_OF_INSTENCES]{ 0.0f };
    float m_pan_dials[NUM_OF_INSTENCES]{ 0.0f };
    bool m_reverb_button_array[NUM_OF_INSTENCES]{ false };
    int marked = 0;

    void fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length,
        const float* buffer_data, const float* delay_buffer_data, float m_delay_feedback);

    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
        const float* buffer_data, const float* delay_buffer_data, int m_delay_time);

    void feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length,
        float* dry_buffer, float m_delay_feedback);


private:

    float last_sample_rate;
    int m_write_position{ 0 };
    int m_sample_rate{ 48000 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessor)
};