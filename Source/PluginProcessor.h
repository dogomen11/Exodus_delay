/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyDistortion.h"
#include "MyFilter.h"

#define NUM_OF_INSTENCES 16
#define INSTENCE_ON 1
#define INSTENCE_OFF 0

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
    //==============================================================================
    //
    //
    //    ███████╗██╗░░██╗░█████╗░██████╗░██╗░░░██╗░██████╗
    //    ██╔════╝╚██╗██╔╝██╔══██╗██╔══██╗██║░░░██║██╔════╝
    //    █████╗░░░╚███╔╝░██║░░██║██║░░██║██║░░░██║╚█████╗░
    //    ██╔══╝░░░██╔██╗░██║░░██║██║░░██║██║░░░██║░╚═══██╗
    //    ███████╗██╔╝╚██╗╚█████╔╝██████╔╝╚██████╔╝██████╔╝
    //    ╚══════╝╚═╝░░╚═╝░╚════╝░╚═════╝░░╚═════╝░╚═════╝░
    //==============================================================================

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

    struct Parameters
    {
        float delay_mix = 0.7f;
        float delay_time = 0.5f;
        float delay_feedback = 0.33f;
        float delay_volume = 1.0f;
        float delay_pan = 0.0f;
    };

    Visualiser m_visualiser;
    Visualiser m_visualiser_2;
    AudioBuffer<float> wet_delay_buffer;
    Parameters parameters;
    dsp::Reverb reverb;
    dsp::Reverb::Parameters reverb_params;
    MyDistortion distortion;
    MyDistortion::Parameters dist_params;

    int current_instence = 0;
    int processor_buffer_write_pos = 0;
    int delay_buffer_length;
    double sample_rate;
    int on_off_marked = INSTENCE_OFF;
    int reverb_marked = INSTENCE_OFF;
    int dist_marked = INSTENCE_OFF;
    bool d_on_off[NUM_OF_INSTENCES] = { INSTENCE_OFF };
    bool d_reverb[NUM_OF_INSTENCES] = { INSTENCE_OFF };
    bool d_dist[NUM_OF_INSTENCES] = { INSTENCE_OFF };

    void setSampleRate(double new_sample_rate);
    void setSize(int new_num_channels, int new_num_samples);
    void setParameters(const Parameters& new_params);
    void fillDelayBuffer(int channel, const int buffer_length, const float* read_pointer, int buffer_write_position);
    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position);
    void feedbackDelay(int channel, const int buffer_length, float* write_pointer, int buffer_write_position);
    void updateDelaySettings();
    void updateReverbSettings();
    void updateDistortionSettings();
    void promoteInstence();
    void applyFX(int channel);
    void applyPan(int channel);
    void applyVolume(int channel);
    void applyReverb(int channel);
    void applyDist(int channel);
    float calculatePanMargin(float pan, int channel);
    void addOnOffMarked(int instence);
    void subOnOffMarked(int instence);
    bool addReverbMarked(int instence);
    bool subReverbMarked(int instence);
    bool addDistMarked(int instence);
    bool subDistMarked(int instence);
    int getOnOffMarked() { return on_off_marked; }
    int getReverbMarked() { return reverb_marked; }
    int getNumSamples() { return wet_delay_buffer.getNumSamples(); }
    float getDelayTime() { return parameters.delay_time; }
    float getDelayMix() { return parameters.delay_mix; }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExodusAudioProcessor)
};
