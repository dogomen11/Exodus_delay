/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyDelay.h"
#include "MyDistortion.h"
#include "MyFilter.h"

//==============================================================================
/**
*/
class ExodusAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ExodusAudioProcessor();
    ~ExodusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState tree_state;
    AudioProcessorValueTreeState::ParameterLayout create_parameter_layout();

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
    Visualiser m_visualiser_2;

    MyDelay delay;
    MyDelay::Parameters delay_params;
    dsp::Reverb reverb;
    dsp::Reverb::Parameters reverb_params;
    MyDistortion distortion;
    MyDistortion::Parameters dist_params;
    void updateDelaySettings();
    void updateReverbSettings();
    void updateDistortionSettings();
    int current_instence = 0;
    int processor_buffer_write_pos = 0;
    void promoteInstence();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExodusAudioProcessor)
};
