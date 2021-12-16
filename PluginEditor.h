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
class ExodusAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ExodusAudioProcessorEditor (ExodusAudioProcessor&);
    ~ExodusAudioProcessorEditor() override;

    //==============================================================================
    void initiateComponents(AudioProcessor&);
    void printComponents();
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    Slider m_input_gain;
    Label m_input_gain_label;
    Slider m_output_gain;
    Label m_output_gain_label;

    Slider m_volume_dials[NUM_OF_INSTENCES];
    Slider m_pan_dials[NUM_OF_INSTENCES];
    ToggleButton m_on_off_buttons[NUM_OF_INSTENCES];
    ToggleButton m_reverb_buttons[NUM_OF_INSTENCES];

    Label m_volume_dials_label;
    Label m_pan_dials_label;
    Label m_on_off_buttons_label;
    Label m_reverb_buttons_label;

    Slider m_delay_time;
    Label m_delay_time_label;
    Slider m_delay_feedback;
    Label m_delay_feedback_label;

    ImageComponent m_background;

    void reAlphaComponents();
 
public:

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> silder_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> button_attach;

    ExodusAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExodusAudioProcessorEditor)
};
