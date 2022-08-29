/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;
using namespace std;
//==============================================================================
/**
*/
class ExodusAudioProcessorEditor :  public juce::AudioProcessorEditor,
                                    public juce::Button::Listener,
                                    public juce::Timer
                                    
{
public:
    ExodusAudioProcessorEditor (ExodusAudioProcessor&);
    ~ExodusAudioProcessorEditor() override;

    void juce::Button::Listener::buttonClicked(Button*) override;
    void reAlphaComponents();
    void initiateComponents(AudioProcessor&);
    void printComponents();
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    ToggleButton instence_indicator;

    void timerCallback() override;

    double winSizeRatio;

    // Input Gain
    Slider m_input_gain;
    Label m_input_gain_label;

    // Output Gain
    Slider m_output_gain;
    Label m_output_gain_label;

    // Volume
    Slider m_volume_dials[NUM_OF_INSTENCES];

    // Pan
    Slider m_pan_dials[NUM_OF_INSTENCES];

    // On/Off
    ToggleButton m_on_off_buttons[NUM_OF_INSTENCES];

    // Reverb
    ToggleButton m_reverb_buttons[NUM_OF_INSTENCES];

    // Lables
    Label m_volume_dials_label;
    Label m_pan_dials_label;
    Label m_on_off_buttons_label;
    Label m_reverb_buttons_label;

    // Delay time
    Slider m_delay_time;
    Label m_delay_time_label;

    // Delay feedback
    Slider m_delay_feedback;
    Label m_delay_feedback_label;

    // Delay mix
    Slider m_delay_mix;
    Label m_delay_mix_label;

    // Backgroungd 
    ImageComponent m_background;

public:

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_input_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_output_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_time_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_feedback_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_mix_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_volume_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pan_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_on_off_buttons_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_reverb_buttons_attach[NUM_OF_INSTENCES];

    ExodusAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExodusAudioProcessorEditor)
};
